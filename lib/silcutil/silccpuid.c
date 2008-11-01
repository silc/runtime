/*

  silccpuid.c

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

#include <silcruntime.h>

#if defined(SILC_I486) || defined(SILC_X86_64)
static void silc_cpuid(SilcUInt32 level, SilcUInt32 *ecx, SilcUInt32 *edx)
{
  /* This works on modern CPUs.  Probably fails on some ancient CPUs. */
  asm volatile ("xorl %%ecx, %%ecx   \n"
		"xorl %%edx, %%edx   \n"
		"movl %2, %%eax      \n"
		"cpuid               \n"
		"movl %%edx, %0      \n"
		"movl %%ecx, %1      \n"
		: "=m" (*edx), "=m" (*ecx)
		: "r" (level)
		: "ebx", "eax", "ecx", "edx");

  SILC_LOG_DEBUG(("CPUID 0x%08x: ecx=0x%08x, edx=0x%08x", level, *ecx, *edx));
}
#endif /* SILC_I486 || SILC_X86_64 */

SilcCPUIdFeatures silc_cpuid_features(void)
{
  SilcCPUIdFeatures id = SILC_CPUID_UNKNOWN;
#if defined(SILC_I486) || defined(SILC_X86_64)
  SilcUInt32 ecx = 0, edx = 0;

  /* Basic level */
  silc_cpuid(1, &ecx, &edx);
  if (!edx)
    return SILC_CPUID_UNKNOWN;

  SILC_LOG_DEBUG(("386"));
  id = SILC_CPUID_386;

  if (edx & (1 << 23)) {
    SILC_LOG_DEBUG(("MMX"));
    id |= SILC_CPUID_MMX;
  }

  if (edx & (1 << 15)) {
    SILC_LOG_DEBUG(("CMOV"));
    id |= SILC_CPUID_CMOV;
  }

  if (edx & (1 << 25)) {
    SILC_LOG_DEBUG(("SSE"));
    id |= SILC_CPUID_SSE;
  }

  if (edx & (1 << 26)) {
    SILC_LOG_DEBUG(("SSE2"));
    id |= SILC_CPUID_SSE2;
  }

  if (ecx & (1 << 0)) {
    SILC_LOG_DEBUG(("SSE3"));
    id |= SILC_CPUID_SSE3;
  }

  if (ecx & (1 << 9)) {
    SILC_LOG_DEBUG(("SSSE3"));
    id |= SILC_CPUID_SSSE3;
  }

  if (ecx & (1 << 19)) {
    SILC_LOG_DEBUG(("SSE4.1"));
    id |= SILC_CPUID_SSE41;
  }

  if (ecx & (1 << 20)) {
    SILC_LOG_DEBUG(("SSE4.2"));
    id |= SILC_CPUID_SSE41;
  }

  if (ecx & (1 << 28)) {
    SILC_LOG_DEBUG(("AVX"));
    id |= SILC_CPUID_AVX;
  }

  if (ecx & (1 << 25)) {
    SILC_LOG_DEBUG(("AES"));
    id |= SILC_CPUID_AES;
  }

  /* Extended level */
  ecx = edx = 0;
  silc_cpuid(0x80000001, &ecx, &edx);
  if (!ecx)
    goto out;

  if (ecx & (1 << 6)) {
    SILC_LOG_DEBUG(("SSE4a"));
    id |= SILC_CPUID_SSE4A;
  }

  if (ecx & (1 << 25)) {
    SILC_LOG_DEBUG(("SSE5"));
    id |= SILC_CPUID_SSE5;
  }

 out:
#endif /* SILC_I486 || SILC_X86_64 */
  return id;
}
