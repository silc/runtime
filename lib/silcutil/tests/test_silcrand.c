/* PRNG tests */

#include "silcruntime.h"

int main(int argc, char **argv)
{
  SilcBool success = FALSE;
  SilcUInt32 num, seen;
  SilcUInt64 lnum;
  int i;

  silc_runtime_init();

  if (argc > 1 && !strcmp(argv[1], "-d")) {
    silc_log_debug(TRUE);
    silc_log_quick(TRUE);
    silc_log_debug_hexdump(TRUE);
    silc_log_set_debug_string("*rand*");
  }

  num = silc_rand();
  SILC_LOG_DEBUG(("Number: %lu", num));

  SILC_LOG_DEBUG(("Seed RNG"));
  silc_rand_seed(1);

  for (i = 0; i < 1000; i++) {
    num = silc_rand();
    SILC_LOG_DEBUG(("Number: %lu", num));
  }

  seen = silc_rand();
  for (i = 0; i < 10000000; i++) {
    num = silc_rand();
    if (num == seen)
      goto err;
  }

  for (i = 0; i < 1000; i++) {
    if (!(i % 100))
      silc_rand_seed((SilcUInt32)lnum);
    lnum = silc_rand64();
    SILC_LOG_DEBUG(("Number: %p", lnum));
  }

  success = TRUE;

 err:
  SILC_LOG_DEBUG(("Testing was %s", success ? "SUCCESS" : "FAILURE"));
  fprintf(stderr, "Testing was %s\n", success ? "SUCCESS" : "FAILURE");

  silc_runtime_uninit();

  return !success;
}
