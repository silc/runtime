/* Global storage tests */

#include "silcruntime.h"

int main(int argc, char **argv)
{
  SilcBool success = FALSE;
  unsigned char *vbuf, *vbuf2;
  unsigned char init[20];
  SilcUInt32 gint, *gintptr;

  if (argc > 1 && !strcmp(argv[1], "-d")) {
    silc_log_debug(TRUE);
    silc_log_quick(TRUE);
    silc_log_debug_hexdump(TRUE);
    silc_log_set_debug_string("*global*");
  }

  SILC_LOG_DEBUG(("Set global var"));
  gint = 100;
  if (!silc_global_set_var("gint", 4, &gint, FALSE))
    goto err;

  SILC_LOG_DEBUG(("Retrieve var"));
  gintptr = silc_global_get_var("gint", FALSE);
  if (!gintptr)
    goto err;
  if (*gintptr != 100)
    goto err;

  SILC_LOG_DEBUG(("Set global var"));
  if (!silc_global_set_var("vbuf", 10, NULL, FALSE))
    goto err;

  SILC_LOG_DEBUG(("Retrieve var"));
  vbuf = silc_global_get_var("vbuf", FALSE);
  if (!vbuf)
    goto err;

  SILC_LOG_DEBUG(("Change value"));
  memset(vbuf, 'F', 10);

  SILC_LOG_DEBUG(("Retrieve var"));
  vbuf = silc_global_get_var("vbuf", FALSE);
  if (!vbuf)
    goto err;
  if (vbuf[0] != 'F')
    goto err;

  SILC_LOG_DEBUG(("Retrieve var (must not find)"));
  vbuf = silc_global_get_var("vbuf2", FALSE);
  if (vbuf)
    goto err;
  SILC_LOG_DEBUG(("Retrieve var (must not find)"));
  vbuf = silc_global_get_var("VBUF", FALSE);
  if (vbuf)
    goto err;
  SILC_LOG_DEBUG(("Retrieve var (must not find)"));
  vbuf = silc_global_get_var("vbuf", TRUE);
  if (vbuf)
    goto err;

  SILC_LOG_DEBUG(("Reset same var"));
  if (!silc_global_set_var("vbuf", 20, NULL, FALSE))
    goto err;
  
  SILC_LOG_DEBUG(("Retrieve var"));
  vbuf = silc_global_get_var("vbuf", FALSE);
  if (!vbuf)
    goto err;

  SILC_LOG_DEBUG(("Change value"));
  memset(vbuf, 'F', 20);

  SILC_LOG_DEBUG(("Retrieve var"));
  vbuf = silc_global_get_var("vbuf", FALSE);
  if (!vbuf)
    goto err;
  if (vbuf[19] != 'F')
    goto err;

  SILC_LOG_DEBUG(("Reset Tls var with initial value"));
  memset(init, 'D', 20);
  if (!silc_global_set_var("vbuf", 20, init, TRUE))
    goto err;
  
  SILC_LOG_DEBUG(("Retrieve var"));
  vbuf2 = silc_global_get_var("vbuf", TRUE);
  if (vbuf == vbuf2)
    goto err;
  if (!vbuf2)
    goto err;
  if (vbuf2[19] != 'D')
    goto err;

  SILC_LOG_DEBUG(("Change value"));
  memset(vbuf2, 'T', 20);

  SILC_LOG_DEBUG(("Retrieve Tls var"));
  vbuf = silc_global_get_var("vbuf", TRUE);
  if (!vbuf)
    goto err;
  if (vbuf[0] != 'T')
    goto err;
 
  SILC_LOG_DEBUG(("Retrieve global var"));
  vbuf = silc_global_get_var("vbuf", FALSE);
  if (!vbuf)
    goto err;
  if (vbuf[19] != 'F')
    goto err;

  SILC_LOG_DEBUG(("Delete global var"));
  silc_global_del_var("vbuf", FALSE);
  SILC_LOG_DEBUG(("Retrieve var (must not find)"));
  vbuf = silc_global_get_var("vbuf", FALSE);
  if (vbuf)
    goto err;

  SILC_LOG_DEBUG(("Delete Tls var"));
  silc_global_del_var("vbuf", TRUE);
  SILC_LOG_DEBUG(("Retrieve var (must not find)"));
  vbuf = silc_global_get_var("vbuf", TRUE);
  if (vbuf)
    goto err;

  success = TRUE;

 err:
  SILC_LOG_DEBUG(("Testing was %s", success ? "SUCCESS" : "FAILURE"));
  fprintf(stderr, "Testing was %s\n", success ? "SUCCESS" : "FAILURE");

  return !success;
}
