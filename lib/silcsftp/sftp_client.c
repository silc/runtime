/*

  sftp_client.c 

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 2001 Pekka Riikonen

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/
/* $Id$ */

#include "silcincludes.h"
#include "silcsftp.h"
#include "sftp_util.h"

/* Request context. Every request will allocate this context and set
   the correct callback function according the `type' field. */
typedef struct {
  uint32 id;
  SilcSFTPPacket type;
  SilcSFTPStatusCallback status;
  SilcSFTPHandleCallback handle;
  SilcSFTPDataCallback data;
  SilcSFTPNameCallback name;
  SilcSFTPAttrCallback attr;
  SilcSFTPExtendedCallback extended;
  void *context;
} *SilcSFTPRequest;

/* SFTP client context */
typedef struct {
  SilcSocketConnection sock;
  SilcSFTPSendPacketCallback send_packet;
  void *send_context;
  SilcSFTPVersionCallback version;
  void *version_context;
  uint32 id;
  SilcDList requests;
} *SilcSFTPClient;

/* File handle */
struct SilcSFTPHandleStruct {
  unsigned char *data;
  uint32 data_len;
};

/* Creates SilcSFTPHandle and returns pointer to it. The caller must free
   the context. */

static SilcSFTPHandle silc_sftp_handle_create(unsigned char *data,
					      uint32 data_len)
{
  SilcSFTPHandle handle;

  handle = silc_calloc(1, sizeof(*handle));
  handle->data = silc_calloc(data_len, sizeof(*handle->data));
  memcpy(handle->data, data, data_len);
  handle->data_len = data_len;

  return handle;
}

/* Deletes the handle indicated by the `handle'. */

static void silc_sftp_handle_delete(SilcSFTPHandle handle)
{
  silc_free(handle->data);
  silc_free(handle);
}

/* Returns the handle data of the `handle' to the `data' pointer. */

static void silc_sftp_handle_get(SilcSFTPHandle handle, 
				 const unsigned char **data,
				 uint32 *data_len)
{
  *data = (const unsigned char *)handle->data;
  *data_len = handle->data_len;
}

/* General routine to send SFTP packet to the SFTP server. */

static void silc_sftp_send_packet(SilcSFTPClient sftp,
				  SilcSFTPPacket type, 
				  uint32 len, ...)
{
  SilcBuffer packet;
  va_list vp;

  va_start(vp, len);
  packet = silc_sftp_packet_encode_vp(type, len, vp);
  va_end(vp);

  if (!packet)
    return;

  SILC_LOG_HEXDUMP(("SFTP packet to server"), packet->data, packet->len);

  /* Send the packet */
  (*sftp->send_packet)(sftp->sock, packet, sftp->send_context);

  silc_buffer_free(packet);
}

/* Finds request by request ID. */

static SilcSFTPRequest silc_sftp_find_request(SilcSFTPClient sftp, uint32 id)
{
  SilcSFTPRequest req;

  SILC_LOG_DEBUG(("Finding request ID: %d", id));

  silc_dlist_start(sftp->requests);
  while ((req = silc_dlist_get(sftp->requests)) != SILC_LIST_END) {
    if (req->id == id)
      return req;
  }

  SILC_LOG_DEBUG(("Unknown request ID"));

  return NULL;
}

/* Function used to call the request callback indicated by the `req'. The
   `status' will be sent to the callback function as the status of the
   operation. The variable argument list includes the status and req->type
   specific data. */

