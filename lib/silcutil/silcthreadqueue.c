/*

  silcthreadqueue.c

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

#include "silcruntime.h"

/************************** Types and definitions ***************************/

/* Queue data context */
typedef struct SilcThreadQueueDataStruct {
  struct SilcThreadQueueDataStruct *next;
  void *data;			/* User data */
} *SilcThreadQueueData;

/* Pipe */
typedef struct SilcThreadQueuePipeStruct {
  SilcList queue;		/* The queue */
  SilcList freelist;		/* Free list of queue data contexts */
  SilcMutex lock;		/* Queue lock */
  SilcCond cond;		/* Condition for waiting */
} *SilcThreadQueuePipe;

/* Thread queue context */
struct SilcThreadQueueStruct {
  SilcThreadQueuePipe pipes;	/* Queue pipes */
  SilcAtomic32 connected;	/* Number of connected threads */
  unsigned int num_pipes  : 31;	/* Number of pipes */
  unsigned int fifo       : 1;	/* FIFO */
};

/************************** SILC Thread Queue API ***************************/

/* Allocate thread queue */

SilcThreadQueue silc_thread_queue_alloc(int num_pipes, SilcBool fifo)
{
  SilcThreadQueue queue;
  SilcUInt32 i;

  if (!num_pipes)
    num_pipes = 1;

  queue = silc_calloc(1, sizeof(*queue));
  if (!queue)
    return NULL;

  SILC_LOG_DEBUG(("Allocated thread queue %p, %d pipes %s", queue,
		  num_pipes, fifo ? "FIFO" : ""));

  queue->pipes = silc_calloc(num_pipes, sizeof(*queue->pipes));
  if (!queue->pipes) {
    silc_free(queue);
    return NULL;
  }
  queue->num_pipes = num_pipes;
  queue->fifo = fifo;

  for (i = 0; i < num_pipes; i++) {
    silc_list_init(queue->pipes[i].queue,
		   struct SilcThreadQueueDataStruct, next);
    silc_list_init(queue->pipes[i].freelist,
		   struct SilcThreadQueueDataStruct, next);
    silc_mutex_alloc(&queue->pipes[i].lock);
    silc_cond_alloc(&queue->pipes[i].cond);
  }

  silc_atomic_init32(&queue->connected, 1);

  return queue;
}

/* Connect current thread to queue */

void silc_thread_queue_connect(SilcThreadQueue queue)
{
  SILC_LOG_DEBUG(("Connect to thread queue %p", queue));
  silc_atomic_add_int32(&queue->connected, 1);
}

/* Disconnect current thread from queue */

SilcBool silc_thread_queue_disconnect(SilcThreadQueue queue)
{
  SilcUInt32 i;
  SilcThreadQueueData data;

  SILC_LOG_DEBUG(("Disconnect from thread queue %p", queue));

  if (silc_atomic_sub_int32(&queue->connected, 1) > 0)
    return TRUE;

  /* Free queue */
  SILC_LOG_DEBUG(("Free thread queue %p", queue));

  for (i = 0; i < queue->num_pipes; i++) {
    silc_cond_free(queue->pipes[i].cond);
    silc_mutex_free(queue->pipes[i].lock);
    silc_list_start(queue->pipes[i].queue);
    while ((data = silc_list_get(queue->pipes[i].queue)))
      silc_free(data);
    silc_list_start(queue->pipes[i].freelist);
    while ((data = silc_list_get(queue->pipes[i].freelist)))
      silc_free(data);
  }

  silc_free(queue->pipes);
  silc_atomic_uninit32(&queue->connected);
  silc_free(queue);

  return FALSE;
}

/* Push data to queue */

