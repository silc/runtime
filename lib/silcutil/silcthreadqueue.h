/*

  silcthreadqueue.h

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

/****h* silcutil/Thread Queue Interface
 *
 * DESCRIPTION
 *
 * This interface provides asynchronous thread queues that can be used to
 * pass messages and data between two or more threads.  Typically a thread
 * would create the queue, push data into the queue and some other thread
 * takes the data from the queue or blocks until more data is available
 * in the queue.
 *
 * The queue itself can have one ore more pipes, allowing user to use one
 * queue to pass different information in different pipes, if each pipe
 * need to be dedicated to specific type of data.
 *
 * EXAMPLE
 *
 * Thread 1:
 *
 * // Create queue and push data into it
 * SilcThreadQueue queue = silc_thread_queue_alloc(1, FALSE);
 * silc_thread_queue_push(queue, 0, data, FALSE);
 *
 * Thread 2:
 *
 * // Connect to the queue
 * silc_thread_queue_connect(queue);
 *
 * // Block here until data is available from the queue
 * data = silc_thread_queue_pop(queue, 0, TRUE);
 *
 ***/

#ifndef SILCTHREADQUEUE_H
#define SILCTHREADQUEUE_H

/****s* silcutil/SilcThreadQueue
 *
 * NAME
 *
 *    typedef struct SilcThreadQueueStruct *SilcThreadQueue;
 *
 * DESCRIPTION
 *
 *    The thread queue context allocated by silc_thread_queue_alloc and
 *    given as argument to all silc_thread_queue_* functions.
 *
 ***/
typedef struct SilcThreadQueueStruct *SilcThreadQueue;

/****f* silcutil/silc_thread_queue_alloc
 *
 * SYNOPSIS
 *
 *    SilcThreadQueue silc_thread_queue_alloc(int num_pipes, SilcBool fifo);
 *
 * DESCRIPTION
 *
 *    Allocates new thread queue context and returns it.  Returns NULL in
 *    case of error and sets the silc_errno.  The returned context is
 *    immediately ready to be used.  For a thread to be able to use the
 *    queue it must first connect to it by calling silc_thread_queue_connect.
 *    The thread that creates the queue automatically connects to the queue.
 *
 *    The 'num_pipes' specifies the number of pipes that exist in the queue.
 *    If `num_pipes' is 0, the 0 is ignored and one pipe is created anyway.
 *    By default, caller should create one pipe, unless more are needed.
 *    The pipes are referenced by index.  First pipe has index 0, second
 *    index 1, and so on.  The index is given as argument when pushing
 *    and popping from the queue.
 *
 *    By default data popped from the queue is done in last-in-first-out
 *    order; the most recently added data is popped first.  If `fifo' is
 *    set to TRUE the order is first-in-first-out; the first added data is
 *    popped first.
 *
 ***/
SilcThreadQueue silc_thread_queue_alloc(int num_pipes, SilcBool fifo);

/****f* silcutil/silc_thread_queue_connect
 *
 * SYNOPSIS
 *
 *    SilcBool silc_thread_queue_connect(SilcThreadQueue queue);
 *
 * DESCRIPTION
 *
 *    Connects current thread to the thread queue.  This function must
 *    be called by each thread wanting to use the thread queue.  After the
 *    thread is finished using the queue it must disconnect from the queue
 *    by calling silc_thread_queue_disconnect.
 *
 ***/
void silc_thread_queue_connect(SilcThreadQueue queue);

/****f* silcutil/silc_thread_queue_disconnect
 *
 * SYNOPSIS
 *
 *    SilcBool silc_thread_queue_disconnect(SilcThreadQueue queue);
 *
 * DESCRIPTION
 *
 *    Disconnects the current thread from the thread queue.  This must be
 *    called after the thread has finished using the thread queue.
 *
 *    When the last thread has disconnected from the queue the queue is
 *    destroyed and this returns FALSE.  Otherwise this returns TRUE as
 *    long as there are threads connected to the queue.
 *
 ***/
SilcBool silc_thread_queue_disconnect(SilcThreadQueue queue);

/****f* silcutil/silc_thread_queue_push
 *
 * SYNOPSIS
 *
 *    void silc_thread_queue_push(SilcThreadQueue queue, int pipe_index,
 *                                void *data, SilcBool demux);
 *
 * DESCRIPTION
 *
 *    Pushes the `data' into the thread queue.  The data will become
 *    immediately available in the queue for other threads.  The `pipe_index'
 *    specifies the pipe to push the data into.  First pipe has index 0,
 *    second has index 1, and so on.  If there is only one pipe the index
 *    is always 0.
 *
 *    If the `demux' is TRUE this will perform demuxing; data pushed to one
 *    pipe will be pushed to all pipes.  In this case the `pipe_index' is
 *    ignored.  Each pipe will return the same data when popped.
 *
 ***/
void silc_thread_queue_push(SilcThreadQueue queue, int pipe_index, void *data,
			    SilcBool demux);

/****f* silcutil/silc_thread_queue_pop
 *
 * SYNOPSIS
 *
 *    void *silc_thread_queue_pop(SilcThreadQueue queue, int pipe_index,
 *                                SilcBool block);
 *
 * DESCRIPTION
 *
 *    Takes data from the queue and returns it.  If `block' is TRUE and
 *    data is not available this will block until data becomes available.
 *    If `block' is FALSE and data is not available this will return NULL.
 *    If `block' is TRUE this will never return NULL.
 *
 *    The `pipe_index' specifies the pipe from which to pop the data.
 *    First pipe has index 0, second has index 1, and so on.  If there is
 *    only one pipe the index is always 0.
 *
 ***/
void *silc_thread_queue_pop(SilcThreadQueue queue, int pipe_index,
			    SilcBool block);

/****f* silcutil/silc_thread_queue_timed_pop
 *
 * SYNOPSIS
 *
 *    void *silc_thread_queue_timed_pop(SilcThreadQueue queue,
 *                                      int pipe_index, int timeout_msec);
 *
 * DESCRIPTION
 *
 *    Takes data from the thread queue or waits at most `timeout_msec'
 *    milliseconds for the data to arrive.  If data is not available when
 *    the timeout occurrs this returns NULL.
 *
 *    The `pipe_index' specifies the pipe from which to pop the data.
 *    First pipe has index 0, second has index 1, and so on.  If there is
 *    only one pipe the index is always 0.
 *
 ***/
void *silc_thread_queue_timed_pop(SilcThreadQueue queue, int pipe_index,
				  int timeout_msec);

/****f* silcutil/silc_thread_queue_pop_list
 *
 * SYNOPSIS
 *
 *    SilcDList silc_thread_queue_pop_list(SilcThreadQueue queue,
 *                                         int pipe_index, SilcBool block);
 *
 * DESCRIPTION
 *
 *    Takes everything from the queue and returns the data in a list.  The
 *    caller must free the returned list with silc_dlist_uninit.  If the
 *    `block' is FALSE this will never block but will return the queue
 *    immediately.  If `block' is TRUE this will block if the queue is
 *    empty.
 *
 *    The `pipe_index' specifies the pipe from which to pop the list.
 *    First pipe has index 0, second has index 1, and so on.  If there is
 *    only one pipe the index is always 0.
 *
 ***/
SilcDList silc_thread_queue_pop_list(SilcThreadQueue queue, int pipe_index,
				     SilcBool block);

#endif /* SILCTHREADQUEUE_H */