static void silc_sftp_call_request(SilcSFTPClient sftp, 
				   SilcSFTPRequest req, 
				   SilcSFTPPacket type,
				   SilcSFTPStatus status, ...)
{
  va_list vp;

  SILC_LOG_DEBUG(("Start"));

  va_start(vp, status);

  switch (req->type) {
  case SILC_SFTP_READ:
    {
      /* Data returned */
      unsigned char *data;
      uint32 data_len;

      if (status != SILC_SFTP_STATUS_OK) {
	if (req->data)
	  (*req->data)((SilcSFTP)sftp, status, NULL, 0, req->context);
	break;
      }

      data = (unsigned char *)va_arg(vp, unsigned char *);
      data_len = (uint32)va_arg(vp, uint32);

      if (req->data)
	(*req->data)((SilcSFTP)sftp, status, (const unsigned char *)data, 
		     data_len, req->context);
    }    
    break;

  case SILC_SFTP_OPEN:
  case SILC_SFTP_OPENDIR:
    {
      /* Handle returned */
      SilcSFTPHandle handle;
      unsigned char *hdata;
      uint32 hdata_len;

      if (status != SILC_SFTP_STATUS_OK) {
	if (req->handle)
	  (*req->handle)((SilcSFTP)sftp, status, NULL, req->context);
	break;
      }

      hdata = (unsigned char *)va_arg(vp, unsigned char *);
      hdata_len = (uint32)va_arg(vp, uint32);
      handle = silc_sftp_handle_create(hdata, hdata_len);

      if (req->handle)
	(*req->handle)((SilcSFTP)sftp, status, handle, req->context);
    }
    break;

  case SILC_SFTP_CLOSE:
  case SILC_SFTP_WRITE:
  case SILC_SFTP_REMOVE:
  case SILC_SFTP_RENAME:
  case SILC_SFTP_MKDIR:
  case SILC_SFTP_RMDIR:
  case SILC_SFTP_SETSTAT:
  case SILC_SFTP_FSETSTAT:
  case SILC_SFTP_SYMLINK:
    {
      /* Status returned */
      char *message, *language_tag;

      message = (char *)va_arg(vp, char *);
      language_tag = (char *)va_arg(vp, char *);

      if (req->status)
	(*req->status)((SilcSFTP)sftp, status, (const char *)message, 
		       (const char *)language_tag, req->context);
    }
    break;

  case SILC_SFTP_STAT:
  case SILC_SFTP_LSTAT:
  case SILC_SFTP_FSTAT:
    {
      /* Attributes returned */
      SilcSFTPAttributes attr;

      if (status != SILC_SFTP_STATUS_OK) {
	if (req->attr)
	  (*req->attr)((SilcSFTP)sftp, status, NULL, req->context);
	break;
      }

      attr = (SilcSFTPAttributes)va_arg(vp, SilcSFTPAttributes);

      if (req->attr)
	(*req->attr)((SilcSFTP)sftp, status, (const SilcSFTPAttributes)attr, 
		     req->context);
    }
    break;

  case SILC_SFTP_READDIR:
  case SILC_SFTP_REALPATH:
  case SILC_SFTP_READLINK:
    {
      /* Name(s) returned */
      SilcSFTPName name;

      if (status != SILC_SFTP_STATUS_OK) {
	if (req->name)
	  (*req->name)((SilcSFTP)sftp, status, NULL, req->context);
	break;
      }

      name = (SilcSFTPName)va_arg(vp, SilcSFTPName);

      if (req->name)
	(*req->name)((SilcSFTP)sftp, status, name, req->context);
    }
    break;

  case SILC_SFTP_EXTENDED:
    {
      /* Extended reply returned */
      unsigned char *data;
      uint32 data_len;

      if (status != SILC_SFTP_STATUS_OK) {
	if (req->extended)
	  (*req->extended)((SilcSFTP)sftp, status, NULL, 0, req->context);
	break;
      }

      data = (unsigned char *)va_arg(vp, unsigned char *);
      data_len = (uint32)va_arg(vp, uint32);

      if (req->extended)
	(*req->extended)((SilcSFTP)sftp, status, (const unsigned char *)data, 
			 data_len, req->context);
    }
    break;

  default:
    break;
  }

  /* Remove this request */
  silc_dlist_del(sftp->requests, req);
  silc_free(req);

  va_end(vp);
}

/* Starts SFTP client by associating the socket connection `sock' to the
   created SFTP client context.  The version callback indicated by the
   `callback' will be called after the SFTP session has been started
   and server has returned the version of the protocol.  The SFTP client
   context is returned in the callback too.  This returns the allocated
   SFTP client context or NULL on error. */

SilcSFTP silc_sftp_client_start(SilcSocketConnection sock,
				SilcSFTPSendPacketCallback send_packet,
				void *send_context,
				SilcSFTPVersionCallback callback,
				void *context)
{
  SilcSFTPClient sftp;

  if (!send_packet)
    return NULL;

  sftp = silc_calloc(1, sizeof(*sftp));
  sftp->sock = sock;
  sftp->send_packet = send_packet;
  sftp->send_context = send_context;
  sftp->version = callback;
  sftp->version_context = context;
  sftp->requests = silc_dlist_init();

  /* Send the SFTP session initialization to the server */
  silc_sftp_send_packet(sftp, SILC_SFTP_INIT, 4, 
			SILC_STR_UI_INT(SILC_SFTP_PROTOCOL_VERSION),
			SILC_STR_END);

  return (SilcSFTP)sftp;
}

