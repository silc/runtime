/*

  silcavltree.c

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 2008 Pekka Riikonen

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote
      products derived from this software without specific prior written
      permission.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
  NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* AVL Tree.  This code is based on code in libdict package.  I am putting
   all changes under the same license, that is the revised BSD license.  The
   original code is copyright by Farooq Mela.

   Main differences are that this implementation does not use a separate
   node context but the user provided entry itself is the node in the tree,
   thus avoiding issues like memory allocation, cleanup, etc., and making
   the interface more generic.  This interface does not allocate any
   memory.  This implementation also supports duplicates by adding them to
   a simple list. */

#include <silcruntime.h>

/* Define to 1 if you want hash table debug enabled */
#ifndef SILC_AVL_TREE_DEBUG
#define SILC_AVL_TREE_DEBUG 0
#endif /* !SILC_AVL_TREE_DEBUG */

#if SILC_ALV_TREE_DEBUG == 1
#define SILC_TREE_DEBUG(fmt) SILC_LOG_DEBUG(fmt)
#else
#define SILC_TREE_DEBUG(fmt)
#endif

/************************ Static utility functions **************************/

/* Rotate left

      /             /
     B             D
    / \           / \
   A   D   ==>   B   E
      / \       / \
     C   E     A   C
*/

static int silc_avl_tree_rot_left(SilcTree *tree, SilcTreeHeader *h)
{
  SilcTreeHeader *right, *parent;
  int hc;

  SILC_ASSERT(h);
  SILC_ASSERT(h->right);

  right = h->right;
  h->right = right->left;
  if (right->left)
    right->left->parent = h;

  parent = h->parent;
  right->parent = parent;

  if (parent) {
    if (parent->left == h)
      parent->left = right;
    else
      parent->right = right;
  } else {
    tree->root = right;
  }

  right->left = h;
  h->parent = right;

  hc = right->t != 0;
  h->t -= 1 + SILC_MAX(right->t, 0);
  right->t -= 1 - SILC_MIN(h->t, 0);

  return hc;
}

/* Rotate right

        /           /
       D           B
      / \         / \
     B   E  ==>  A   D
    / \             / \
   A   C           C   E
*/

static int silc_avl_tree_rot_right(SilcTree *tree, SilcTreeHeader *h)
{
  SilcTreeHeader *left, *parent;
  int hc;

  SILC_ASSERT(h);
  SILC_ASSERT(h->left);

  left = h->left;
  h->left = left->right;
  if (left->right)
    left->right->parent = h;

  parent = h->parent;
  left->parent = parent;

  if (parent) {
    if (parent->left == h)
      parent->left = left;
    else
      parent->right = left;
  } else {
    tree->root = left;
  }

  left->right = h;
  h->parent = left;

  hc = left->t != 0;
  h->t += 1 - SILC_MIN(left->t, 0);
  left->t += 1 + SILC_MAX(h->t, 0);

  return hc;
}

/****************************** Public API **********************************/

/* Find entry */

void *silc_avl_tree_find(SilcTree *tree, void *entry,
			 SilcCompare compare, void *context)
{
  SilcTreeHeader *h;
  SilcCompare cmp = compare ? compare : tree->compare;
  void *cmp_context = compare ? context : tree->context;
  int ret;

  SILC_TREE_DEBUG(("AVL tree %p, find %p", tree, entry));

  h = tree->root;
  while (h) {
    ret = cmp(entry, SILC_TREE_GET_ENTRY(tree, h), cmp_context);
    if (ret == SILC_COMPARE_EQUAL_TO) {
      SILC_TREE_DEBUG(("Found %p", SILC_TREE_GET_ENTRY(tree, h)));
      return SILC_TREE_GET_ENTRY(tree, h);
    }
    if (ret == SILC_COMPARE_STOP)
      return NULL;
    h = ret > 0 ? h->right : h->left;
  }

  SILC_TREE_DEBUG(("Not found"));
  silc_set_errno_nofail(SILC_ERR_NOT_FOUND);
  return NULL;
}

