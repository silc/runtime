/*

  silclist.h

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 2002 - 2008 Pekka Riikonen

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/

/****h* silcutil/List Interface
 *
 * DESCRIPTION
 *
 * Generic list interface that can turn any structure with list pointers
 * into a SilcList.  The interface can provide both singly and doubly linked
 * lists.  The interface does not allocate any memory.
 *
 * SILC List is not thread-safe.  If the same list context must be used
 * in multithreaded environment concurrency control must be employed.
 *
 * EXAMPLE
 *
 * struct EntryStruct {
 *   char *dummy;
 *   struct EntryStruct *next;        // The list member pointer
 * };
 *
 * SilcList list;
 *
 * // Initialize list
 * silc_list_init(list, struct EntryStruct, next);
 *
 ***/

#ifndef SILCLIST_H
#define SILCLIST_H

/****s* silcutil/SilcList
 *
 * NAME
 *
 *    typedef struct { ... } SilcList;
 *
 * DESCRIPTION
 *
 *    This is the SilcList context, and is initialized by calling the
 *    function silc_list_init.
 *
 ***/
typedef struct SilcListStruct {
  void *head;			     /* Start of the list */
  void *tail;			     /* End of the list */
  void *current;		     /* Current pointer in list */
  SilcUInt16 next_offset;	     /* Offset to 'next' pointer */
  SilcUInt16 prev_offset;	     /* Offset to 'prev' pointer */
  unsigned int prev_set    : 1;	     /* Set if 'prev' exists */
  unsigned int end_set     : 1;	     /* Set if silc_list_end was called */
  unsigned int count       : 30;     /* Number of entries in the list */
} SilcList;

/****d* silcutil/SILC_LIST_END
 *
 * NAME
 *
 *    #define SILC_LIST_END NULL
 *
 * DESCRIPTION
 *
 *    Functions return this when the list is invalid or when traversing
 *    the list there is no more entires in the list.
 *
 * SOURCE
 */
#define SILC_LIST_END NULL
/***/

/****f* silcutil/silc_list_init
 *
 * SYNOPSIS
 *
 *    #define silc_list_init(list, type, nextfield) ...
 *
 * DESCRIPTION
 *
 *    This macro initializes the SilcList list.  The `list' is the defined
 *    list, second argument is the structure of the entries in the list,
 *    and last argument is the pointer in the structure that is used
 *    as next list members.  When using SilcList you must not touch the
 *    structure member pointers manually.  If your list has also a prev
 *    pointer should use silc_list_init_prev instead of this call if
 *    you need to be able traverse the list backwards as well.
 *
 * EXAMPLE
 *
 *    struct SilcInternalEntryStruct {
 *      char *dummy;
 *      struct SilcInternalEntryStruct *next; // The list member pointer
 *    };
 *
 *    SilcList list;
 *    silc_list_init(list, struct SilcInternalEntryStruct, next);
 *
 ***/
#define silc_list_init(list, type, nextfield)		\
do {							\
  (list).count = 0;					\
  (list).next_offset = silc_offsetof(type, nextfield);	\
  (list).prev_set = 0;					\
  (list).prev_offset = 0;				\
  (list).end_set = 0;					\
  (list).head = (list).tail = (list).current = NULL;	\
} while(0)

/****f* silcutil/silc_list_init_prev
 *
 * SYNOPSIS
 *
 *    #define silc_list_init_prev(list, type, nextfield, prevfield) ...
 *
 * DESCRIPTION
 *
 *    This macro initializes the SilcList list.  The `list' is the defined
 *    list, second argument is the structure of the entries in the list,
 *    and last two arguments are the pointers in the structure that is used
 *    as next and prev list members.  When using SilcList you must not
 *    touch the structure member pointers manually.
 *
 *    Having both next and prev pointers makes it possible to traverse
 *    list from both ends of the list (from start to end, and from end
 *    to start).
 *
 * EXAMPLE
 *
 *    struct SilcInternalEntryStruct {
 *      char *dummy;
 *      struct SilcInternalEntryStruct *next; // The list member pointer
 *      struct SilcInternalEntryStruct *prev; // The list member pointer
 *    };
 *
 *    SilcList list;
 *    silc_list_init_prev(list, struct SilcInternalEntryStruct, next, prev);
 *
 ***/
#define silc_list_init_prev(list, type, nextfield, prevfield)	\
do {								\
  (list).count = 0;						\
  (list).next_offset = silc_offsetof(type, nextfield);		\
  (list).prev_offset = silc_offsetof(type, prevfield);		\
  (list).prev_set = 1;						\
  (list).end_set = 0;						\
  (list).head = (list).tail = (list).current = NULL;		\
} while(0)

/****f* silcutil/silc_list_count
 *
 * SYNOPSIS
 *
 *    SilcUInt32 silc_list_count(SilcList list);
 *
 * DESCRIPTION
 *
 *    Returns the number of entries in the list indicated by `list'.
 *
 ***/
#define silc_list_count(list) (list).count