/* Shutdown's the SFTP client.  The caller is responsible of closing
   the associated socket connection.  The SFTP context is freed and is
   invalid after this function returns. */

void silc_sftp_client_shutdown(SilcSFTP context)
{
  SilcSFTPClient sftp = (SilcSFTPClient)context;

  silc_dlist_uninit(sftp->requests);
  silc_free(sftp);
}

/* Function that is called to process the incmoing SFTP packet. */
/* XXX Some day this will go away and we have automatic receive callbacks
   for SilcSocketConnection API or SilcPacketContext API. */

void silc_sftp_client_receive_process(SilcSFTP context,
				      SilcSocketConnection sock,
				      SilcPacketContext *packet)
{
  SilcSFTPClient sftp = (SilcSFTPClient)context;
  SilcSFTPRequest req;
  SilcSFTPPacket type;
  const unsigned char *payload = NULL;
  uint32 payload_len;
  int ret;
  SilcBufferStruct buf;
  uint32 id;

  SILC_LOG_DEBUG(("Start"));

  /* Parse the packet */
  type = silc_sftp_packet_decode(packet->buffer, (unsigned char **)&payload, 
				 &payload_len);
  if (!type)
    return;

  silc_buffer_set(&buf, (unsigned char *)payload, payload_len);

  switch (type) {
  case SILC_SFTP_VERSION:
    {
      SilcSFTPVersion version;

      SILC_LOG_DEBUG(("Version packet"));

      ret = silc_buffer_unformat(&buf,
				 SILC_STR_UI_INT(&version),
				 SILC_STR_END);
      if (ret < 0) {
	(*sftp->version)((SilcSFTP)sftp, SILC_SFTP_STATUS_FAILURE, 0, 
			 sftp->version_context);
	break;
      }

      /* Call the callback */
      (*sftp->version)((SilcSFTP)sftp, SILC_SFTP_STATUS_OK, version, 
		       sftp->version_context);
    }
    break;

  case SILC_SFTP_STATUS:
    {
      uint32 status;
      char *message = NULL, *language_tag = NULL;

      SILC_LOG_DEBUG(("Status packet"));

      ret = silc_buffer_unformat(&buf, 
				 SILC_STR_UI_INT(&id),
				 SILC_STR_UI_INT(&status),
				 SILC_STR_END);
      if (ret < 0)
	break;

      if (status != SILC_SFTP_STATUS_OK) {
	silc_buffer_pull(&buf, 8);
	ret = silc_buffer_unformat(&buf,
				   SILC_STR_UI32_STRING_ALLOC(&message),
				   SILC_STR_UI32_STRING_ALLOC(&language_tag),
				   SILC_STR_END);
	if (ret < 0)
	  break;

	silc_buffer_push(&buf, 8);
      }

      /* Get request */
      req = silc_sftp_find_request(sftp, id);
      if (!req) {
	silc_free(message);
	silc_free(language_tag);
	break;
      }

      /* Call the callback */
      silc_sftp_call_request(sftp, req, type, status, message, language_tag);

      silc_free(message);
      silc_free(language_tag);
    }
    break;

  case SILC_SFTP_HANDLE:
    {
      unsigned char *handle = NULL;
      uint32 handle_len;

      SILC_LOG_DEBUG(("Handle packet"));

      ret = silc_buffer_unformat(&buf, 
				 SILC_STR_UI_INT(&id),
				 SILC_STR_UI32_NSTRING(&handle, 
						       &handle_len),
				 SILC_STR_END);
      if (ret < 0)
	break;

      /* Get request */
      req = silc_sftp_find_request(sftp, id);
      if (!req)
	break;

      /* Call the callback */
      silc_sftp_call_request(sftp, req, type, SILC_SFTP_STATUS_OK, 
			     handle, handle_len);
    }
    break;

  case SILC_SFTP_DATA:
    {
      unsigned char *data = NULL;
      uint32 data_len = 0;

      SILC_LOG_DEBUG(("Data packet"));

      ret = silc_buffer_unformat(&buf, 
				 SILC_STR_UI_INT(&id),
				 SILC_STR_UI32_NSTRING(&data, &data_len),
				 SILC_STR_END);
      if (ret < 0)
	break;

      /* Get request */
      req = silc_sftp_find_request(sftp, id);
      if (!req)
	break;

      /* Call the callback */
      silc_sftp_call_request(sftp, req, type, SILC_SFTP_STATUS_OK, 
			     data, data_len);
    }
    break;

  case SILC_SFTP_NAME:
    {
      uint32 count;
      SilcSFTPName name = NULL;

      SILC_LOG_DEBUG(("Name packet"));

      ret = silc_buffer_unformat(&buf, 
				 SILC_STR_UI_INT(&id),
				 SILC_STR_UI_INT(&count),
				 SILC_STR_END);
      if (ret < 0)
	break;

      /* Get request */
      req = silc_sftp_find_request(sftp, id);
      if (!req)
	break;

      silc_buffer_pull(&buf, 8);
      name = silc_sftp_name_decode(count, &buf);
      if (!name)
	break;
      silc_buffer_push(&buf, 8);

      /* Call the callback */
      silc_sftp_call_request(sftp, req, type, SILC_SFTP_STATUS_OK, name);
      silc_sftp_name_free(name);
    }
    break;

  case SILC_SFTP_ATTRS:
    {
      SilcSFTPAttributes attr = NULL;
      unsigned char *data;
      SilcBufferStruct tmpbuf;

      SILC_LOG_DEBUG(("Attributes packet"));

      ret = silc_buffer_unformat(&buf, 
				 SILC_STR_UI_INT(&id),
				 SILC_STR_UI_XNSTRING(&data, buf.len - 4),
				 SILC_STR_END);
      if (ret < 0)
	break;

      /* Get request */
      req = silc_sftp_find_request(sftp, id);
      if (!req)
	break;

      silc_buffer_set(&tmpbuf, data, buf.len - 4);
      attr = silc_sftp_attr_decode(&tmpbuf);
      if (!attr)
	break;

      /* Call the callback */
      silc_sftp_call_request(sftp, req, type, SILC_SFTP_STATUS_OK, attr);
    }
    break;

  case SILC_SFTP_EXTENDED_REPLY:
    {
      unsigned char *data = NULL;

      SILC_LOG_DEBUG(("Extended reply packet"));

      ret = silc_buffer_unformat(&buf, 
				 SILC_STR_UI_INT(&id),
				 SILC_STR_UI_XNSTRING(&data, buf.len - 4),
				 SILC_STR_END);
      if (ret < 0)
	break;

      /* Get request */
      req = silc_sftp_find_request(sftp, id);
      if (!req)
	break;

      /* Call the callback */
      silc_sftp_call_request(sftp, req, type, SILC_SFTP_STATUS_OK, 
			     data, buf.len - 4);
    }
    break;

  default:
    break;
  }
}

