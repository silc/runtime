/*

  silcsockconn.c

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 1997 - 2001 Pekka Riikonen

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/
/* $Id$ */

#include "silcincludes.h"

/* Heartbeat context */
struct SilcSocketConnectionHB {
  uint32 heartbeat;
  SilcSocketConnectionHBCb hb_callback;
  void *hb_context;
  void *timeout_queue;
  SilcTask hb_task;
  SilcSocketConnection sock;
};

/* Allocates a new socket connection object. The allocated object is 
   returned to the new_socket argument. */

void silc_socket_alloc(int sock, SilcSocketType type, void *user_data, 
		       SilcSocketConnection *new_socket)
{
  SILC_LOG_DEBUG(("Allocating new socket connection object"));

  /* Set the pointers. Incoming and outgoing data buffers
     are allocated by the application when they are first used. */
  *new_socket = silc_calloc(1, sizeof(**new_socket));
  (*new_socket)->sock = sock;
  (*new_socket)->type = type;
  (*new_socket)->user_data = user_data;
  (*new_socket)->protocol = NULL;
  (*new_socket)->flags = 0;
  (*new_socket)->inbuf = NULL;
  (*new_socket)->outbuf = NULL;
  (*new_socket)->users++;
}

/* Free's the Socket connection object. */

void silc_socket_free(SilcSocketConnection sock)
{
  sock->users--;
  SILC_LOG_DEBUG(("Socket %p refcnt %d->%d", sock, sock->users + 1,
		  sock->users));
  if (sock->users < 1) {
    silc_buffer_free(sock->inbuf);
    silc_buffer_free(sock->outbuf);
    if (sock->hb) {
      silc_task_unregister(sock->hb->timeout_queue, sock->hb->hb_task);
      silc_free(sock->hb->hb_context);
      silc_free(sock->hb);
    }

    memset(sock, 'F', sizeof(*sock));
    silc_free(sock);
  }
}

/* Increase the reference counter. */

SilcSocketConnection silc_socket_dup(SilcSocketConnection sock)
{
  sock->users++;
  SILC_LOG_DEBUG(("Socket %p refcnt %d->%d", sock, sock->users - 1,
		  sock->users));
  return sock;
}

/* Internal timeout callback to perform heartbeat */

SILC_TASK_CALLBACK(silc_socket_heartbeat)
{
  SilcSocketConnectionHB hb = (SilcSocketConnectionHB)context;

  if (!hb->heartbeat)
    return;

  if (hb->hb_callback)
    hb->hb_callback(hb->sock, hb->hb_context);

  hb->hb_task = silc_task_register(hb->timeout_queue, hb->sock->sock, 
				   silc_socket_heartbeat,
				   context, hb->heartbeat, 0,
				   SILC_TASK_TIMEOUT,
				   SILC_TASK_PRI_LOW);
}

/* Sets the heartbeat timeout and prepares the socket for performing
   heartbeat in `heartbeat' intervals (seconds). The `hb_context' is
   allocated by the application and will be sent as argument to the
   `hb_callback' function that is called when the `heartbeat' timeout
   expires.  The callback `hb_context' won't be touched by the library
   but will be freed automatically when calling silc_socket_free.  The
   `timeout_queue' is the application's scheduler timeout queue. */

void silc_socket_set_heartbeat(SilcSocketConnection sock, 
			       uint32 heartbeat,
			       void *hb_context,
			       SilcSocketConnectionHBCb hb_callback,
			       void *timeout_queue)
{

  if (sock->hb) {
    silc_task_unregister(sock->hb->timeout_queue, sock->hb->hb_task);
    silc_free(sock->hb->hb_context);
    silc_free(sock->hb);
  }

  sock->hb = silc_calloc(1, sizeof(*sock->hb));
  sock->hb->heartbeat = heartbeat;
  sock->hb->hb_context = hb_context;
  sock->hb->hb_callback = hb_callback;
  sock->hb->timeout_queue = timeout_queue;
  sock->hb->sock = sock;
  sock->hb->hb_task = silc_task_register(timeout_queue, sock->sock,
                                         silc_socket_heartbeat,
                                         (void *)sock->hb, heartbeat, 0,
                                         SILC_TASK_TIMEOUT,
                                         SILC_TASK_PRI_LOW);
}