/****f* silcutil/silc_list_start
 *
 * SYNOPSIS
 *
 *    void silc_list_start(SilcList list);
 *
 * DESCRIPTION
 *
 *    Sets the start of the list.  This prepares the list for traversing
 *    the entries from the start of the list towards end of the list.
 *
 ***/
#define silc_list_start(list)				\
  ((list).current = (list).head, (list).end_set = 0)

/****f* silcutil/silc_list_end
 *
 * SYNOPSIS
 *
 *    void silc_list_end(SilcList list);
 *
 * DESCRIPTION
 *
 *    Sets the end of the list.  This prepares the list for traversing
 *    the entries from the end of the list towards start of the list.
 *
 * NOTES
 *
 *    You can use this call only if you initialized the list with
 *    silc_list_init_prev.
 *
 ***/
#define silc_list_end(list)				\
  ((list).current = (list).tail, (list).end_set = 1)

/* Macros to get position to next and prev list pointers */
#define __silc_list_next(list, pos)				\
  ((void **)((unsigned char *)(pos) + (list).next_offset))
#define __silc_list_prev(list, pos)				\
  ((void **)((unsigned char *)(pos) + (list).prev_offset))

/****f* silcutil/silc_list_add
 *
 * SYNOPSIS
 *
 *    void silc_list_add(SilcList list, void *entry);
 *
 * DESCRIPTION
 *
 *    Adds new entry indicated by `entry' to the end of the list indicated
 *    by `list'.
 *
 ***/
#define silc_list_add(list, entry)			\
do {							\
  if (!(list).head)					\
    (list).head = (entry);				\
  else							\
    *__silc_list_next(list, (list).tail) = (entry);	\
  if ((list).prev_set)					\
    *__silc_list_prev(list, entry) = (list).tail;	\
  (list).tail = (entry);				\
  *__silc_list_next(list, entry) = NULL;		\
  (list).count++;					\
} while(0)

/****f* silcutil/silc_list_insert
 *
 * SYNOPSIS
 *
 *    void silc_list_insert(SilcList list, void *current, void *entry);
 *
 * DESCRIPTION
 *
 *    Insert new entry indicated by `entry' after the entry `current'
 *    to the list indicated by `list'.  If `current' is NULL, then the
 *    `entry' is added at the head of the list.  Use the silc_list_add
 *    to add at the end of the list.
 *
 ***/
#define silc_list_insert(list, current, entry)				 \
do {									 \
  if (!(current)) {							 \
    if ((list).head)							 \
      *__silc_list_next(list, entry) = (list).head;			 \
    else								 \
      *__silc_list_next(list, entry) = NULL;				 \
    if ((list).prev_set && (list).head)					 \
      *__silc_list_prev(list, (list).head) = entry;			 \
    if (!(list).tail)							 \
      (list).tail = (entry);						 \
    (list).head = (entry);						 \
    if ((list).prev_set)						 \
      *__silc_list_prev(list, entry) = NULL;				 \
  } else {								 \
    *__silc_list_next(list, entry) = *__silc_list_next(list, current);	 \
    *__silc_list_next(list, current) = entry;				 \
    if ((list).prev_set) {						 \
      *__silc_list_prev(list, entry) = current;				 \
      if (*__silc_list_next(list, entry))				 \
        *__silc_list_prev(list, *__silc_list_next(list, entry)) = entry; \
    }									 \
    if ((list).tail == (current))					 \
      (list).tail = (entry);						 \
  }									 \
  (list).count++;							 \
} while(0)

/****f* silcutil/silc_list_del
 *
 * SYNOPSIS
 *
 *    void silc_list_del(SilcListlist, void *entry);
 *
 * DESCRIPTION
 *
 *    Remove entry indicated by `entry' from the list indicated by `list'.
 *
 ***/
#define silc_list_del(list, entry)					\
do {									\
  void **p, *prev;							\
  prev = NULL;								\
  for (p = &(list).head; *p; p = __silc_list_next(list, *p)) {		\
    if (*p == (entry)) {						\
      *p = *__silc_list_next(list, entry);				\
      if ((list).prev_set && *p)					\
        *__silc_list_prev(list, *p) = *__silc_list_prev(list, entry);	\
      if ((list).current == (entry))					\
        (list).current = *p;						\
      (list).count--;							\
      break;								\
    }									\
    prev = *p;								\
  }									\
  if (entry == (list).tail)						\
    (list).tail = prev;							\
} while(0)

/****f* silcutil/silc_list_get
 *
 * SYNOPSIS
 *
 *    void *silc_list_get(SilcList list);
 *
 * DESCRIPTION
 *
 *    Returns the current entry from the list indicated by `list' and
 *    moves the list pointer forward so that calling this next time will
 *    return the next entry from the list.  This can be used to traverse
 *    the list.  Returns SILC_LIST_END when the entire list has been
 *    tarversed and no additional entries exist in the list. Later,
 *    silc_list_start (or silc_list_end) must be called again when
 *    re-starting the list tarversing.
 *
 * EXAMPLE
 *
 *    // Traverse the list from the beginning to the end
 *    silc_list_start(list);
 *    while ((entry = silc_list_get(list)) != SILC_LIST_END) {
 *      ...
 *    }
 *
 *    // Traverse the list from the end to the beginning
 *    silc_list_end(list);
 *    while ((entry = silc_list_get(list)) != SILC_LIST_END) {
 *      ...
 *    }
 *
 ***/
