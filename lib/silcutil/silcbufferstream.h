/*

  silcbufferstream.h

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

/****h* silcutil/Buffer Stream Interface
 *
 * DESCRIPTION
 *
 * Buffer stream interface to send and receive buffers.  The benefit of this
 * interface is that the receiver need not parse buffers from the received
 * data but each buffer sent is delivered separately to the receiver
 * callback, even if multiple buffers were received at the same time.  The
 * length of the buffer is delivered with the data.  The buffer data follows
 * a 32-bit length field in the stream.
 *
 * This interface is named SILC Buffer Stream API instead of simply SILC
 * Packet API which would be more desriptive name but that API name is already
 * used by another SILC distribution.
 *
 * Buffer stream is not thread-safe.  If the same buffer stream must be
 * used in multithreaded environment concurrency control must be employed.
 *
 ***/

#ifndef SILCBUFFERSTREAM_H
#define SILCBUFFERSTREAM_H

/****f* silcutil/SilcBufferReceiveCallback
 *
 * SYNOPSIS
 *
 *    typedef void (*SilcBufferReceiveCallback)(SilcResult status,
 *                                              SilcStream stream,
 *                                              SilcBuffer buffer,
 *                                              void *context);
 *
 * DESCRIPTION
 *
 *    Callback function to deliver the received `buffer' from the `stream'.
 *    The `buffer' is the buffer that was sent to the stream.  If more than
 *    one buffers were sent each is delivered separately to this callback.
 *    The `status' will indicate an error if such occurred in the stream.
 *    The `buffer' is NULL in case of error.  The receiver must free
 *    the `buffer'.
 *
 ***/
typedef void (*SilcBufferReceiveCallback)(SilcResult status,
					  SilcStream stream,
					  SilcBuffer buffer,
					  void *context);

/****f* silcutil/silc_buffer_stream_create
 *
 * SYNOPSIS
 *
 *    SilcStream silc_buffer_stream_create(SilcStream stream,
 *                                         SilcBufferReceiveCallback receiver,
 *                                         void *context);
 *
 * DESCRIPTION
 *
 *    Creates a buffer stream and returns it.  The `stream' is the underlaying
 *    stream to be used to actually send the buffer and receive buffers.
 *    The returned stream is used with this API to send the buffers.  The
 *    `stream' must stay valid as long the buffer stream is used.
 *
 *    To send buffers to the stream silc_buffer_stream_send can be used.
 *    The silc_stream_write cannot be used with the returned stream.  Buffers
 *    coming from the `stream' will be delivered to the `receiver' callback.
 *    The returned stream and `context' will also be delivered to `receiver'.
 *
 *    The returned stream must be destroyed by calling silc_stream_destroy.
 *    Other SilcStream API functions cannot be used with buffer stream.
 *
 ***/
SilcStream silc_buffer_stream_create(SilcStream stream,
				     SilcBufferReceiveCallback receiver,
				     void *context);

/****f* silcutil/silc_buffer_stream_send
 *
 * SYNOPSIS
 *
 *    SilcBool silc_buffer_stream_send(SilcStream stream,
 *                                     SilcBuffer buffer);
 *
 * DESCRIPTION
 *
 *    Sends `buffer' to the buffer stream indicated by `stream'.  If the
 *    `stream' is not a buffer stream created by silc_buffer_stream_create
 *    this will return FALSE.  Returns FALSE on error and sets silc_errno.
 *
 ***/
SilcBool silc_buffer_stream_send(SilcStream stream,
				 SilcBuffer buffer);

#endif /* SILCBUFFERSTREAM_H */
