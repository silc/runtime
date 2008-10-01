/*

  silcmemory.h

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 1999 - 2008 Pekka Riikonen

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/

/****h* silcutil/Memory Interface
 *
 * DESCRIPTION
 *
 * Routines for allocating and freeing memory.
 *
 ***/

#ifndef SILCMEMORY_H
#define SILCMEMORY_H

/* Prototypes */

#ifndef SILC_MEMTRACE

/****f* silcutil/silc_malloc
 *
 * SYNOPSIS
 *
 *    void *silc_malloc(size_t size);
 *
 * DESCRIPTION
 *
 *    Allocates memory of `size' bytes and returns pointer to the allocated
 *    memory area.  Free the memory by calling silc_free.  Returns NULL on
 *    error and sets silc_errno.
 *
 ***/
void *silc_malloc(size_t size);

/****f* silcutil/silc_calloc
 *
 * SYNOPSIS
 *
 *    void *silc_calloc(size_t items, size_t size);
 *
 * DESCRIPTION
 *
 *    Allocates memory of for an array of `items' elements of `size' bytes
 *    and returns pointer to the allocated memory area.  The memory area is
 *    also zeroed.  Free the memory by calling silc_free.  Returns NULL on
 *    error and sets silc_errno.
 *
 ***/
void *silc_calloc(size_t items, size_t size);

/****f* silcutil/silc_realloc
 *
 * SYNOPSIS
 *
 *    void *silc_realloc(void *ptr, size_t size);
 *
 * DESCRIPTION
 *
 *    Change the size of the memory block indicated by `ptr' to the new
 *    size of `size' bytes.  The contents of `ptr' will not be changed.
 *    If `ptr' is NULL the call is equivalent to silc_malloc.  If the
 *    `size' is zero (0) the call is equivalent to silc_free.  Free the
 *    memory by calling silc_free.
 *
 * NOTES
 *
 *    The pointer returned to the reallocated memory area might not be
 *    same as `ptr'.
 *
 ***/
void *silc_realloc(void *ptr, size_t size);

/****f* silcutil/silc_free
 *
 * SYNOPSIS
 *
 *    void silc_free(void *ptr);
 *
 * DESCRIPTION
 *
 *    Frees the memory area indicated by the `ptr'. If `ptr' is NULL no
 *    operation is performed.
 *
 ***/
void silc_free(void *ptr);

/****f* silcutil/silc_memdup
 *
 * SYNOPSIS
 *
 *    void *silc_memdup(const void *ptr, size_t size);
 *
 * DESCRIPTION
 *
 *    Duplicates the memory area indicated by `ptr' which is of size
 *    of `size' bytes. Returns pointer to the duplicated memory area.
 *    This NULL terminates the dupped memory area by allocating `size' + 1
 *    bytes, so this function can be used to duplicate strings that does
 *    not have NULL termination.
 *
 ***/
void *silc_memdup(const void *ptr, size_t size);

/****f* silcutil/silc_strdup
 *
 * SYNOPSIS
 *
 *    char *silc_strdup(const char *str);
 *
 * DESCRIPTION
 *
 *    Duplicates the string indicated by `str' and returns the duplicated
 *    string.  Returns NULL on error and sets silc_errno.
 *
 ***/
char *silc_strdup(const char *str);

#else
#include "memtrace.h"
#endif /* SILC_MEMTRACE */


/* Following functions that use SilcStack as memory source. */

/****f* silcutil/silc_smalloc
 *
 * SYNOPSIS
 *
 *    void *silc_smalloc(SilcStack stack, SilcUInt32 size);
 *
 * DESCRIPTION
 *
 *    Allocate memory block of size of `size' from the stack indicated by
 *    `stack' and return pointer to it.  Returns NULL on error.  This
 *    function allocates aligned memory so it can be used to allocate
 *    memory for structures, for example.
 *
 * NOTES
 *
 *    Be careful with this function:  do not free the returned pointer
 *    explicitly and do not save the returned pointer to a permanent
 *    location.
 *
 *    If `stack' is NULL this function calls silc_malloc.
 *
 ***/
