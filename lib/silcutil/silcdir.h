/*

  silcdir.h

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

/****h* silcutil/Directory Interface
 *
 * DESCRIPTION
 *
 * The SILC Directory API provides portable way to open and read directories
 * and their content.
 *
 * EXAMPLE
 *
 * SilcDir dir;
 * SilcDirEntry entry;
 *
 * dir = silc_dir_open("foodir");
 *
 * while ((entry = silc_dir_read(dir, NULL)))
 *   printf("File name: %s\n", silc_dir_entry_name(entry));
 *
 * silc_dir_close(dir);
 *
 ***/

#ifndef SILCDIR_H
#define SILCDIR_H

/****s* silcutil/SilcDir
 *
 * NAME
 *
 *    typedef struct SilcDirStruct *SilcDir;
 *
 * DESCRIPTION
 *
 *    The directory context.  This is allocated by silc_dir_open and
 *    freed by calling silc_dir_close.
 *
 ***/
typedef struct SilcDirStruct *SilcDir;

/****s* silcutil/SilcDirEntry
 *
 * NAME
 *
 *    typedef struct SilcDirEntryStruct *SilcDirEntry;
 *
 * DESCRIPTION
 *
 *    The directory entry context.  The entry is usually a file in the
 *    directory.
 *
 ***/
typedef struct SilcDirEntryStruct *SilcDirEntry;

/****f* silcutil/silc_dir_open
 *
 * SYNOPSIS
 *
 *    SilcDir silc_dir_open(const char *name);
 *
 * DESCRIPTION
 *
 *    Opens the directory named `name' and returns its context.  Returns NULL
 *    on error and sets the silc_errno.  This function must be called before
 *    being able to read the directory and its contents.
 *
 ***/
SilcDir silc_dir_open(const char *name);

/****f* silcutil/silc_dir_close
 *
 * SYNOPSIS
 *
 *    void silc_dir_close(SilcDir dir);
 *
 * DESCRIPTION
 *
 *    Closes the directory `dir'.
 *
 ***/
void silc_dir_close(SilcDir dir);

/****f* silcutil/silc_dir_read
 *
 * SYNOPSIS
 *
 *    SilcDirEntry silc_dir_read(SilcDir dir, SilcFileStat status);
 *
 * DESCRIPTION
 *
 *    Reads next entry (file) from the directory `dir'.  The silc_dir_open
 *    must be called first before reading from the directory.  Returns the
 *    next entry context or NULL if there are no more entries or error occurs.
 *    In case of error the silc_errno is also set.
 *
 *    If the `status' is non-NULL this will also call silc_file_stat and
 *    returns the status into the `status' pointer.
 *
 *    The returned context remains valid until the silc_dir_read is called
 *    again.
 *
 ***/
SilcDirEntry silc_dir_read(SilcDir dir, SilcFileStat status);

/****f* silcutil/silc_dir_rewind
 *
 * SYNOPSIS
 *
 *    void silc_dir_rewind(SilcDir dir);
 *
 * DESCRIPTION
 *
 *    Rewinds the directory `dir' to the beginning of the directory.  Calling
 *    silc_dir_read after this will return the first entry in the directory.
 *
 ***/
void silc_dir_rewind(SilcDir dir);

/****f* silcutil/silc_dir_name
 *
 * SYNOPSIS
 *
 *    const char *silc_dir_name(SilcDir dir);
 *
 * DESCRIPTION
 *
 *    Returns the name of the directory from `dir' context.
 *
 ***/
const char *silc_dir_name(SilcDir dir);

/****f* silcutil/silc_dir_entry_name
 *
 * SYNOPSIS
 *
 *    const char *silc_dir_entry_name(SilcDirEntry entry);
 *
 * DESCRIPTION
 *
 *    Returns the name of the entry (file) `entry'.  The returned pointer
 *    remains valid until the silc_dir_read is called again.
 *
 ***/
const char *silc_dir_entry_name(SilcDirEntry entry);

#endif /* SILCDIR_H */
