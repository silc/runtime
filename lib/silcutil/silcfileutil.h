/*

  silcfileutil.h

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 1997 - 2008 Pekka Riikonen

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/

/****h* silcutil/File Util Interface
 *
 * DESCRIPTION
 *
 * The SILC File Util Interface is a small set of functions that provides a
 * portable access method to the filesystem.
 *
 ***/

#ifndef SILCFILEUTIL_H
#define SILCFILEUTIL_H

/****d* silcutil/SilcFileMode
 *
 * NAME
 *
 *    typedef enum { ... } SilcFileMode;
 *
 * DESCRIPTION
 *
 *    A file mode bits that specify the file's mode, type and protection
 *    in the SilcFileStat context.
 *
 * SOURCE
 */
typedef enum {
  /* Type */
  SILC_FILE_IFDIR     = 0x00000001,  /* Entry is directory */
  SILC_FILE_IFCHR     = 0x00000002,  /* Entry is character device */
  SILC_FILE_IFBLK     = 0x00000004,  /* Entry is block device */
  SILC_FILE_IFREG     = 0x00000008,  /* Entry is regular file */
  SILC_FILE_IFIFO     = 0x00000010,  /* Entry is FIFO */
  SILC_FILE_IFLNK     = 0x00000020,  /* Entry is symbolic link */
  SILC_FILE_IFSOCK    = 0x00000040,  /* Entry is socket */

  /* Protection */
  SILC_FILE_IRUSR     = 0x00000080,  /* Owner has read permission */
  SILC_FILE_IWUSR     = 0x00000100,  /* Owner has write permission */
  SILC_FILE_IXUSR     = 0x00000200,  /* Owner has execute permission */
  SILC_FILE_IRGRP     = 0x00000400,  /* Group has read permission */
  SILC_FILE_IWGRP     = 0x00000800,  /* Group has write permission */
  SILC_FILE_IXGRP     = 0x00001000,  /* Group has execute permission */
  SILC_FILE_IROTH     = 0x00002000,  /* Others have read permission */
  SILC_FILE_IWOTH     = 0x00004000,  /* Others have write permission */
  SILC_FILE_IXOTH     = 0x00008000,  /* Others have execute permission */
} SilcFileMode;
/***/

/****s* silcutil/SilcFileStat
 *
 * NAME
 *
 *    typedef struct SilcFileStatObject { ... } *SilcFileStat,
 *                                               SilcFileStatStruct;
 *
 * DESCRIPTION
 *
 *    The file entry status information structure.  The structure contains
 *    various information about a file.  The structure is filled by calling
 *    the silc_file_stat or silc_file_fstat functions.
 *
 * SOURCE
 */
typedef struct SilcFileStatObject {
  SilcTimeStruct last_access;		/* Time of last access */
  SilcTimeStruct last_mod;	        /* Time of last modification */
  SilcTimeStruct last_change;	        /* Time of last status change */
  SilcUInt64 size;			/* Entry size in bytes */
  SilcUInt32 uid;			/* Owner ID of the entry */
  SilcUInt32 gid;			/* Group owner ID of the entry */
  SilcUInt32 dev;			/* Entry device number */
  SilcUInt32 rdev;			/* Device number if special file */
  SilcUInt32 nlink;			/* Number of hard links */
  SilcFileMode mode;			/* Entry mode */
} *SilcFileStat, SilcFileStatStruct;
/***/

/* Prototypes */

/****f* silcutil/silc_file_open
 *
 * SYNOPSIS
 *
 *    int silc_file_open(const char *filename, int flags);
 *
 * DESCRIPTION
 *
 *    Opens a file indicated by the filename `filename' with flags indicated
 *    by `flags'.  The opening permission defaults to 0600.  The `flags'
 *    are defined in open(2).  Returns the opened file descriptor or -1 on
 *    error.
 *
 ***/
int silc_file_open(const char *filename, int flags);

/****f* silcutil/silc_file_open_mode
 *
 * SYNOPSIS
 *
 *    int silc_file_open_mode(const char *filename, int flags, int mode);
 *
 * DESCRIPTION
 *
 *    Opens a file indicated by the filename `filename' with flags indicated
 *    by `flags'.  The argument `mode' specifies the permissions to use in
 *    case a new file is created.  The `flags' are defined in open(2).
 *    Returns the opened file descriptor or -1 on error and sets silc_errno.
 *
 ***/
int silc_file_open_mode(const char *filename, int flags, int mode);

/****f* silcutil/silc_file_read
 *
 * SYNOPSIS
 *
 *    int silc_file_read(int fd, unsigned char *buf, SilcUInt32 buf_len);
 *
 * DESCRIPTION
 *
 *    Reads data from file descriptor `fd' to `buf'.  Returns the amount of
 *    bytes read, 0 on EOF or -1 on error and sets silc_errno.
 *
 ***/
int silc_file_read(int fd, unsigned char *buf, SilcUInt32 buf_len);

