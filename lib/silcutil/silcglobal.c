/*

  silcglobal.c

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

/****************************** Global Storage ******************************/

/* Set global variable */

void *silc_global_set_var(const char *name, SilcUInt32 variable_size,
			  void *initial_value, SilcBool in_tls)
{
  SilcTls tls = silc_thread_get_tls();
  SilcBool ret;
  char *var_name;
  void *var;

  SILC_LOG_DEBUG(("Adding %s variable '%s' of %d bytes",
		  in_tls ? "Tls" : "global", name, variable_size));

  if (!variable_size) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return NULL;
  }

  if (!tls) {
    /* Initialize Tls for this thread */
    tls = silc_thread_tls_init();
    if (!tls)
      return NULL;
  }

  if (!in_tls) {
    if (!tls->variables) {
      tls->variables = silc_hash_table_alloc(NULL, 0,
					     silc_hash_string, NULL,
					     silc_hash_string_compare, NULL,
					     silc_hash_destructor, NULL,
					     TRUE);
      if (!tls->variables)
	return NULL;
    }
  } else {
    if (!tls->tls_variables) {
      tls->tls_variables = silc_hash_table_alloc(NULL, 0,
						 silc_hash_string, NULL,
						 silc_hash_string_compare, NULL,
						 silc_hash_destructor, NULL,
						 TRUE);
      if (!tls->tls_variables)
	return NULL;
    }
  }

  /* Allocate the variable */
  var = silc_malloc(variable_size);
  if (!var)
    return NULL;

  /* Initialize the variable */
  if (initial_value)
    memcpy(var, initial_value, variable_size);
  else
    memset(var, 0, variable_size);

  var_name = silc_strdup(name);
  if (!var_name) {
    silc_free(var);
    return NULL;
  }

  /* Add the variable */
  if (!in_tls) {
    silc_mutex_lock(tls->lock);
    ret = silc_hash_table_set(tls->variables, var_name, var);
    silc_mutex_unlock(tls->lock);
  } else {
    ret = silc_hash_table_set(tls->tls_variables, var_name, var);
  }

  if (!ret) {
    silc_free(var);
    silc_free(var_name);
    var = NULL;
  }

  return var;
}

/* Get global variable */

void *silc_global_get_var(const char *name, SilcBool in_tls)
{
  SilcTls tls = silc_thread_get_tls();
  void *variable;
  SilcBool ret;

  SILC_LOG_DEBUG(("Find %s variable named '%s'", in_tls ? "Tls" : "global",
		  name));

  if (!tls) {
    silc_set_errno(SILC_ERR_NOT_FOUND);
    return NULL;
  }

  if (!in_tls && !tls->variables) {
    silc_set_errno(SILC_ERR_NOT_FOUND);
    return NULL;
  }

  if (in_tls && !tls->tls_variables) {
    silc_set_errno(SILC_ERR_NOT_FOUND);
    return NULL;
  }

  if (!in_tls) {
    silc_mutex_lock(tls->lock);
    ret = silc_hash_table_find(tls->variables, (void *)name, NULL, &variable);
    silc_mutex_unlock(tls->lock);
  } else {
    ret = silc_hash_table_find(tls->tls_variables, (void *)name, NULL,
			       &variable);
  }

  if (!ret)
    return NULL;

  return variable;
}

/* Delete global variable */

SilcBool silc_global_del_var(const char *name, SilcBool in_tls)
{
  SilcTls tls = silc_thread_get_tls();

  SILC_LOG_DEBUG(("Delete %s variable '%s'", in_tls ? "Tls" : "global", name));

  if (!tls) {
    silc_set_errno(SILC_ERR_NOT_FOUND);
    return FALSE;
  }

  if (!in_tls && !tls->variables) {
    silc_set_errno(SILC_ERR_NOT_FOUND);
    return FALSE;
  }

  if (in_tls && !tls->tls_variables) {
    silc_set_errno(SILC_ERR_NOT_FOUND);
    return FALSE;
  }

  return silc_hash_table_del(in_tls ? tls->tls_variables : tls->variables,
			     (void *)name);
}