void silc_sftp_open(SilcSFTP sftp, 
		    const char *filename,
		    SilcSFTPFileOperation pflags,
		    SilcSFTPAttributes attrs,
		    SilcSFTPHandleCallback callback,
		    void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  SilcBuffer attrs_buf;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Open request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_OPEN;
  req->handle = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  attrs_buf = silc_sftp_attr_encode(attrs);
  len = 4 + 4 + strlen(filename) + 4 + attrs_buf->len;

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(filename)),
			SILC_STR_UI32_STRING(filename),
			SILC_STR_UI_INT(pflags),
			SILC_STR_UI_XNSTRING(attrs_buf->data, 
					     attrs_buf->len),
			SILC_STR_END);

  silc_buffer_free(attrs_buf);
}

void silc_sftp_close(SilcSFTP sftp,
		     SilcSFTPHandle handle,
		     SilcSFTPStatusCallback callback,
		     void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;
  const unsigned char *hdata;
  uint32 hdata_len;

  SILC_LOG_DEBUG(("Close request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_CLOSE;
  req->status = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  silc_sftp_handle_get(handle, &hdata, &hdata_len);
  len = 4 + 4 + hdata_len;

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(hdata_len),
			SILC_STR_UI_XNSTRING(hdata, hdata_len),
			SILC_STR_END);
}

void silc_sftp_read(SilcSFTP sftp,
		    SilcSFTPHandle handle,
		    uint64 offset, 
		    uint32 len,
		    SilcSFTPDataCallback callback,
		    void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len2 = 0;
  const unsigned char *hdata;
  uint32 hdata_len;

  SILC_LOG_DEBUG(("Read request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_READ;
  req->data = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  silc_sftp_handle_get(handle, &hdata, &hdata_len);
  len2 = 4 + 4 + hdata_len + 8 + 4;

  silc_sftp_send_packet(client, req->type, len2, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(hdata_len),
			SILC_STR_UI_XNSTRING(hdata, hdata_len),
			SILC_STR_UI_INT64(offset),
			SILC_STR_UI_INT(len),
			SILC_STR_END);
}

void silc_sftp_write(SilcSFTP sftp,
		     SilcSFTPHandle handle,
		     uint64 offset,
		     const unsigned char *data,
		     uint32 data_len,
		     SilcSFTPStatusCallback callback,
		     void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;
  const unsigned char *hdata;
  uint32 hdata_len;

  SILC_LOG_DEBUG(("Write request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_WRITE;
  req->status = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  silc_sftp_handle_get(handle, &hdata, &hdata_len);
  len = 4 + 4 + hdata_len + 8 + 4 + data_len;

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(hdata_len),
			SILC_STR_UI_XNSTRING(hdata, hdata_len),
			SILC_STR_UI_INT64(offset),
			SILC_STR_UI_INT(data_len),
			SILC_STR_UI_XNSTRING(data, data_len),
			SILC_STR_END);
}

void silc_sftp_remove(SilcSFTP sftp,
		      const char *filename,
		      SilcSFTPStatusCallback callback,
		      void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Remove request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_REMOVE;
  req->status = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(filename);

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(filename)),
			SILC_STR_UI32_STRING(filename),
			SILC_STR_END);
}

void silc_sftp_rename(SilcSFTP sftp,
		      const char *oldname,
		      const char *newname,
		      SilcSFTPStatusCallback callback,
		      void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Rename request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_RENAME;
  req->status = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(oldname) + 4 + strlen(newname);

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(oldname)),
			SILC_STR_UI32_STRING(oldname),
			SILC_STR_UI_INT(strlen(newname)),
			SILC_STR_UI32_STRING(newname),
			SILC_STR_END);
}

