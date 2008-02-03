/*

  silcunixdir.c

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

/* Directory entry context */
struct SilcDirEntryStruct {
  struct dirent *entry;			  /* Entry */
  SilcFileStatStruct status;		  /* Status */
};

/* The directory context */
struct SilcDirStruct {
  DIR *dir;			          /* Directory */
  char *name;				  /* Directory name */
  struct SilcDirEntryStruct entry;	  /* Current entry */
};

/****************************** SILC Dir API ********************************/

/* Open directory */

SilcDir silc_dir_open(const char *name)
{
  SilcDir dir;

  if (!name || !strlen(name)) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return NULL;
  }

  SILC_LOG_DEBUG(("Open directory '%s'", name));

  dir = silc_calloc(1, sizeof(*dir));
  if (!dir)
    return NULL;

  dir->name = silc_strdup(name);
  if (!dir->name) {
    silc_free(dir);
    return NULL;
  }

  if (dir->name[strlen(dir->name) - 1] == '/')
    dir->name[strlen(dir->name) - 1] = '\0';

  dir->dir = opendir(name);
  if (!dir->dir) {
    silc_set_errno_posix(errno);
    silc_free(dir->name);
    silc_free(dir);
    return NULL;
  }

  return dir;
}

/* Close directory */

void silc_dir_close(SilcDir dir)
{
  if (!dir)
    return;

  SILC_LOG_DEBUG(("Close directory '%s'", dir->name));

  closedir(dir->dir);
  silc_free(dir->name);
  silc_free(dir);
}

/* Read next entry in the directory */

SilcDirEntry silc_dir_read(SilcDir dir, SilcFileStat status)
{
  if (!dir) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return NULL;
  }

  SILC_LOG_DEBUG(("Read directory '%s'", dir->name));

  dir->entry.entry = readdir(dir->dir);
  if (!dir->entry.entry) {
    if (errno)
      silc_set_errno_posix(errno);
    return NULL;
  }

  if (status) {
    char *name = NULL;
    silc_asprintf(&name, "%s/%s", dir->name, dir->entry.entry->d_name);
    if (name)
      silc_file_stat(name, TRUE, status);
    silc_free(name);
  }

  return (SilcDirEntry)&dir->entry;
}

/* Rewind directory */

void silc_dir_rewind(SilcDir dir)
{
  if (!dir)
    return;

  SILC_LOG_DEBUG(("Rewind directory '%s'", dir->name));

  rewinddir(dir->dir);
}

/* Return directory name */

const char *silc_dir_name(SilcDir dir)
{
  if (!dir) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return NULL;
  }

  return dir->name;
}

/* Return entry name */

const char *silc_dir_entry_name(SilcDirEntry entry)
{
  if (!entry) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return NULL;
  }

  return (const char *)entry->entry->d_name;
}