/* Insert entry to tree */

SilcBool silc_avl_tree_add(SilcTree *tree, void *entry)
{
  SilcTreeHeader *h, *parent = NULL, *q = NULL;
  int ret = 0;

  SILC_TREE_DEBUG(("AVL tree %p, adding %p", tree, entry));

  /* If tree is empty, add to root */
  if (!tree->root) {
    h = SILC_TREE_GET_HEADER(tree, entry);
    h->parent = h->left = h->right = h->dup = NULL;
    h->t = 0;
    tree->root = h;

    SILC_TREE_DEBUG(("Entry %p added as root", entry));

    SILC_ASSERT(!tree->count);
    tree->count = 1;
    return TRUE;
  }

  /* Find the spot to add the new entry */
  h = tree->root;
  while (h) {
    /* Same entry context must not be in tree */
    if (entry == SILC_TREE_GET_ENTRY(tree, h)) {
      silc_set_errno_nofail(SILC_ERR_ALREADY_EXISTS);
      return FALSE;
    }

    ret = tree->compare(entry, SILC_TREE_GET_ENTRY(tree, h), tree->context);
    if (!tree->duplicates && ret == SILC_COMPARE_EQUAL_TO) {
      silc_set_errno_nofail(SILC_ERR_ALREADY_EXISTS);
      return FALSE;
    }

    /* Duplicate entry, add to list */
    if (ret == SILC_COMPARE_EQUAL_TO) {
      q = SILC_TREE_GET_HEADER(tree, entry);
      q->duplicate = TRUE;
      q->parent = h;
      if (h->dup)
	h->dup->parent = q;
      q->dup = h->dup;
      h->dup = q;
      SILC_TREE_DEBUG(("Entry %p is duplicate to %p", entry,
		      SILC_TREE_GET_ENTRY(tree, h)));
      tree->count++;
      return TRUE;
    }

    parent = h;
    if (parent->t)
      q = parent;
    h = ret > 0 ? h->right : h->left;
  }

  /* Update parent */
  h = SILC_TREE_GET_HEADER(tree, entry);
  if (ret < 0)
    parent->left = h;
  else
    parent->right = h;

  SILC_TREE_DEBUG(("Entry %p added, parent %p", entry,
		   SILC_TREE_GET_ENTRY(tree, parent)));

  /* Update the new entry */
  h->parent = parent;
  h->left = h->right = h->dup = NULL;
  h->t = 0;

  /* Balance */
  while (parent != q) {
    parent->t = (parent->right == h) * 2 - 1;
    h = parent;
    parent = h->parent;
  }

  if (q) {
    if (q->left == h) {
      if (--q->t == (-2)) {
	if (q->left->t > 0)
	  silc_avl_tree_rot_left(tree, q->left);
	silc_avl_tree_rot_right(tree, q);
      }
    } else {
      if (++q->t == 2) {
	if (q->right->t < 0)
	  silc_avl_tree_rot_right(tree, q->right);
	silc_avl_tree_rot_left(tree, q);
      }
    }
  }

  tree->count++;
  return TRUE;
}

/* Delete entry from tree */

