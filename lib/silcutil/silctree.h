/*

  silctree.h

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

/****h* silcutil/Binary Search Tree Interface
 *
 * DESCRIPTION
 *
 * Binary Search Tree Interface provides simple interface for adding,
 * deleting, retrieving and enumerating items in a tree.  The inteface
 * allows also duplicate values in the tree, and it does not allocate any
 * memory.  The interface can support many types of binary search trees.
 *
 * The interface is not thread-safe.  If the same SilcTree context must be
 * used in multithreaded environment concurrency control must be employed.
 *
 ***/

#ifndef SILCTREE_H
#define SILCTREE_H

/****s* silcutil/SilcTree
 *
 * NAME
 *
 *    typedef struct SilcTreeStruct SilcTree;
 *
 * DESCRIPTION
 *
 *    This is the SilcTree context, and is initialized by calling the
 *    function silc_tree_init.  It need not be uninitialized.
 *
 ***/
typedef struct SilcTreeStruct SilcTree;

/****s* silcutil/SilcTreeHeader
 *
 * NAME
 *
 *    typedef struct SilcTreeHeaderStruct SilcTreeHeader;
 *
 * DESCRIPTION
 *
 *    This structure must be present in each context that is added to the
 *    tree.  The structure can be at any place in the context.
 *
 * EXAMPLE
 *
 * // Structure that is going to be added to tree
 * typedef struct {
 *   SilcTreeHeader header;
 *   char *name;
 *   int id;
 * } FooEntry;
 *
 ***/
typedef struct SilcTreeHeaderStruct SilcTreeHeader;

/****d* silcutil/SilcTreeType
 *
 * NAME
 *
 *    typedef enum { ... } SilcTreeType;
 *
 * DESCRIPTION
 *
 *    The supported tree types.
 *
 * SOURCE
 */
typedef enum {
  /* AVL Tree.  Automatically balancing binary search tree that provides
     excellent performance. */
  SILC_TREE_AVL    = 0,
} SilcTreeType;
/***/

#include "silctree_i.h"

/* Tree implementations */
extern DLLAPI const struct SilcTreeOpsStruct silc_tree_avl_ops;

/****f* silcutil/silc_tree_init
 *
 * SYNOPSIS
 *
 *    SilcBool silc_tree_init(SilcTree tree, SilcTreeType type,
 *                            SilcCompare compare, void *context,
 *                            SilcUInt16 offset, SilcBool duplicates);
 *
 * DESCRIPTION
 *
 *    Initializes the `tree' as a tree type indicated by `type'.  The
 *    `compare' function will be called to compare entries in the tree,
 *    for example, when finding entries from the tree.  The `context' is
 *    delivered to the callback function.
 *
 *    The `offset' is the byte offset of the SilcTreeHeader structure field
 *    in the entry structure that is going to be added to the tree.  Each
 *    structure that is added to the tree must contain SilcTreeHeader
 *    structure.  Use silc_offsetof to get the byte offset.
 *
 *    If the `duplicates' is TRUE the tree will allow the addition of
 *    duplicate values.  However, the entry context to be added must not
 *    already be in the tree, but its value may be same as some other
 *    context's.
 *
 *    The `tree' need not be uninitialized.  The caller is responsible of
 *    freeing the entries it has added to the tree.
 *
 *    Return FALSE if the `type' is of unknown tree type.  Returns TRUE
 *    otherwise.  This function does not allocate any memory.
 *
 * EXAMPLE
 *
 * // Structure that is going to be added to tree
 * typedef struct {
 *   SilcTreeHeader header;
 *   char *name;
 *   int id;
 * } FooEntry;
 *
 * SilcTree tree;
 * silc_tree_init(tree, SILC_TREE_AVL, compare, context,
 *                silc_offsetof(FooEntry, header));
 *
 ***/
#define silc_tree_init(tree, type, compare, context, offset, d)	\
  __silc_tree_init(&(tree), type, compare, context, offset, d)
