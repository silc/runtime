/*

  silcfileutil.c

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

#include "silcruntime.h"

/* Opens a file indicated by the filename `filename' with flags indicated
   by the `flags'. */

int silc_file_open(const char *filename, int flags)
{
  return silc_file_open_mode(filename, flags, 0600);
}

/* Opens a file indicated by the filename `filename' with flags indicated
   by the `flags', and with the specified `mode'. */

int silc_file_open_mode(const char *filename, int flags, int mode)
{
  int fd = open(filename, flags, mode);
  if (fd < 0)
    silc_set_errno_posix(errno);
  return fd;
}

/* Reads data from file descriptor `fd' to `buf'. */

int silc_file_read(int fd, unsigned char *buf, SilcUInt32 buf_len)
{
  int ret = read(fd, (void *)buf, buf_len);
  if (ret < 0)
    silc_set_errno_posix(errno);
  return ret;
}

/* Writes `buffer' of length of `len' to file descriptor `fd'. */

int silc_file_write(int fd, const char *buffer, SilcUInt32 len)
{
  int ret = write(fd, (const void *)buffer, len);
  if (ret < 0)
    silc_set_errno_posix(errno);
  return ret;
}

/* Closes file descriptor */

int silc_file_close(int fd)
{
  int ret = close(fd);
  if (ret < 0)
    silc_set_errno_posix(errno);
  return ret;
}

/* Writes a buffer to the file. */

int silc_file_writefile(const char *filename, const char *buffer,
			SilcUInt32 len)
{
  int fd;
  int flags = O_CREAT | O_WRONLY | O_TRUNC;

#if defined(O_BINARY)
  flags |= O_BINARY;
#endif /* O_BINARY */

  if ((fd = open(filename, flags, 0644)) == -1) {
    SILC_LOG_ERROR(("Cannot open file %s for writing: %s", filename,
		    silc_errno_string(silc_errno)));
    return -1;
  }

  if (silc_file_write(fd, buffer, len) == -1) {
    SILC_LOG_ERROR(("Cannot write to file %s: %s", filename,
		    silc_errno_string(silc_errno)));
    silc_file_close(fd);
    return -1;
  }

#ifdef SILC_UNIX
  fsync(fd);
#endif /* SILC_UNIX */

  return silc_file_close(fd);
}

/* Writes a buffer to the file.  If the file is created specific mode is
   set to the file. */

int silc_file_writefile_mode(const char *filename, const char *buffer,
			     SilcUInt32 len, int mode)
{
  int fd;
  int flags = O_CREAT | O_WRONLY | O_TRUNC;

#if defined(O_BINARY)
  flags |= O_BINARY;
#endif /* O_BINARY */

  if ((fd = open(filename, flags, mode)) == -1) {
    SILC_LOG_ERROR(("Cannot open file %s for writing: %s", filename,
		    silc_errno_string(silc_errno)));
    return -1;
  }

  if ((silc_file_write(fd, buffer, len)) == -1) {
    SILC_LOG_ERROR(("Cannot write to file %s: %s", filename,
		    silc_errno_string(silc_errno)));
    silc_file_close(fd);
    return -1;
  }

#ifdef SILC_UNIX
  fsync(fd);
#endif /* SILC_UNIX */

  return silc_file_close(fd);
}

/* Reads a file to a buffer. The allocated buffer is returned. Length of
   the file read is returned to the return_len argument. */

char *silc_file_readfile(const char *filename, SilcUInt32 *return_len,
			 SilcStack stack)
{
  int fd;
  unsigned char *buffer;
  int filelen;

  fd = silc_file_open(filename, O_RDONLY);
  if (fd < 0) {
    if (silc_errno == SILC_ERR_NO_SUCH_FILE)
      return NULL;
    SILC_LOG_ERROR(("Cannot open file %s: %s", filename,
		    silc_errno_string(silc_errno)));
    return NULL;
  }

  filelen = lseek(fd, (off_t)0L, SEEK_END);
  if (filelen < 0) {
    silc_set_errno_posix(errno);
    silc_file_close(fd);
    return NULL;
  }
  if (lseek(fd, (off_t)0L, SEEK_SET) < 0) {
    silc_set_errno_posix(errno);
    silc_file_close(fd);
    return NULL;
  }

  buffer = silc_calloc(filelen + 1, sizeof(*buffer));
  if (!buffer) {
    silc_set_errno_posix(errno);
    silc_file_close(fd);
    return NULL;
  }

  if ((silc_file_read(fd, buffer, filelen)) == -1) {
    memset(buffer, 0, sizeof(buffer));
    silc_file_close(fd);
    SILC_LOG_ERROR(("Cannot read from file %s: %s", filename,
		    silc_errno_string(silc_errno)));
    return NULL;
  }

  silc_file_close(fd);
  buffer[filelen] = EOF;

  if (return_len)
    *return_len = filelen;

  return (char *)buffer;
}

/* Returns the size of `filename'. */

SilcUInt64 silc_file_size(const char *filename)
{
  SilcFileStatStruct status;

  if (!silc_file_stat(filename, FALSE, &status))
    return 0;

  return status.size;
}

