/*

  silcrand.c

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

/************************* Types and definitions ****************************/

/* Random number state context size */
#define SILC_RAND_STATE_SIZE 624
#define SILC_RAND_STATE_FULL_SIZE ((SILC_RAND_STATE_SIZE + 1) *	\
				   sizeof(SilcUInt32))

/* State position offset */
#define SILC_RAND_STATE_POS (SILC_RAND_STATE_SIZE + 1) - 1

/************************ Static utility functions **************************/

/* Refresh the state */

void silc_rand_refresh(SilcUInt32 *rs)
{
  SilcUInt32 i, c;

  for (i = 0; i < SILC_RAND_STATE_SIZE - 1; i++) {
    c = (rs[i] & 0x80000000UL) | (rs[i + 1] & 0x7fffffffUL);
    if (c & 1)
      rs[i] = rs[(i + 397) % SILC_RAND_STATE_SIZE] ^ (c >> 1) ^ 0x9908b0dfUL;
    else
      rs[i] = rs[(i + 397) % SILC_RAND_STATE_SIZE] ^ (c >> 1);
  }

  c = (rs[i] & 0x80000000UL) | (rs[0] & 0x7fffffffUL);
  if (c & 1)
    rs[i] = rs[(i + 397) % SILC_RAND_STATE_SIZE] ^ (c >> 1) ^ 0x9908b0dfUL;
  else
    rs[i] = rs[(i + 397) % SILC_RAND_STATE_SIZE] ^ (c >> 1);

  rs[SILC_RAND_STATE_POS] = 0;
}

/* Seed */

static void silc_rand_seed_state(SilcUInt32 *rs, SilcUInt32 seed)
{
  SilcUInt32 c, i;

  rs[0] = seed;
  for (i = 1; i < SILC_RAND_STATE_SIZE; i++) {
    c = rs[i - 1];
    rs[i] = (1812433253UL * ((c ^ (c >> 30)) + 1));
  }

  silc_rand_refresh(rs);
}

/* Return random state or create it. */

static SilcUInt32 *silc_rand_state(SilcBool seed)
{
  SilcUInt32 *rs;

  rs = silc_global_get_var("srtrs", TRUE);
  if (!rs) {
    rs = silc_global_set_var("srtrs", SILC_RAND_STATE_FULL_SIZE, NULL, TRUE);
    if (!rs)
      return NULL;

    if (seed)
      silc_rand_seed_state(rs, (SilcUInt32)silc_time_usec());
  }

  return rs;
}

/* Temper next position and return the value */

SilcUInt32 silc_rand_temper(SilcUInt32 *rs)
{
  SilcUInt32 val;

  /* Refresh if necessary */
  if (++rs[SILC_RAND_STATE_POS] >= SILC_RAND_STATE_SIZE)
    silc_rand_refresh(rs);

  /* Temper */
  val = rs[rs[SILC_RAND_STATE_POS]];
  val = val ^ (val >> 11);
  val = val ^ ((val << 7) & 0x9d2c5680UL);
  val = val ^ ((val << 15) & 0xefc60000UL);
  return val ^ (val >> 18);
}

/******************************* SILC Rand API ******************************/

/* Seed the generator */

void silc_rand_seed(SilcUInt32 seed)
{
  SilcUInt32 *rs;

  rs = silc_rand_state(FALSE);
  if (!rs)
    return;

  silc_rand_seed_state(rs, seed);
}

/* Return 32-bit random number */

SilcUInt32 silc_rand(void)
{
  SilcUInt32 *rs;

  rs = silc_rand_state(TRUE);
  if (!rs)
    return 0x23456789 + (SilcUInt32)silc_time_usec();

  return silc_rand_temper(rs);
}

/* Return 64-bit random number */

SilcUInt64 silc_rand64(void)
{
  SilcUInt32 *rs;

  rs = silc_rand_state(TRUE);
  if (!rs)
    return 0x1234567891234567 + silc_time_usec();

  return (SilcUInt64)(((SilcUInt64)silc_rand_temper(rs) << 32) |
		      silc_rand_temper(rs));
}
