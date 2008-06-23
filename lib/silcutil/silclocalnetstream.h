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
 * with each other in the local machine using the local network.  The
 * interface provides a form of interprocess communication (IPC) using network
 * sockets (TCP).
 *
 * Since the implementation uses real TCP network socket the listener can be
 * used for any TCP communication, however connections may be estalished only
 * from the local machine.  The connections use the loopback network.
 *
 * EXAMPLE
 *
 * // Create listener
 * listener = silc_local_net_create_listener("/tmp/conn1", 0, schedule,
 *                                           accept_callback, ctx);
 *
 * // Connect to the listener
 * silc_local_net_connect("/tmp/conn1", schedule, connected_callback, ctx);
 *
 * // Close listener
 * silc_local_net_close_listener(listener);
 *
 ***/

#ifndef SILCLOCALNETSTREAM_H
#define SILCLOCALNETSTREAM_H

/****d* silcutil/SilcLocalNetSecurity
 *
 * NAME
 *
 *    typedef enum { ... } SilcLocalNetSecurity
 *
 * DESCRIPTION
 *
 *    The security flags for the local network listener.  They specify
 *    how the listener can be accessed.  The flags are a bitmasks and can
 *    be combined.  Note that, these flags apply only when this API is
 *    used.  Anyone in local machine is able to see the network listener
 *    port by checking all bound network listeners and thus are able to
 *    connect to it.
 *
 * SOURCE
 */
typedef enum {
  SILC_LOCAL_NET_ALL    = 0x0000,     /* Anyone in local machine can connect */
  SILC_LOCAL_NET_USER   = 0x0001,     /* Same user can connect */
  SILC_LOCAL_NET_GROUP  = 0x0002,     /* Same group can connect */
} SilcLocalNetSecurity;
/***/

/****f* silcutil/silc_local_net_create_listener
 *
 * SYNOPSIS
 *
 *    SilcNetListener
 *    silc_local_net_create_listener(const char *filepath,
 *                                   SilcLocalNetSecurity security,
 *                                   SilcSchedule schedule,
 *                                   SilcNetCallback callback,
 *                                   void *context);
 *
 * DESCRIPTION
 *
 *    Creates a local network stream listener and returns a network server.
 *    The `filepath' is a local filepath that must be used by the clients to
 *    connect to the server.  The `security' specify the access method to
 *    the listener.  It can specify for example that only the user creating
 *    the listener is able to connect to it.
 *
 *    The `callback' will be called when a client connects to the listener
 *    with the `context'.  The returned stream to the `callback' is a
 *    socket stream (silcsocketstream.h).  The returned listener must be
 *    closed by calling silc_local_net_close_listener.
 *
 *    Clients can connect to the listener by calling the
 *    silc_local_net_connect.
 *
 *    Returns NULL on error and set silc_errno.  If `schedule' is NULL this
 *    will call silc_schedule_get_global to try to get global scheduler.
 *
 ***/
SilcNetListener silc_local_net_create_listener(const char *filepath,
					       SilcLocalNetSecurity security,
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
 *    been created.  The stream returned to `callback' is a socket stream
 *    (silcsocketstream.h).  SilcStream API can be used with the returned
 *    stream.  The stream must be destroyed by calling silc_stream_destroy.
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