void silc_thread_queue_push(SilcThreadQueue queue, int pipe_index, void *data,
			    SilcBool demux)
{
  SilcThreadQueueData d;
  SilcUInt32 i;

  if (silc_unlikely(!data))
    return;

  SILC_ASSERT(pipe_index < queue->num_pipes);

  SILC_LOG_DEBUG(("Push data %p to thread queue %p, pipe %d, demux %s",
		  data, queue, pipe_index, demux ? "yes" : "no"));

  silc_mutex_lock(queue->pipes[pipe_index].lock);

  d = silc_list_pop(queue->pipes[pipe_index].freelist);
  if (!d) {
    d = silc_calloc(1, sizeof(*d));
    if (!d)
      return;
  }
  d->data = data;

  if (demux) {
    for (i = 0; i < queue->num_pipes; i++) {
      if (queue->fifo)
	silc_list_add(queue->pipes[i].queue, d);
      else
	silc_list_insert(queue->pipes[i].queue, NULL, d);
    }
  } else {
    if (queue->fifo)
      silc_list_add(queue->pipes[pipe_index].queue, d);
    else
      silc_list_insert(queue->pipes[pipe_index].queue, NULL, d);
  }

  silc_cond_broadcast(queue->pipes[pipe_index].cond);
  silc_mutex_unlock(queue->pipes[pipe_index].lock);
}

/* Get data or wait if wanted or return NULL. */

void *silc_thread_queue_pop(SilcThreadQueue queue, int pipe_index,
			    SilcBool block)
{
  SilcThreadQueueData d;
  void *data;

  SILC_ASSERT(pipe_index < queue->num_pipes);

  silc_mutex_lock(queue->pipes[pipe_index].lock);

  if (block) {
    /* Block */
    while ((d = silc_list_pop(queue->pipes[pipe_index].queue)) == NULL)
      silc_cond_wait(queue->pipes[pipe_index].cond,
		     queue->pipes[pipe_index].lock);
    silc_list_add(queue->pipes[pipe_index].freelist, d);
    data = d->data;
  } else {
    /* No blocking */
    d = silc_list_pop(queue->pipes[pipe_index].queue);
    data = NULL;
    if (d) {
      silc_list_add(queue->pipes[pipe_index].freelist, d);
      data = d->data;
    }
  }

  SILC_LOG_DEBUG(("Pop data %p from thread queue %p, pipe %d", data,
		  queue, pipe_index));

  silc_mutex_unlock(queue->pipes[pipe_index].lock);

  return data;
}

/* Get data or wait for a while */

void *silc_thread_queue_timed_pop(SilcThreadQueue queue, int pipe_index,
				  int timeout_msec)
{
  SilcThreadQueueData d;
  void *data = NULL;

  SILC_ASSERT(pipe_index < queue->num_pipes);

  silc_mutex_lock(queue->pipes[pipe_index].lock);

  while ((d = silc_list_pop(queue->pipes[pipe_index].queue)) == NULL)
    if (!silc_cond_timedwait(queue->pipes[pipe_index].cond,
			     queue->pipes[pipe_index].lock, timeout_msec))
      break;

  if (d) {
    silc_list_add(queue->pipes[pipe_index].freelist, d);
    data = d->data;
  }

  SILC_LOG_DEBUG(("Pop data %p from thread queue %p, pipe %d", data, queue,
		  pipe_index));

  silc_mutex_unlock(queue->pipes[pipe_index].lock);

  return data;
}

/* Pop entire queue */

SilcDList silc_thread_queue_pop_list(SilcThreadQueue queue, int pipe_index,
				     SilcBool block)
{
  SilcThreadQueueData d;
  SilcDList list;

  SILC_ASSERT(pipe_index < queue->num_pipes);

  silc_mutex_lock(queue->pipes[pipe_index].lock);

  if (block)
    while (silc_list_count(queue->pipes[pipe_index].queue) == 0)
      silc_cond_wait(queue->pipes[pipe_index].cond,
		     queue->pipes[pipe_index].lock);

  list = silc_dlist_init();
  if (!list)
    return NULL;

  silc_list_start(queue->pipes[pipe_index].queue);
  while ((d = silc_list_get(queue->pipes[pipe_index].queue))) {
    silc_dlist_add(list, d->data);
    silc_list_add(queue->pipes[pipe_index].freelist, d);
  }

  silc_list_init(queue->pipes[pipe_index].queue,
		 struct SilcThreadQueueDataStruct, next);

  silc_mutex_unlock(queue->pipes[pipe_index].lock);

  silc_dlist_start(list);

  return list;
}