static inline
SilcBool __silc_tree_init(SilcTree *tree,
			  SilcTreeType type, SilcCompare compare,
			  void *context, SilcUInt32 offset,
			  SilcBool duplicates)
{
  switch (type) {
  case SILC_TREE_AVL:
    tree->ops = &silc_tree_avl_ops;
    break;

  default:
    return FALSE;
    break;
  }

  tree->root = NULL;
  tree->compare = compare;
  tree->context = context;
  tree->count = 0;
  tree->offset = offset;
  tree->duplicates = duplicates;

  return TRUE;
}

/****f* silcutil/silc_tree_add
 *
 * SYNOPSIS
 *
 *    SilcBool silc_tree_add(SilcTree *tree, void *entry);
 *
 * DESCRIPTION
 *
 *    Add `entry' to the `tree'.  Returns TRUE after the entry has been
 *    added to the tree.  If the `tree' does not allow duplicate entries
 *    this will return FALSE if same value as `entry' is already in the
 *    tree.
 *
 * EXAMPLE
 *
 * FooEntry *client_entry;
 *
 * client_entry->id = id;
 * client_entry->name = name;
 * silc_tree_add(tree, client_entry);
 *
 ***/
#define silc_tree_add(tree, e) (tree).ops->add(&(tree), (e))

/****f* silcutil/silc_tree_del
 *
 * SYNOPSIS
 *
 *    SilcBool silc_tree_del(SilcTree *tree, void *entry);
 *
 * DESCRIPTION
 *
 *    Delete entry from the `tree'.  If the `entry' is not the actual entry
 *    context that was added to the tree but is merely a temporary context
 *    it must be memset'ed to zero (0) initially.  The deletion routine will
 *    assume that the given `entry' is the actual added entry context if its
 *    SilcTreeHeader structure is not zeroed when deleting the entry.  If it
 *    is zeroed the deletion will first try to find the entry from the tree
 *    and then delete the found entry.  See example for both cases.
 *
 *    Return FALSE if the entry does not exist in the tree.  Returns TRUE
 *    after successful deletion.
 *
 * EXAMPLE
 *
 * // Delete the entry, we have access to the originally added context
 * silc_tree_del(tree, client_entry);
 *
 * // Delete client entry with ID 100
 * FooEntry tmp;
 * memset(&tmp, 0, sizeof(tmp));
 * tmp.id = 100;
 * silc_tree_del(tree, &tmp);
 *
 * // Delete all entries from the tree
 * while ((entry = silc_tree_enumerate(tree, NULL)))
 *   silc_tree_del(tree, entry);
 *
 ***/
#define silc_tree_del(tree, e) (tree).ops->del(&(tree), (e))

/****f* silcutil/silc_tree_find
 *
 * SYNOPSIS
 *
 *    void *silc_tree_find(SilcTree *tree, void *entry);
 *
 * DESCRIPTION
 *
 *    Find entry from the tree.  Returns the found entry or NULL if the
 *    entry does not exist in the tree and sets silc_errno.
 *
 *    If there are duplicate values in the tree this will find the first
 *    one.  Rest of the duplicate values can be found by calling
 *    silc_tree_enumerate_duplicates.  It will stop the enumeration when
 *    the last duplicate entry is returned.
 *
 * EXAMPLE
 *
 * FooEntry probe, *client_entry;
 *
 * // Find entry by ID 100
 * probe.id = 100;
 * client_entry = silc_tree_find(tree, &probe);
 *
 ***/
#define silc_tree_find(tree, e) (tree).ops->find(&(tree), (e), NULL, NULL)

/****f* silcutil/silc_tree_find_ext
 *
 * SYNOPSIS
 *
 *    void *silc_tree_find_ext(SilcTree *tree, void *entry,
 *                             SilcCompare compare, void *context);
 *
 * DESCRIPTION
 *
 *    Same as silc_tree_find but takes a separate comparison function as
 *    argument.
 *
 ***/
#define silc_tree_find_ext(tree, e, compare, context) \
  (tree).ops->find(&(tree), (e), (compare), (context))

/****f* silcutil/silc_tree_count
 *
 * SYNOPSIS
 *
 *    SilcUInt32 silc_tree_count(SilcTree *tree);
 *
 * DESCRIPTION
 *
 *    Returns the number of entries in the tree.
 *
 ***/
#define silc_tree_count(tree) (tree).count

