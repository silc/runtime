/*

  silctree_i.h

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

#ifndef SILCTREE_I_H
#define SILCTREE_I_H

#ifndef SILCTREE_H
#error "Do not include this header directly"
#endif

/* Tree operatins header */
struct SilcTreeOpsStruct {
  SilcBool (*add)(SilcTree *tree, void *entry);
  SilcBool (*del)(SilcTree *tree, void *entry);
  void *(*find)(SilcTree *tree, void *entry, SilcTreeCompare compare,
		void *context);
};

/* Generic tree header, present in each entry in tree */
struct SilcTreeHeaderStruct {
  struct SilcTreeHeaderStruct *parent;
  struct SilcTreeHeaderStruct *left;
  struct SilcTreeHeaderStruct *right;
  struct SilcTreeHeaderStruct *dup;
  SilcInt16 t;
  unsigned int duplicate   : 1;
};

/* Tree context */
struct SilcTreeStruct {
  const struct SilcTreeOpsStruct *ops;
  SilcTreeHeader *root;
  SilcTreeCompare compare;
  void *context;
  SilcUInt32 count;
  unsigned int offset      : 31;
  unsigned int duplicates  : 1;
};

/* Get tree header from entry */
#define SILC_TREE_GET_HEADER(tree, pos)			\
  ((void *)((unsigned char *)(pos) + tree->offset))

/* Get entry from tree header */
#define SILC_TREE_GET_ENTRY(tree, pos)			\
  ((void *)((unsigned char *)(pos) - tree->offset))

#endif /* SILCTREE_I_H */
