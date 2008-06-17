/*

  silcbufferstream.c

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 2008 Pekka Riikonen

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/

#include <silcruntime.h>

/************************** Types and definitions ***************************/

#define SILC_IS_BUFFER_STREAM(s) (s && s->ops == &silc_buffer_stream_ops)

const SilcStreamOps silc_buffer_stream_ops;

/* Buffer stream context */
typedef struct {
  const SilcStreamOps *ops;
  SilcStream stream;
  SilcBuffer outbuf;
  SilcBuffer inbuf;
  SilcBufferStruct queue;
  SilcBufferReceiveCallback receiver;
  void *context;
  unsigned int closed   : 1;
} *SilcBufferStream;

/************************ Static utility functions **************************/

/* IO callback */

static void silc_buffer_stream_io(SilcStream stream,
				  SilcStreamStatus status,
				  void *context)
{
  SilcBufferStream bs = context;
  SilcBuffer buffer = NULL;
  SilcUInt32 buf_len;
  int ret, len;

  if (bs->closed)
    return;

  if (status == SILC_STREAM_CAN_READ) {
    /* Read data */
    SILC_LOG_DEBUG(("Read data from buffer stream %p", bs));

    while ((ret = silc_stream_read(bs->stream, bs->inbuf->tail,
				   silc_buffer_taillen(bs->inbuf))) > 0) {
      if (!buffer) {
	buffer = silc_buffer_alloc(0);
	if (!buffer)
	  return;
      }

      silc_buffer_pull_tail(bs->inbuf, ret);

      /* Parse the buffer */
      while ((len = silc_buffer_unformat(bs->inbuf,
					 SILC_STR_BUFFER_ALLOC(buffer),
					 SILC_STR_END)) > 0) {
	/* Deliver the buffer */
	SILC_LOG_HEXDUMP(("Received buffer, size %d",
			  silc_buffer_len(buffer)),
			 silc_buffer_data(buffer), silc_buffer_len(buffer));
	bs->receiver(SILC_OK, (SilcStream)bs, buffer, bs->context);
	silc_buffer_pull(bs->inbuf, len);

	buffer = silc_buffer_alloc(0);
	if (!buffer)
	  return;
      }

      if (silc_buffer_len(bs->inbuf) > 0) {
	/* Not complete buffer, read more data */
	buf_len = 4;
	if (silc_buffer_len(bs->inbuf) >= 4) {
	  SILC_GET32_MSB(buf_len, bs->inbuf->data);
	  SILC_LOG_DEBUG(("Incomplete buffer, wait for rest, buffer size %d",
			  buf_len));
	}

	/* Enlarge inbuf if needed */
	if (silc_buffer_taillen(bs->inbuf) < buf_len)
	  silc_buffer_realloc(bs->inbuf, silc_buffer_truelen(bs->inbuf) +
			      buf_len);
	continue;
      }

      /* All data read, read more */
      silc_buffer_reset(bs->inbuf);
    }

    silc_buffer_free(buffer);

    if (ret == 0 || ret == -2) {
      bs->receiver(silc_errno, (SilcStream)bs, NULL, bs->context);
      return;
    }
  } else {
    /* Write any pending data */
    SILC_LOG_DEBUG(("Write pending data to buffer stream %p", bs));

    while (silc_buffer_len(&bs->queue) > 0) {
      ret = silc_stream_write(bs->stream, silc_buffer_data(&bs->queue),
			      silc_buffer_len(&bs->queue));
      if (silc_unlikely(ret == 0))
	return;

      if (silc_unlikely(ret == -2))
	return;

      if (silc_unlikely(ret == -1)) {
	SILC_LOG_DEBUG(("Buffer stream %p would block, send later", bs));
	return;
      }

      /* Wrote data */
      silc_buffer_pull(&bs->queue, ret);
    }

    memset(&bs->queue, 0, sizeof(bs->queue));
    silc_buffer_reset(bs->outbuf);
  }
}

/****************************** Public API **********************************/

/* Create buffer stream */

SilcStream silc_buffer_stream_create(SilcStream stream,
				     SilcBufferReceiveCallback receiver,
				     void *context)
{
  SilcBufferStream bs;

  if (!stream || !receiver) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return NULL;
  }

  bs = silc_calloc(1, sizeof(*bs));
  if (!bs)
    return NULL;

  SILC_LOG_DEBUG(("Created new buffer stream %p", bs));

  bs->ops = &silc_buffer_stream_ops;
  bs->stream = stream;
  bs->receiver = receiver;
  bs->context = context;
  bs->inbuf = silc_buffer_alloc(32);
  bs->outbuf = silc_buffer_alloc(0);
  if (!bs->inbuf || !bs->outbuf) {
    silc_buffer_free(bs->inbuf);
    silc_buffer_free(bs->outbuf);
    silc_free(bs);
    return NULL;
  }

  /* Set IO callback to the underlaying stream */
  silc_stream_set_notifier(bs->stream,
			   silc_stream_get_schedule(bs->stream),
			   silc_buffer_stream_io, bs);

  return (SilcStream)bs;
}

