/*

  silcglobal.h

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

/****h* silcutil/Global Variable Interface
 *
 * DESCRIPTION
 *
 * The Global Variable API can be used to set variables as global in the
 * process or in current thread.  On some platforms, like Symbian, global
 * variables cannot be used directly.  This API can be used to write
 * portable code that can use global variables on all supported platforms.
 *
 * EXAMPLE
 *
 * // Initialize global buffer
 * unsigned char buffer[256];
 * silc_global_set_va("somebuf", sizeof(buffer), NULL, FALSE);
 *
 * // Retrieve the buffer
 * unsigned char *buf = silc_global_get_var("somebuf", FALSE);
 *
 * // Set integer and its value global in the current thread
 * SilcUInt32 integer = 100;
 * silc_global_set_var("someint", 4, &integer, TRUE);
 *
 * // Retrieve the integer and change its value
 * SilcUInt32 *intptr = silc_global_get_var("someint", TRUE);
 * *intptr = 200;
 *
 ***/

#ifndef SILCGLOBAL_H
#define SILCGLOBAL_H

/****f* silcutil/silc_global_set_var
 *
 * SYNOPSIS
 *
 *    void *silc_global_set_var(const char *name, SilcUInt32 variable_size,
 *                              void *initial_value, SilcBool tls);
 *
 * DESCRIPTION
 *
 *    Sets a variable of size of `variable_size' bytes as a global variable
 *    under the name of `name'.  If `initial_value' is non-NULL it is
 *    considered to be the initial value of the stored variable.  If it is
 *    NULL this will initialize the variable as all zeroes.
 *
 *    If variable with `name' already exists it will be replaced with the
 *    `initial_value' or as all zeroes, and the old pointer will become
 *    invalid.
 *
 *    If `tls' is FALSE the variable is visible to all threads in the process.
 *    If it is TRUE the variable is visible only in the current thread.  The
 *    variable can be retrieved using the same name by calling the
 *    silc_thread_global_get_var.
 *
 *    Returns NULL and sets silc_errno if the variable could not be added, or
 *    a pointer to the added variable otherwise.
 *
 * EXAMPLE
 *
 *    // Initialize global buffer
 *    unsigned char buffer[256];
 *    silc_global_set_va("somebuf", sizeof(buffer), NULL, FALSE);
 *
 *    // Retrieve the buffer
 *    unsigned char *buf = silc_global_get_var("somebuf", FALSE);
 *
 *    // Set integer global
 *    SilcUInt32 integer = 100;
 *    silc_global_set_var("someint", 4, &integer, FALSE);
 *
 *    // Retrieve the integer and change its value
 *    SilcUInt32 *intptr = silc_global_get_var("someint", FALSE);
 *    *intptr = 200;
 *
 *    // Set structure as global in the thread
 *    silc_global_set_va("somestruct", sizeof(*context), NULL, TRUE);
 *
 *    // Retrieve the context
 *    context = silc_global_get_var("somestruct", TRUE);
 *
 ***/
void *silc_global_set_var(const char *name, SilcUInt32 variable_size,
			  void *initial_value, SilcBool tls);

/****f* silcutil/silc_global_get_var
 *
 * SYNOPSIS
 *
 *    void *silc_global_get_var(const char *name, SilcBool tls);
 *
 * DESCRIPTION
 *
 *    Returns the variable from the global variable storage that has been
 *    stored under the name `name'.  If `tls' is FALSE this fetches the
 *    variable from global storage, if it is TRUE it is fetched from the
 *    Thread-local storage.  Returns NULL if such variable does not exist
 *    and sets silc_errno.
 *
 ***/
void *silc_global_get_var(const char *name, SilcBool tls);

/****f* silcutil/silc_global_del_var
 *
 * SYNOPSIS
 *
 *    SilcBool silc_global_del_var(const char *name, SilcBool tls);
 *
 * DESCRIPTION
 *
 *    Deletes the global variable named `name'.  Returns TRUE if it was
 *    deleted, FALSE if such variable does not exist and sets silc_errno.
 *
 *    If variable is not deleted before the process or thread is destroyed
 *    it will be deleted and freed automatically.
 *
 ***/
SilcBool silc_global_del_var(const char *name, SilcBool tls);

#endif /* SILCGLOBAL_H */