/****f* silcutil/silc_tree_minmax
 *
 * SYNOPSIS
 *
 *    void *silc_tree_minmax(SilcTree *tree, SilcBool min);
 *
 * DESCRIPTION
 *
 *    Returns either smallest or largest value from the `tree'.  If the `min'
 *    is TRUE returns the smallest, otherwise returns the largest.  Returns
 *    NULL if the tree is empty.
 *
 ***/
#define silc_tree_minmax(tree, min) __silc_tree_minmax(&(tree), (min))
static inline
void *__silc_tree_minmax(SilcTree *tree, SilcBool min)
{
  SilcTreeHeader *h;

  h = tree->root;
  if (!h)
    return NULL;

  if (min)
    while (h->left)
      h = h->left;
  else
    while (h->right)
      h = h->right;

  return SILC_TREE_GET_ENTRY(tree, h);
}

/****f* silcutil/silc_tree_enumerate
 *
 * SYNOPSIS
 *
 *    void *silc_tree_enumerate(SilcTree *tree, void *at);
 *
 * DESCRIPTION
 *
 *    Enumerates the `tree' starting/continuing at the `at'.  When `at' is
 *    NULL this will start enumeration from the root of the tree.  The found
 *    entry must be given as `at' for next call to continue the enumeration
 *    in order.  The enumeration is done in ascending order starting from the
 *    smallest value.  If there are duplicates in the tree, their order is
 *    undefined.  Returns NULL at the end of the enumeration.
 *
 * EXAMPLE
 *
 * // Start enumerating from beginning in order
 * for (entry = silc_tree_enumerate(tree, NULL); entry != NULL;
 *      entry = silc_tree_enumerate(tree, entry))
 *   printf("Client entry %s\n", entry->name);
 *
 * // Delete all entries from the tree
 * while ((entry = silc_tree_enumerate(tree, NULL)))
 *   silc_tree_del(tree, entry);
 *
 ***/
#define silc_tree_enumerate(tree, e) __silc_tree_enumerate(&(tree), (e))
static inline
void *__silc_tree_enumerate(SilcTree *tree, void *at)
{
  SilcTreeHeader *h, *p;

  if (at == NULL)
    return __silc_tree_minmax(tree, TRUE);

  h = SILC_TREE_GET_HEADER(tree, at);

  if (h->dup)
    return SILC_TREE_GET_ENTRY(tree, h->dup);
  else if (h->duplicate)
    for (h = h->parent; h->duplicate; h = h->parent) ;

  if (h->right) {
    for (h = h->right; h->left; h = h->left) ;
    return SILC_TREE_GET_ENTRY(tree, h);
  }

  p = h->parent;
  while (p && p->right == h) {
    h = p;
    p = p->parent;
  }

  return p ? SILC_TREE_GET_ENTRY(tree, p) : NULL;
}

/****f* silcutil/silc_tree_enumerate_duplicates
 *
 * SYNOPSIS
 *
 *    void *silc_tree_enumerate_duplicates(SilcTree *tree, void *at);
 *
 * DESCRIPTION
 *
 *    Enumerates all duplicate values starting at the `at'.  If `at' is the
 *    only one of that value in the tree this will return NULL.  Returns same
 *    values as `at' until there are no more and will then return NULL.  The
 *    `at' must not be NULL.
 *
 * EXAMPLE
 *
 * // Find all entries of ID 100
 * probe.id = 100;
 * entry = silc_tree_find(tree, &probe);
 * printf("Entry %p ID %d\n", entry, entry->id);
 * while ((entry = silc_tree_enumerate_duplicates(tree, entry)))
 *   printf("Entry %p ID %d\n", entry, entry->id);
 *
 ***/
#define silc_tree_enumerate_duplicates(tree, e) \
  __silc_tree_enumerate_dup(&(tree), (e))
static inline
void *__silc_tree_enumerate_dup(SilcTree *tree, void *at)
{
  SilcTreeHeader *h = SILC_TREE_GET_HEADER(tree, at);
  return h->dup ? SILC_TREE_GET_ENTRY(tree, h->dup) : NULL;
}

#endif /* SILCTREE_H */