/* Return file size */

SilcUInt64 silc_file_fsize(int fd)
{
  SilcFileStatStruct status;

  if (!silc_file_fstat(fd, &status))
    return 0;

  return status.size;
}

/* Fill file status context */

static void silc_file_fill_stat(struct stat *status,
				SilcFileStat return_stat)
{
  memset(return_stat, 0, sizeof(*return_stat));

  silc_time_value(status->st_atime * 1000, &return_stat->last_access);
  silc_time_value(status->st_mtime * 1000, &return_stat->last_mod);
  silc_time_value(status->st_ctime * 1000, &return_stat->last_change);

  return_stat->rdev = status->st_rdev;
  return_stat->dev = status->st_dev;
  return_stat->nlink = status->st_nlink;
  return_stat->gid = status->st_gid;
  return_stat->uid = status->st_uid;
  return_stat->size = status->st_size;

#if defined(S_IFSOCK)
  if (status->st_mode & S_IFSOCK)
    return_stat->mode |= SILC_FILE_IFSOCK;
#endif /* S_IFSOCK */
#if defined(S_IFLNK)
  if (status->st_mode & S_IFLNK)
    return_stat->mode |= SILC_FILE_IFLNK;
#endif /* S_IFLNK */
#if defined(S_IFREG)
  if (status->st_mode & S_IFREG)
    return_stat->mode |= SILC_FILE_IFREG;
#endif /* S_IFREG */
#if defined(S_IFBLK)
  if (status->st_mode & S_IFBLK)
    return_stat->mode |= SILC_FILE_IFBLK;
#endif /* S_IFBLK */
#if defined(S_IFDIR)
  if (status->st_mode & S_IFDIR)
    return_stat->mode |= SILC_FILE_IFDIR;
#endif /* S_IFDIR */
#if defined(S_IFCHR)
  if (status->st_mode & S_IFCHR)
    return_stat->mode |= SILC_FILE_IFCHR;
#endif /* S_IFCHR */
#if defined(S_IFIFO)
  if (status->st_mode & S_IFIFO)
    return_stat->mode |= SILC_FILE_IFIFO;
#endif /* S_IFIFO */
#if defined(S_IRUSR)
  if (status->st_mode & S_IRUSR)
    return_stat->mode |= SILC_FILE_IRUSR;
#endif /* S_IRUSR */
#if defined(S_IWUSR)
  if (status->st_mode & S_IWUSR)
    return_stat->mode |= SILC_FILE_IWUSR;
#endif /* S_IWUSR */
#if defined(S_IXUSR)
  if (status->st_mode & S_IXUSR)
    return_stat->mode |= SILC_FILE_IXUSR;
#endif /* S_IXUSR */
#if defined(S_IRGRP)
  if (status->st_mode & S_IRGRP)
    return_stat->mode |= SILC_FILE_IRGRP;
#endif /* S_IRGRP */
#if defined(S_IWGRP)
  if (status->st_mode & S_IWGRP)
    return_stat->mode |= SILC_FILE_IWGRP;
#endif /* S_IWGRP */
#if defined(S_IXGRP)
  if (status->st_mode & S_IXGRP)
    return_stat->mode |= SILC_FILE_IXGRP;
#endif /* S_IXGRP */
#if defined(S_IROTH)
  if (status->st_mode & S_IROTH)
    return_stat->mode |= SILC_FILE_IROTH;
#endif /* S_IROTH */
#if defined(S_IWOTH)
  if (status->st_mode & S_IWOTH)
    return_stat->mode |= SILC_FILE_IWOTH;
#endif /* S_IWOTH */
#if defined(S_IXOTH)
  if (status->st_mode & S_IXOTH)
    return_stat->mode |= SILC_FILE_IXOTH;
#endif /* S_IXOTH */
}

/* Return file status information */

SilcBool silc_file_stat(const char *filename, SilcBool follow_symlinks,
			SilcFileStat return_stat)
{
  struct stat status;

  if (silc_unlikely(!filename)) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return FALSE;
  }

  SILC_LOG_DEBUG(("Get status for file '%s'", filename));

  if (!follow_symlinks) {
    if (silc_unlikely(stat(filename, &status) != 0)) {
      silc_set_errno_posix(errno);
      return FALSE;
    }
  } else {
#ifdef HAVE_LSTAT
    if (silc_unlikely(lstat(filename, &status) != 0)) {
      silc_set_errno_posix(errno);
      return FALSE;
    }
#else
    if (silc_unlikely(stat(filename, &status) != 0)) {
      silc_set_errno_posix(errno);
      return FALSE;
    }
#endif /* HAVE_LSTAT */
  }

  if (return_stat)
    silc_file_fill_stat(&status, return_stat);

  return TRUE;
}

/* Return file status information. */

SilcBool silc_file_fstat(int fd, SilcFileStat return_stat)
{
  struct stat status;

  if (silc_unlikely(fstat(fd, &status) != 0)) {
    silc_set_errno_posix(errno);
    return FALSE;
  }

  if (return_stat)
    silc_file_fill_stat(&status, return_stat);

  return TRUE;
}