void silc_sftp_mkdir(SilcSFTP sftp,
		     const char *path,
		     SilcSFTPAttributes attrs,
		     SilcSFTPStatusCallback callback,
		     void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;
  SilcBuffer attrs_buf;

  SILC_LOG_DEBUG(("Mkdir request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_MKDIR;
  req->status = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  attrs_buf = silc_sftp_attr_encode(attrs);
  len = 4 + 4 + strlen(path) + attrs_buf->len;

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(path)),
			SILC_STR_UI32_STRING(path),
			SILC_STR_UI_XNSTRING(attrs_buf->data,
					     attrs_buf->len),
			SILC_STR_END);

  silc_buffer_free(attrs_buf);
}

void silc_sftp_rmdir(SilcSFTP sftp,
		     const char *path,
		     SilcSFTPStatusCallback callback,
		     void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Rmdir request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_RMDIR;
  req->status = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(path);

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(path)),
			SILC_STR_UI32_STRING(path),
			SILC_STR_END);
}

void silc_sftp_opendir(SilcSFTP sftp,
		       const char *path,
		       SilcSFTPHandleCallback callback,
		       void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Opendir request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_OPENDIR;
  req->handle = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(path);

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(path)),
			SILC_STR_UI32_STRING(path),
			SILC_STR_END);
}

void silc_sftp_readdir(SilcSFTP sftp,
		       SilcSFTPHandle handle,
		       SilcSFTPNameCallback callback,
		       void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;
  const unsigned char *hdata;
  uint32 hdata_len;

  SILC_LOG_DEBUG(("Readdir request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_READDIR;
  req->name = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  silc_sftp_handle_get(handle, &hdata, &hdata_len);
  len = 4 + 4 + hdata_len;

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(hdata_len),
			SILC_STR_UI_XNSTRING(hdata, hdata_len),
			SILC_STR_END);
}

void silc_sftp_stat(SilcSFTP sftp,
		    const char *path,
		    SilcSFTPAttrCallback callback,
		    void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Stat request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_STAT;
  req->attr = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(path);

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(path)),
			SILC_STR_UI32_STRING(path),
			SILC_STR_END);
}

