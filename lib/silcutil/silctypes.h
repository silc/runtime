/*

  silctypes.h 

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 2002 Pekka Riikonen

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/

/****h* silcutil/SILC Types
 *
 * DESCRIPTION
 *
 *    This header includes the most basic types used in the SILC source
 *    tree, such as arithmetic types and their manipulation macros.  This
 *    file is included in the silcincludes.h and is automatically available
 *    for application.
 *
 ***/

#ifndef SILCTYPES_H
#define SILCTYPES_H

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* Define offsetof */
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#if SILC_SIZEOF_SHORT > 2
#error "size of the short must be 2 bytes"
#endif

/****d* silcutil/SILCTypes/SilcUInt8
 *
 * NAME
 *
 *    typedef unsigned char SilcUInt8;
 *
 * DESCRIPTION
 *
 *    8-bit unsigned integer.
 *
 * SOURCE
 */
typedef unsigned char SilcUInt8;
/***/

/****d* silcutil/SILCTypes/SilcInt8
 *
 * NAME
 *
 *    typedef signed char SilcInt8;
 *
 * DESCRIPTION
 *
 *    8-bit signed integer.
 *
 * SOURCE
 */
typedef signed char SilcInt8;
/***/

/****d* silcutil/SILCTypes/SilcUInt16
 *
 * NAME
 *
 *    typedef unsigned short SilcUInt16;
 *
 * DESCRIPTION
 *
 *    16-bit unsigned integer.  Guaranteed to be 16-bits.
 *
 * SOURCE
 */
typedef unsigned short SilcUInt16;
/***/

/****d* silcutil/SILCTypes/SilcInt16
 *
 * NAME
 *
 *    typedef signed short SilcInt16;
 *
 * DESCRIPTION
 *
 *    16-bit signed integer.  Guaranteed to be 16-bits.
 *
 * SOURCE
 */
typedef signed short SilcInt16;
/***/

/****d* silcutil/SILCTypes/SilcUInt32
 *
 * NAME
 *
 *    typedef unsigned long SilcUInt32;
 *
 * DESCRIPTION
 *
 *    32-bit unsigned integer.  Guaranteed to be 32-bits.
 *
 * SOURCE
 */
#if SILC_SIZEOF_LONG == 4
typedef unsigned long SilcUInt32;
typedef signed long SilcInt32;
#else
#if SILC_SIZEOF_INT == 4
typedef unsigned int SilcUInt32;
typedef signed int SilcInt32;
#else
#if SILC_SIZEOF_LONG_LONG >= 4
#ifndef WIN32
typedef unsigned long long SilcUInt32;
typedef signed long long SilcInt32;
#endif
#endif
#endif
#endif
/***/

/****d* silcutil/SILCTypes/SilcInt32
 *
 * NAME
 *
 *    typedef signed long SilcInt32;
 *
 * DESCRIPTION
 *
 *    32-bit signed integer.  Guaranteed to be 32-bits.
 *
 ***/

/****d* silcutil/SILCTypes/SilcUInt64
 *
 * NAME
 *
 *    typedef unsigned long long SilcUInt64;
 *
 * DESCRIPTION
 *
 *    64-bit unsigned integer.  Guaranteed to be 64-bits on systems that
 *    support it.
 *
 * SOURCE
 */
#if SILC_SIZEOF_LONG >= 8
typedef unsigned long SilcUInt64;
typedef signed long SilcInt64;
#else
#if SILC_SIZEOF_LONG_LONG >= 8
#ifndef WIN32
typedef unsigned long long SilcUInt64;
typedef signed long long SilcInt64;
#else
typedef SilcUInt32 SilcUInt64; /* XXX Use Windows's own 64 bit types */
typedef SilcInt32 SilcInt64;
#endif
#else
typedef SilcUInt32 SilcUInt64;
typedef SilcInt32 SilcInt64;
#endif
#endif
/***/

/****d* silcutil/SILCTypes/SilcInt64
 *
 * NAME
 *
 *    typedef signed long long SilcInt64;
 *
 * DESCRIPTION
 *
 *    64-bit signed integer.  Guaranteed to be 64-bits on systems that
 *    support it.
 *
 ***/

#if SILC_SIZEOF_VOID_P < 4
typedef SilcUInt32 * void *;
#endif

#ifndef __cplusplus
#ifndef bool
#define bool unsigned char
#endif
#endif

#define GET_WORD(cp) ((SilcUInt32)(SilcUInt8)(cp)[0]) << 24	\
		    | ((SilcUInt32)(SilcUInt8)(cp)[1] << 16)	\
		    | ((SilcUInt32)(SilcUInt8)(cp)[2] << 8)	\
		    | ((SilcUInt32)(SilcUInt8)(cp)[3])

/****d* silcutil/SILCTypes/SILC_GET16_MSB
 *
 * NAME
 *
 *    #define SILC_GET16_MSB ...
 *
 * DESCRIPTION
 *
 *    Return two 8-bit bytes, most significant bytes first.
 *
 * SOURCE
 */
#define SILC_GET16_MSB(l, cp)				\
	(l) = ((SilcUInt32)(SilcUInt8)(cp)[0] << 8)	\
	    | ((SilcUInt32)(SilcUInt8)(cp)[1])
/***/