/****f* silcutil/silc_file_write
 *
 * SYNOPSIS
 *
 *    int silc_file_write(int fd, const char *buffer, SilcUInt32 len);
 *
 * DESCRIPTION
 *
 *    Writes `buffer' of length of `len' to file descriptor `fd'.  Returns
 *    the amount of bytes written, 0 on EOF or -1 on error and sets silc_errno.
 *
 ***/
int silc_file_write(int fd, const char *buffer, SilcUInt32 len);

/****f* silcutil/silc_file_close
 *
 * SYNOPSIS
 *
 *    int silc_file_close(int fd);
 *
 * DESCRIPTION
 *
 *    Closes file descriptor previously opened with silc_file_open().
 *    Returns 0 on success or -1 on error and sets silc_errno.
 *
 ***/
int silc_file_close(int fd);

/****f* silcutil/silc_file_set_nonblock
 *
 * SYNOPSIS
 *
 *    int silc_file_set_nonblock(int fd);
 *
 * DESCRIPTION
 *
 *    Sets the file descriptor to non-blocking mode.
 *
 ***/
int silc_file_set_nonblock(int fd);

/****f* silcutil/silc_file_readfile
 *
 * SYNOPSIS
 *
 *    char *silc_file_readfile(const char *filename, SilcUInt32 *return_len,
 *                             SilcStack stack);
 *
 * DESCRIPTION
 *
 *    Reads the content of `filename' to a buffer.  The allocated buffer is
 *    returned.  This does not NULL terminate the buffer but EOF terminate
 *    it.  The caller must replace the EOF with NULL if the buffer must be
 *    NULL terminated.
 *
 *    If the `return_len' pointer is not NULL, it's filled with the length of
 *    the file.  The returned length does not include the terminator.
 *
 *    If `stack' is non-NULL the returned buffer is allocated from `stack'.
 *    The allocation consumes `stack' so caller should push the stack before
 *    calling this function and pop it later.
 *
 ***/
char *silc_file_readfile(const char *filename, SilcUInt32 *return_len,
			 SilcStack stack);

/****f* silcutil/silc_file_writefile
 *
 * SYNOPSIS
 *
 *    int silc_file_writefile(const char *filename, const char *buffer,
 *                            SilcUInt32 len);
 *
 * DESCRIPTION
 *
 *    Writes a buffer to the file.  Returns 0 on success and -1 on error.
 *
 ***/
int silc_file_writefile(const char *filename, const char *buffer,
			SilcUInt32 len);

/****f* silcutil/silc_file_writefile_mode
 *
 * SYNOPSIS
 *
 *    int silc_file_writefile_mode(const char *filename, const char *buffer,
 *                                 SilcUInt32 len, int mode);
 *
 * DESCRIPTION
 *
 *    Writes a buffer to the file.  If the file is created the specified `mode'
 *    is set to the file.  Returns 0 on success and -1 on error.
 *
 ***/
int silc_file_writefile_mode(const char *filename, const char *buffer,
			     SilcUInt32 len, int mode);

/****f* silcutil/silc_file_size
 *
 * SYNOPSIS
 *
 *    SilcUInt64 silc_file_size(const char *filename);
 *
 * DESCRIPTION
 *
 *    Returns the size of `filename'.  Returns 0 on error and sets silc_errno.
 *    If silc_errno is not set the file size is 0 bytes if this returns 0.
 *
 ***/
SilcUInt64 silc_file_size(const char *filename);

/****f* silcutil/silc_file_fsize
 *
 * SYNOPSIS
 *
 *    SilcUInt64 silc_file_fsize(int fd);
 *
 * DESCRIPTION
 *
 *    Returns the size of the file indicated by open file descriptor `fd'.
 *    Returns 0 on error and sets silc_errno.  If silc_errno is not set the
 *    file size is 0 bytes if this returns 0.
 *
 ***/
SilcUInt64 silc_file_fsize(int fd);

/****f* silcutil/silc_file_stat
 *
 * SYNOPSIS
 *
 *    SilcBool silc_file_stat(const char *filename, SilcBool follow_symlinks,
 *                            SilcFileStat return_stat);
 *
 * DESCRIPTION
 *
 *    Returns status information of a file named `filename'.  The status
 *    information is returned to `return_stat' structure.  If the
 *    `follow_symlinks' is TRUE this will return the status of the file the
 *    symlink referts to.  If it is FALSE, returns the status of the link
 *    itself.
 *
 *    Returns FALSE on error and sets the silc_errno.  Returns TRUE otherwise.
 *
 ***/
SilcBool silc_file_stat(const char *filename, SilcBool follow_symlinks,
			SilcFileStat return_stat);

/****f* silcutil/silc_file_fstat
 *
 * SYNOPSIS
 *
 *    SilcBool silc_file_fstat(int fd, SilcFileStat return_stat);
 *
 * DESCRIPTION
 *
 *    Returns status information of a opened file indicated by the file
 *    descriptor `fd'.  The status information is returned to the
 *    `return_stat' structure.
 *
 *    Returns FALSE on error and sets the silc_errno.  Returns TRUE otherwise.
 *
 ***/
SilcBool silc_file_fstat(int fd, SilcFileStat return_stat);

#endif	/* !SILCFILEUTIL_H */
