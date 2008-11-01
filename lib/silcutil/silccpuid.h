/*

  silccpuid.h

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

#ifndef SILCCPUID_H
#define SILCCPUID_H

/****d* silcutil/SilcCPUIdFeatures
 *
 * NAME
 *
 *    typedef enum { ... } SilcCPUIdFeatures;
 *
 * DESCRIPTION
 *
 *    CPU features.  Indicates which instruction sets the CPU supports.
 *
 */
typedef enum {
  SILC_CPUID_UNKNOWN     = 0x00000000,           /* 386 only */
  SILC_CPUID_386         = 0x00000002,           /* 386 */
  SILC_CPUID_MMX         = 0x00000004,		 /* MMX */
  SILC_CPUID_CMOV        = 0x00000008,		 /* CMOV, FCOMI */
  SILC_CPUID_SSE         = 0x00000010,		 /* SSE */
  SILC_CPUID_SSE2        = 0x00000020,		 /* SSE2 */
  SILC_CPUID_SSE3        = 0x00000040,		 /* SSE3 */
  SILC_CPUID_SSSE3       = 0x00000080,		 /* SSSE3 */
  SILC_CPUID_SSE41       = 0x00000100,		 /* SSE4.1 */
  SILC_CPUID_SSE42       = 0x00000200,		 /* SSE4.2 */
  SILC_CPUID_SSE4A       = 0x00000400,		 /* SSE4a */
  SILC_CPUID_SSE5        = 0x00000800,		 /* SSE5 */
  SILC_CPUID_AVX         = 0x00001000,		 /* AVX */
  SILC_CPUID_AES         = 0x00002000,		 /* AES */
} SilcCPUIdFeatures;
/***/

/****f* silcutil/silc_cpuid_features
 *
 * SYNOPSIS
 *
 *    SilcCPUIdFeatures silc_cpuid_features(void);
 *
 * DESCRIPTION
 *
 *    Returns the CPU instruction set features.  The ID can be used to
 *    determine what features the CPU supports.  The returned value is a
 *    bitmask of supported features.
 *
 * NOTES
 *
 *    This function should not be called at a code path where speed is
 *    critical.  It is recommended to call this once and later use the
 *    returned value to determine features.
 *
 *    This function works only on x86 or x86-64 CPUs.
 *
 ***/
SilcCPUIdFeatures silc_cpuid_features(void);

#endif /* SILCCPUID_H */
