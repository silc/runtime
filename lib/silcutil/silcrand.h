/*

  silcrand.h

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

/****h* silcutil/Random Number Interface
 *
 * DESCRIPTION
 *
 * Interface for generating pseudo-random numbers.  This random number
 * generator must not be used in applications needing cryptographically
 * strong random numbers.  For that purpose the SILC Crypto Toolkit should
 * be used.
 *
 * The implementation uses Mersenne Twister random number generator.
 *
 * The SILC Rand API is thread-safe.  Each thread can use the API at the
 * same time without affecting the random state of other threads.
 *
 * EXAMPLE
 *
 * silc_rand_seed(seed);
 * printf("Random number: %lu", silc_rand());
 *
 ***/

#ifndef SILCRAND_H
#define SILCRAND_H

/****f* silcutil/silc_rand_seed
 *
 * SYNOPSIS
 *
 *    void silc_rand_seed(SilcUInt32 seed);
 *
 * DESCRIPTION
 *
 *    Seed the random number generator with the value `seed'.  This should
 *    be called before starting to retrieve random numbers and the `seed'
 *    itself should be random or non-determinable.  It is also wise to reseed
 *    the random number generator periodically if random numbers are used
 *    a lot.
 *
 ***/
void silc_rand_seed(SilcUInt32 seed);

/****f* silcutil/silc_rand
 *
 * SYNOPSIS
 *
 *    SilcUInt32 silc_rand(void);
 *
 * DESCRIPTION
 *
 *    Returns 32-bit random number.
 *
 ***/
SilcUInt32 silc_rand(void);

/****f* silcutil/silc_rand64
 *
 * SYNOPSIS
 *
 *    SilcUInt64 silc_rand64(void)
 *
 * DESCRIPTION
 *
 *    Returns 64-bit random number.
 *
 ***/
SilcUInt64 silc_rand64(void);

#endif /* SILCRAND_H */