/* Send buffer to stream */

SilcBool silc_buffer_stream_send(SilcStream stream,
				 SilcBuffer buffer)
{
  SilcBufferStream bs = stream;
  int ret;

  SILC_LOG_HEXDUMP(("Send to buffer stream %p %d bytes", bs,
		    silc_buffer_len(buffer)),
		   silc_buffer_data(buffer), silc_buffer_len(buffer));

  if (silc_unlikely(!SILC_IS_BUFFER_STREAM(bs))) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return FALSE;
  }

  if (silc_unlikely(!buffer)) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return FALSE;
  }

  if (silc_unlikely(bs->closed)) {
    SILC_LOG_DEBUG(("Buffer stream %p is closed", bs));
    silc_set_errno(SILC_ERR_NOT_VALID);
    return FALSE;
  }

  /* Put to queue */
  if (silc_buffer_format(bs->outbuf,
			 SILC_STR_ADVANCE,
			 SILC_STR_BUFFER(buffer),
			 SILC_STR_END) < 0)
    return FALSE;

  ret = silc_buffer_headlen(&bs->queue);
  bs->queue.head = bs->outbuf->head;
  bs->queue.data = bs->queue.head + ret;
  bs->queue.tail = bs->outbuf->data;
  bs->queue.end  = bs->outbuf->end;

  /* Write the queue buffer */
  while (silc_buffer_len(&bs->queue) > 0) {
    ret = silc_stream_write(bs->stream, silc_buffer_data(&bs->queue),
			    silc_buffer_len(&bs->queue));
    if (silc_unlikely(ret == 0))
      return FALSE;

    if (silc_unlikely(ret == -2))
      return FALSE;

    if (silc_unlikely(ret == -1)) {
      SILC_LOG_DEBUG(("Buffer stream %p would block, send later", bs));
      return TRUE;
    }

    /* Wrote data */
    silc_buffer_pull(&bs->queue, ret);
  }

  memset(&bs->queue, 0, sizeof(bs->queue));
  silc_buffer_reset(bs->outbuf);

  SILC_LOG_DEBUG(("Buffer sent to buffer stream %p", bs));

  return TRUE;
}

/******************************* Stream API *********************************/

int silc_buffer_stream_read(SilcStream stream, unsigned char *buf,
			    SilcUInt32 buf_len)
{
  SILC_LOG_ERROR(("The silc_stream_read cannot be used with buffer streams"));
  return -2;
}

int silc_buffer_stream_write(SilcStream stream, const unsigned char *data,
			     SilcUInt32 data_len)
{
  SILC_LOG_ERROR(("Use silc_buffer_stream_send with buffer streams"));
  return -2;
}

SilcBool silc_buffer_stream_close(SilcStream stream)
{
  SilcBufferStream bs = stream;

  SILC_LOG_DEBUG(("Closing buffer stream %p", bs));

  bs->closed = TRUE;
  silc_stream_set_notifier(bs->stream,
			   silc_stream_get_schedule(bs->stream), NULL, NULL);

  return TRUE;
}

void silc_buffer_stream_destroy(SilcStream stream)
{
  SilcBufferStream bs = stream;

  SILC_LOG_DEBUG(("Destroying buffer stream %p", bs));

  silc_buffer_stream_close(stream);
  silc_buffer_free(bs->outbuf);
  silc_buffer_free(bs->inbuf);
  silc_free(bs);
}

SilcBool silc_buffer_stream_notifier(SilcStream stream,
				     SilcSchedule schedule,
				     SilcStreamNotifier callback,
				     void *context)
{
  SILC_LOG_ERROR(("The silc_stream_set_notifier cannot be used with "
		  "buffer streams"));
  return FALSE;
}

SilcSchedule silc_buffer_stream_get_schedule(SilcStream stream)
{
  SilcBufferStream bs = stream;
  return silc_stream_get_schedule(bs->stream);
}

/* Buffer stream operations */
const SilcStreamOps silc_buffer_stream_ops =
{
  silc_buffer_stream_read,
  silc_buffer_stream_write,
  silc_buffer_stream_close,
  silc_buffer_stream_destroy,
  silc_buffer_stream_notifier,
  silc_buffer_stream_get_schedule
};
