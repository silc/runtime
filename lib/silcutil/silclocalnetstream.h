/*

  silclocalnetstream.h

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

/****h* silcutil/Local Network Stream Interface
 *
 * DESCRIPTION
 *
 * Local network stream interface enables two or more processes to communicate
 * with each other in the local machine using local network start.  The
 * interface provides a form of interprocess communication (IPC) using network
 * sockets.
 *
 ***/

#ifndef SILCLOCALNETSTREAM_H
#define SILCLOCALNETSTREAM_H

/****f* silcutil/silc_local_net_create_listener
 *
 * SYNOPSIS
 *
 *    SilcNetListener silc_local_net_create_listener(const char *filepath,
 *                                                   SilcSchedule schedule,
 *                                                   SilcNetCallback callback,
 *                                                   void *context);
 *
 * DESCRIPTION
 *
 *    Creates a local network stream listener and returns a network server.
 *    The `filepath' is a local filepath that must be used by the clients to
 *    connect to the server.
 *
 *    The `callback' will be called when a client connects to the listener
 *    with the `context'.  The returned listener must be closed by calling
 *    silc_local_net_close_listener.
 *
 *    Clients can connect to the listener by calling the
 *    silc_local_net_connect.
 *
 *    Returns NULL on error and set silc_errno.  If `schedule' is NULL this
 *    will call silc_schedule_get_global to try to get global scheduler.
 *
 ***/
SilcNetListener silc_local_net_create_listener(const char *filepath,
					       SilcSchedule schedule,
					       SilcNetCallback callback,
					       void *context);

/****f* silcutil/silc_local_net_close_listener
 *
 * SYNOPSIS
 *
 *    void silc_local_net_close_listener(SilcNetListener local_listener);
 *
 * DESCRIPTION
 *
 *    Closes the local network stream listener.
 *
 ***/
void silc_local_net_close_listener(SilcNetListener local_listener);

/****f* silcutil/silc_local_net_connect
 *
 * SYNOPSIS
 *
 *    SilcAsyncOperation silc_local_net_connect(const char *filepath,
 *                                              SilcSchedule schedule,
 *                                              SilcNetCallback callback,
 *                                              void *context);
 *
 * DESCRIPTION
 *
 *    Connects to the local network server at the provided `filepath'.  If
 *    the `filepath' does not exist or is not valid local network listener
 *    the connection will fail.
 *
 *    The `callback' with `context' will be called once the connection has
 *    been created.
 *
 *    If `schedule' is NULL this will call silc_schedule_get_global to try
 *    to get global scheduler.
 *
 ***/
SilcAsyncOperation silc_local_net_connect(const char *filepath,
					  SilcSchedule schedule,
					  SilcNetCallback callback,
					  void *context);

#endif /* SILCLOCALNETSTREAM_H */