void *silc_smalloc(SilcStack stack, SilcUInt32 size);

/****f* silcutil/silc_scalloc
 *
 * SYNOPSIS
 *
 *    void *silc_scalloc(SilcStack stack, SilcUInt32 items, SilcUInt32 size);
 *
 * DESCRIPTION
 *
 *    Allocate memory block of size of `size' from the stack indicated by
 *    `stack', zero the memory area and return pointer to it.  This
 *    function allocates aligned memory.  Returns NULL on error.
 *
 * NOTES
 *
 *    Be careful with this function:  do not free the returned pointer
 *    explicitly and do not save the returned pointer to a permanent
 *    location.
 *
 *    If `stack' is NULL this function calls silc_calloc.
 *
 ***/
void *silc_scalloc(SilcStack stack, SilcUInt32 items, SilcUInt32 size);

/****f* silcutil/silc_srealloc
 *
 * SYNOPSIS
 *
 *    void *silc_srealloc(SilcStack stack, SilcUInt32 old_size,
 *                        void *ptr, SilcUInt32 size);
 *
 * DESCRIPTION
 *
 *    Change the size of the memory block indicated by `ptr' to the new
 *    size of `size' bytes.  The contents of `ptr' will not be changed.
 *    If `ptr' is NULL the call is equivalent to silc_smalloc.  If `size'
 *    is zero (0) error will occur.  Returns NULL on error and the old
 *    pointer remain intact.  This may return different pointer from `ptr'
 *
 * NOTES
 *
 *    If the reallocation from `stack' fails, this function will allocate
 *    new block of size of `size' bytes from `stack' and copy the data from
 *    `ptr' to the new memory block.
 *
 *    If `stack' is NULL this function calls silc_realloc.
 *
 ***/
void *silc_srealloc(SilcStack stack, SilcUInt32 old_size,
		    void *ptr, SilcUInt32 size);

/****f* silcutil/silc_smemdup
 *
 * SYNOPSIS
 *
 *    void *silc_smemdup(SilcStack stack, const void *ptr, SilcUInt32 size);
 *
 * DESCRIPTION
 *
 *    Duplicates the memory area indicated by `ptr' which is the size of
 *    `size' bytes.  Returns pointer to the duplicated memory area.  This
 *    NULL terminates the dupped memory area by allocating `size' + 1
 *    bytes, so this function can be used to duplicate strings that does not
 *    have NULL termination.  This function allocates aligned memory so
 *    it can be used to duplicate also structures.  Returns NULL on error.
 *
 * NOTES
 *
 *    Be careful with this function:  do not free the returned pointer
 *    explicitly and do not save the returned pointer to a permanent
 *    location.
 *
 *    If `stack' is NULL this function calls silc_memdup.
 *
 ***/
void *silc_smemdup(SilcStack stack, const void *ptr, SilcUInt32 size);

/****f* silcutil/silc_sfree
 *
 * SYNOPSIS
 *
 *    void silc_scalloc(SilcStack stack, void *ptr);
 *
 * DESCRIPTION
 *
 *    This function can be used to free the `ptr' if `stack' is NULL.  This
 *    function does nothing if `stack' is non-NULL.  When `stack' is NULL
 *    this function calls silc_free.
 *
 ***/
void silc_sfree(SilcStack stack, void *ptr);

/****f* silcutil/silc_sstrdup
 *
 * SYNOPSIS
 *
 *    char *silc_sstrdup(SilcStack stack, const char *str);
 *
 * DESCRIPTION
 *
 *    Duplicates the string indicated by `str' and returns the duplicated
 *    string.  Returns NULL on error.
 *
 * NOTES
 *
 *    Be careful with this function:  do not free the returned pointer
 *    explicitly and do not save the returned pointer to a permanent
 *    location.
 *
 *    If `stack' is NULL this function calls silc_strdup.
 *
 ***/
char *silc_sstrdup(SilcStack stack, const char *str);

#endif /* SILCMEMORY_H */