/****d* silcutil/SILCTypes/SILC_PUT16_MSB
 *
 * NAME
 *
 *    #define SILC_PUT16_MSB ...
 *
 * DESCRIPTION
 *
 *    Put two 8-bit bytes, most significant bytes first.
 *
 * SOURCE
 */
#define SILC_PUT16_MSB(l, cp)			\
	(cp)[0] = l >> 8;			\
	(cp)[1] = l;
/***/

/****d* silcutil/SILCTypes/SILC_GET16_LSB
 *
 * NAME
 *
 *    #define SILC_GET16_MSB ...
 *
 * DESCRIPTION
 *
 *    Return two 8-bit bytes, least significant bytes first.
 *
 * SOURCE
 */
#define SILC_GET16_LSB(l, cp)				\
	(l) = ((SilcUInt32)(SilcUInt8)(cp)[0])		\
	    | ((SilcUInt32)(SilcUInt8)(cp)[1] << 8)
/***/

/****d* silcutil/SILCTypes/SILC_PUT16_LSB
 *
 * NAME
 *
 *    #define SILC_PUT16_LSB ...
 *
 * DESCRIPTION
 *
 *    Put two 8-bit bytes, least significant bytes first.
 *
 * SOURCE
 */
#define SILC_PUT16_LSB(l, cp)			\
	(cp)[0] = l;				\
	(cp)[1] = l >> 8;
/***/

/****d* silcutil/SILCTypes/SILC_GET32_MSB
 *
 * NAME
 *
 *    #define SILC_GET32_MSB ...
 *
 * DESCRIPTION
 *
 *    Return four 8-bit bytes, most significant bytes first.
 *
 * SOURCE
 */
#define SILC_GET32_MSB(l, cp)				\
	(l) = ((SilcUInt32)(SilcUInt8)(cp)[0]) << 24	\
	    | ((SilcUInt32)(SilcUInt8)(cp)[1] << 16)	\
	    | ((SilcUInt32)(SilcUInt8)(cp)[2] << 8)	\
	    | ((SilcUInt32)(SilcUInt8)(cp)[3])
/***/

/****d* silcutil/SILCTypes/SILC_PUT32_MSB
 *
 * NAME
 *
 *    #define SILC_PUT32_MSB ...
 *
 * DESCRIPTION
 *
 *    Put four 8-bit bytes, most significant bytes first.
 *
 * SOURCE
 */
#define SILC_PUT32_MSB(l, cp)			\
	(cp)[0] = l >> 24;			\
	(cp)[1] = l >> 16;			\
	(cp)[2] = l >> 8;			\
	(cp)[3] = l;
/***/

/****d* silcutil/SILCTypes/SILC_GET32_LSB
 *
 * NAME
 *
 *    #define SILC_GET32_LSB ...
 *
 * DESCRIPTION
 *
 *    Return four 8-bit bytes, least significant bytes first.
 *
 * SOURCE
 */
#define SILC_GET32_LSB(l, cp)				\
	(l) = ((SilcUInt32)(SilcUInt8)(cp)[0])		\
	    | ((SilcUInt32)(SilcUInt8)(cp)[1] << 8)	\
	    | ((SilcUInt32)(SilcUInt8)(cp)[2] << 16)	\
	    | ((SilcUInt32)(SilcUInt8)(cp)[3] << 24)

/* Same as upper but XOR the result always. Special purpose macro. */
#define SILC_GET32_X_LSB(l, cp)				\
	(l) ^= ((SilcUInt32)(SilcUInt8)(cp)[0])		\
	    | ((SilcUInt32)(SilcUInt8)(cp)[1] << 8)	\
	    | ((SilcUInt32)(SilcUInt8)(cp)[2] << 16)	\
	    | ((SilcUInt32)(SilcUInt8)(cp)[3] << 24)
/***/

/****d* silcutil/SILCTypes/SILC_PUT32_LSB
 *
 * NAME
 *
 *    #define SILC_PUT32_LSB ...
 *
 * DESCRIPTION
 *
 *    Put four 8-bit bytes, least significant bytes first.
 *
 * SOURCE
 */
#define SILC_PUT32_LSB(l, cp)			\
	(cp)[0] = l;				\
	(cp)[1] = l >> 8;			\
	(cp)[2] = l >> 16;			\
	(cp)[3] = l >> 24;
/***/

/****d* silcutil/SILCTypes/SILC_GET64_MSB
 *
 * NAME
 *
 *    #define SILC_GET64_MSB ...
 *
 * DESCRIPTION
 *
 *    Return eight 8-bit bytes, most significant bytes first.
 *
 * SOURCE
 */
#define SILC_GET64_MSB(l, cp)				\
       (l) = ((((SilcUInt64)GET_WORD((cp))) << 32) |	\
	      ((SilcUInt64)GET_WORD((cp) + 4)))
/***/

/****d* silcutil/SILCTypes/SILC_PUT64_MSB
 *
 * NAME
 *
 *    #define SILC_PUT64_MSB ...
 *
 * DESCRIPTION
 *
 *    Put eight 8-bit bytes, most significant bytes first.
 *
 * SOURCE
 */
#define SILC_PUT64_MSB(l, cp)					\
do {								\
  SILC_PUT32_MSB((SilcUInt32)((SilcUInt64)(l) >> 32), (cp));	\
  SILC_PUT32_MSB((SilcUInt32)(l), (cp) + 4);			\
} while(0)
/***/

#endif /* SILCTYPES_H */