#define silc_list_get(x) __silc_list_get(&(x))
static inline
void *__silc_list_get(SilcList *list)
{
  void *pos = list->current;
  if (pos)
    list->current = (list->end_set ? *__silc_list_prev(*list, pos) :
		     *__silc_list_next(*list, pos));
  return pos;
}

/****f* silcutil/silc_list_pop
 *
 * SYNOPSIS
 *
 *    void *silc_list_pop(SilcList list);
 *
 * DESCRIPTION
 *
 *    Pops the head of the list.  Removes the head of the list and returns
 *    the removed head.  This will always remove the head of the list even
 *    if silc_list_end was called.  Calling silc_list_start is not necessary.
 *    Returns SILC_LIST_END if the list is empty.
 *
 ***/
#define silc_list_pop(x) __silc_list_pop(&(x))
static inline
void *__silc_list_pop(SilcList *list)
{
  void *head, **p;

  if (!list->head)
    return NULL;

  head = list->head;
  p = &list->head;
  *p = *__silc_list_next(*list, head);
  if (list->prev_set && *p)
    *__silc_list_prev(*list, *p) = *__silc_list_prev(*list, head);

  if (list->current == head)
    list->current = *p;
  if (head == list->tail)
    list->tail = NULL;

  list->count--;

  return head;
}

/****f* silcutil/silc_list_sort
 *
 * SYNOPSIS
 *
 *    void silc_list_sort(SilcList list, SilcCompare compare, context);
 *
 * DESCRIPTION
 *
 *    Sort the list.  The `compare' function will be called with `context'
 *    to do comparison between the entries.  The `compare' must return
 *    SILC_COMPARE_LESS_THAN, SILC_COMPARE_EQUAL_TO, or
 *    SILC_COMPARE_GREATER_THAN zero if the first argument is considered to
 *    be respectively less than, equal to, or greater than the second.
 *    The entries are then sorted in ascending order.  The function must not
 *    return SILC_COMPARE_STOP.
 *
 * NOTES
 *
 *    The list must be initialized with silc_list_init_prev for sorting
 *    to work.
 *
 ***/
#define silc_list_sort(x, comp, ctx) __silc_list_sort(&(x), comp, ctx)
static inline
void __silc_list_sort(SilcList *list, SilcCompare compare, void *context)
{
  void *c_cur, *c_prev;

  SILC_ASSERT(list->prev_set);
  if (!list->prev_set)
    return;

  if (silc_list_count(*list) < 2)
    return;

  /* Gnome sort */
  silc_list_start(*list);
  c_prev = silc_list_get(*list);
  while ((c_cur = silc_list_get(*list))) {
    if (compare(c_prev, c_cur, context) == SILC_COMPARE_GREATER_THAN) {
      list->current = *__silc_list_prev(*list, c_prev);
      silc_list_del(*list, c_prev);
      silc_list_insert(*list, c_cur, c_prev);

      if (list->current) {
        c_prev = list->current;
        list->current = c_cur;
        continue;
      }
      list->current = c_cur;
      __silc_list_get(list);
    }
    c_prev = c_cur;
  }

  list->current = NULL;
}

/****f* silcutil/silc_list_find
 *
 * SYNOPSIS
 *
 *    void *silc_list_find(SilcList list, void *entry1, SilcCompare compare,
 *                         void *context);
 *
 * DESCRIPTION
 *
 *    Find an entry from the `list'.  The `compare' function will be called
 *    with `context' to do comparison between the entries.  If `compare'
 *    returns 0 the `entry1' is considered to be equal to `entry2' and the
 *    found entry is returned.  Returns NULL if the entry could not be found.
 *
 *    The silc_list_start or silc_list_end must be called before calling this
 *    if the finding should be started from the beginning or from the end.
 *    Calling it however is not required.  The founding can be started at
 *    any point of the list.  The silc_list_find can be called many times
 *    to find more entries following the previously found entries.
 *
 *    The list can be enumerated by calling silc_list_get after the found
 *    entry.  The silc_list_get will return the next entry after the found
 *    entry.
 *
 ***/
#define silc_list_find(x, e, comp, ctx) __silc_list_find(&(x), (e), comp, ctx)
static inline
void *__silc_list_find(SilcList *list, void *entry1, SilcCompare compare,
		       void *context)
{
  void *entry2;
  int ret;
  while ((entry2 = __silc_list_get(list))) {
    ret = compare(entry1, entry2, context);
    if (ret == SILC_COMPARE_EQUAL_TO)
      return entry2;
    if (ret == SILC_COMPARE_STOP)
      break;
  }
  return NULL;
}

#endif /* SILCLIST_H */