SilcBool silc_avl_tree_del(SilcTree *tree, void *entry)
{
  SilcTreeHeader *h, *q, *out, *parent = NULL, tmp;
  int left;

  SILC_TREE_DEBUG(("AVL tree %p, delete %p", tree, entry));

  if (!tree->root || !entry) {
    silc_set_errno_nofail(SILC_ERR_INVALID_ARGUMENT);
    return FALSE;
  }

  /* Unless the incoming entry is already the one to be deleted find it
     from the tree first. */
  q = h = SILC_TREE_GET_HEADER(tree, entry);
  if (!h->parent && !h->left && !h->right && !h->t) {
    entry = silc_avl_tree_find(tree, entry, NULL, NULL);
    if (!entry)
      return FALSE;
    q = h = SILC_TREE_GET_HEADER(tree, entry);
  }

  /* If entry has duplicates, replace this with one of them */
  if (h->dup) {
    h->dup->duplicate = FALSE;
    h->dup->parent = h->parent;
    h->dup->left = h->left;
    h->dup->right = h->right;
    h->dup->t = h->t;

    /* Update parent */
    if (h->parent) {
      if (h->parent->left == h)
	h->parent->left = h->dup;
      else
	h->parent->right = h->dup;
    } else {
      tree->root = h->dup;
    }

    /* Update leafs */
    if (h->left)
      h->left->parent = h->dup;
    if (h->right)
      h->right->parent = h->dup;

    /* Cleanup the deleted entry */
    SILC_TREE_DEBUG(("Deleted %p", SILC_TREE_GET_ENTRY(tree, h)));
    h->parent = h->left = h->right = h->dup = NULL;
    h->t = 0;

    tree->count--;
    return TRUE;
  }

  /* If the entry is not a leaf, swap with the smallest from right side */
  if (h->left && h->right) {
    for (q = out = h->right; out->left; q = out = out->left) ;
    tmp = *h;
    *h = *out;
    *out = tmp;

    /* Update node's parent with the replaced node */
    if (out->parent) {
      if (out->parent->left == h)
	out->parent->left = out;
      else
	out->parent->right = out;
    } else {
      tree->root = out;
    }

    /* Update parents and leafs */
    if (h->parent == h)
      h->parent = out;
    if (out->left == out)
      out->left = h;
    else if (out->right == out)
      out->right = h;
    out->left->parent = out;
    out->right->parent = out;
  }

  parent = h->parent;
  out = h->left ? h->left : h->right;
  if (out)
    out->parent = parent;

  /* Cleanup the deleted entry */
  SILC_TREE_DEBUG(("Deleted %p", SILC_TREE_GET_ENTRY(tree, h)));
  h->parent = h->left = h->right = h->dup = NULL;
  h->t = 0;

  if (!parent) {
    tree->root = out;
    tree->count--;
    return TRUE;
  }

  /* Update parent */
  left = parent->left == q;
  if (left)
    parent->left = out;
  else
    parent->right = out;

  /* Balance */
  for (;;) {
    if (left) {
      if (++parent->t == 0) {
	h = parent;
	goto higher;
      }
      if (parent->t == 2) {
	SILC_ASSERT(parent->right);
	if (parent->right->t < 0) {
	  silc_avl_tree_rot_right(tree, parent->right);
	  silc_avl_tree_rot_left(tree, parent);
	} else {
	  SILC_ASSERT(parent->right->right);
	  if (silc_avl_tree_rot_left(tree, parent) == 0)
	    break;
	}
      } else {
	break;
      }
    } else {
      if (--parent->t == 0) {
	h = parent;
	goto higher;
      }
      if (parent->t == (-2)) {
	SILC_ASSERT(parent->left);
	if (parent->left->t > 0) {
	  silc_avl_tree_rot_left(tree, parent->left);
	  silc_avl_tree_rot_right(tree, parent);
	} else {
	  SILC_ASSERT(parent->left->left);
	  if (silc_avl_tree_rot_right(tree, parent) == 0)
	    break;
	}
      } else {
	break;
      }
    }

    /* Only get here on double rotations or single rotations that changed
       subtree height - in either event, `parent->parent' is positioned
       where `parent' was positioned before any rotations. */
    h = parent->parent;
  higher:
    if ((parent = h->parent) == NULL)
      break;
    left = parent->left == h;
  }

  tree->count--;
  return TRUE;
}

/* AVL tree operations */
const struct SilcTreeOpsStruct silc_tree_avl_ops =
{
  silc_avl_tree_add,
  silc_avl_tree_del,
  silc_avl_tree_find,
};