void silc_sftp_lstat(SilcSFTP sftp,
		     const char *path,
		     SilcSFTPAttrCallback callback,
		     void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Lstat request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_LSTAT;
  req->attr = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(path);

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(path)),
			SILC_STR_UI32_STRING(path),
			SILC_STR_END);
}

void silc_sftp_fstat(SilcSFTP sftp,
		     SilcSFTPHandle handle,
		     SilcSFTPAttrCallback callback,
		     void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;
  const unsigned char *hdata;
  uint32 hdata_len;

  SILC_LOG_DEBUG(("Fstat request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_FSTAT;
  req->attr = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  silc_sftp_handle_get(handle, &hdata, &hdata_len);
  len = 4 + 4 + hdata_len;

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(hdata_len),
			SILC_STR_UI_XNSTRING(hdata, hdata_len),
			SILC_STR_END);
}

void silc_sftp_setstat(SilcSFTP sftp,
		       const char *path,
		       SilcSFTPAttributes attrs,
		       SilcSFTPStatusCallback callback,
		       void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;
  SilcBuffer attrs_buf;

  SILC_LOG_DEBUG(("Setstat request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_SETSTAT;
  req->status = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  attrs_buf = silc_sftp_attr_encode(attrs);
  len = 4 + 4 + strlen(path) + attrs_buf->len;

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(path)),
			SILC_STR_UI32_STRING(path),
			SILC_STR_UI_XNSTRING(attrs_buf->data,
					     attrs_buf->len),
			SILC_STR_END);

  silc_buffer_free(attrs_buf);
}

void silc_sftp_fsetstat(SilcSFTP sftp,
			SilcSFTPHandle handle,
			SilcSFTPAttributes attrs,
			SilcSFTPStatusCallback callback,
			void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;
  SilcBuffer attrs_buf;
  const unsigned char *hdata;
  uint32 hdata_len;

  SILC_LOG_DEBUG(("Fsetstat request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_FSETSTAT;
  req->status = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  silc_sftp_handle_get(handle, &hdata, &hdata_len);
  attrs_buf = silc_sftp_attr_encode(attrs);
  len = 4 + 4 + hdata_len + attrs_buf->len;

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(hdata_len),
			SILC_STR_UI_XNSTRING(hdata, hdata_len),
			SILC_STR_UI_XNSTRING(attrs_buf->data,
					     attrs_buf->len),
			SILC_STR_END);

  silc_buffer_free(attrs_buf);
}

void silc_sftp_readlink(SilcSFTP sftp,
			const char *path,
			SilcSFTPNameCallback callback,
			void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Readlink request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_READLINK;
  req->name = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(path);

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(path)),
			SILC_STR_UI32_STRING(path),
			SILC_STR_END);
}

void silc_sftp_symlink(SilcSFTP sftp,
		       const char *linkpath,
		       const char *targetpath,
		       SilcSFTPStatusCallback callback,
		       void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Symlink request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_SYMLINK;
  req->status = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(linkpath) + 4 + strlen(targetpath);

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(linkpath)),
			SILC_STR_UI32_STRING(linkpath),
			SILC_STR_UI_INT(strlen(targetpath)),
			SILC_STR_UI32_STRING(targetpath),
			SILC_STR_END);
}

void silc_sftp_realpath(SilcSFTP sftp,
			const char *path,
			SilcSFTPNameCallback callback,
			void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Realpath request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_REALPATH;
  req->name = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(path);

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(path)),
			SILC_STR_UI32_STRING(path),
			SILC_STR_END);
}

void silc_sftp_extended(SilcSFTP sftp,
			const char *request,
			const unsigned char *data,
			uint32 data_len,
			SilcSFTPExtendedCallback callback,
			void *context)
{
  SilcSFTPClient client = (SilcSFTPClient)sftp;
  SilcSFTPRequest req;
  uint32 len = 0;

  SILC_LOG_DEBUG(("Extended request"));

  req = silc_calloc(1, sizeof(*req));
  req->id = client->id++;
  req->type = SILC_SFTP_WRITE;
  req->extended = callback;
  req->context = context;
  silc_dlist_add(client->requests, req);

  len = 4 + 4 + strlen(request) + data_len;

  silc_sftp_send_packet(client, req->type, len, 
			SILC_STR_UI_INT(req->id),
			SILC_STR_UI_INT(strlen(request)),
			SILC_STR_UI32_STRING(request),
			SILC_STR_UI_XNSTRING(data, data_len),
			SILC_STR_END);
}