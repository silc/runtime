/* CPUID tests */

#include "silcruntime.h"

int main(int argc, char **argv)
{
  SilcBool success = FALSE;
  SilcCPUIdFeatures id;

  if (argc > 1 && !strcmp(argv[1], "-d")) {
    silc_log_debug(TRUE);
    silc_log_quick(TRUE);
    silc_log_debug_hexdump(TRUE);
    silc_log_set_debug_string("*cpu*");
  }

  id = silc_cpuid_features();

  success = TRUE;

// err:
  SILC_LOG_DEBUG(("Testing was %s", success ? "SUCCESS" : "FAILURE"));
  fprintf(stderr, "Testing was %s\n", success ? "SUCCESS" : "FAILURE");

  return !success;
}
