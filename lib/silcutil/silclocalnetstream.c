/*

  silclocalnetstream.c

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

/* Local net listener context */
typedef struct {
  SilcNetListener listener;
  char *filepath;
  SilcNetCallback callback;
  void *context;
} *SilcLocalNetListener;

/************************ Static utility functions **************************/

/* Connection accept callback */

static void silc_local_net_accept(SilcResult result, SilcStream stream,
				  void *context)
{
  SilcLocalNetListener listener = context;
  const char *remote_ip;

  if (!silc_socket_stream_get_info(stream, NULL, NULL, &remote_ip, NULL)) {
    silc_stream_destroy(stream);
    return;
  }

  /* Make sure the connection comes from local host */
  if (strcmp(remote_ip, "127.0.0.1")) {
    SILC_LOG_DEBUG(("Connection is coming from %s, will not accept",
		    remote_ip));
    silc_stream_destroy(stream);
    return;
  }

  listener->callback(result, stream, listener->context);
}

/****************************** Public API **********************************/

/* Create listener */

SilcNetListener silc_local_net_create_listener(const char *filepath,
					       SilcSchedule schedule,
					       SilcNetCallback callback,
					       void *context)
{
  SilcLocalNetListener listener;
  SilcUInt16 *local_port;
  const char *addr = "127.0.0.1";
  char port[8];

  SILC_LOG_DEBUG(("Creating local network stream listener %s", filepath));

  if (!filepath || !callback) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return NULL;
  }

  /* Make sure file doesn't exist */
  if (silc_file_stat(filepath, FALSE, NULL)) {
    silc_set_errno(SILC_ERR_ALREADY_EXISTS);
    return NULL;
  }

  listener = silc_calloc(1, sizeof(*listener));
  if (!listener)
    return NULL;
  listener->callback = callback;
  listener->context = context;

  listener->filepath = silc_strdup(filepath);
  if (!listener->filepath) {
    silc_free(listener);
    return NULL;
  }

  /* Create local TCP listener */
  listener->listener =
    silc_net_tcp_create_listener(&addr, 1, 0, TRUE, FALSE, schedule,
				 silc_local_net_accept, listener);
  if (!listener) {
    silc_free(listener);
    return NULL;
  }

  /* Get the bound port */
  local_port = silc_net_listener_get_port(listener->listener, NULL);
  if (!local_port) {
    silc_net_close_listener(listener->listener);
    silc_free(listener);
    return NULL;
  }

  /* Create the file */
  silc_snprintf(port, sizeof(port), "%d", *local_port);
  if (silc_file_writefile(filepath, port, strlen(port) + 1)) {
    silc_free(local_port);
    silc_net_close_listener(listener->listener);
    silc_free(listener);
    return NULL;
  }

  SILC_LOG_DEBUG(("Created local network stream listener %p", listener));

  silc_free(local_port);

  return (SilcNetListener)listener;
}

/* Close listener */

void silc_local_net_close_listener(SilcNetListener local_listener)
{
  SilcLocalNetListener listener = (SilcLocalNetListener)local_listener;

  SILC_LOG_DEBUG(("Closing local network stream listener %p, %s",
		  listener, listener->filepath));

  unlink(listener->filepath);
  silc_net_close_listener(listener->listener);
  silc_free(listener);
}

/* Connect to the local network listener */

SilcAsyncOperation silc_local_net_connect(const char *filepath,
					  SilcSchedule schedule,
					  SilcNetCallback callback,
					  void *context)
{
  unsigned char *port_data;
  int port;

  SILC_LOG_DEBUG(("Connecting to local network listner %s", filepath));

  if (!filepath) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    if (callback)
      callback(silc_errno, NULL, context);
    return NULL;
  }

  /* Read the port from file */
  port_data = silc_file_readfile(filepath, NULL, NULL);
  if (!port_data) {
    if (callback)
      callback(silc_errno, NULL, context);
    return NULL;
  }
  port = atoi(port_data);

  silc_free(port_data);

  /* Connect */
  return silc_net_tcp_connect("127.0.0.1", "127.0.0.1", port, schedule,
			      callback, context);
}
