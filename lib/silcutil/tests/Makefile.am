#
#  Makefile.am
#
#  Author: Pekka Riikonen <priikone@silcnet.org>
#
#  Copyright (C) 2004 - 2008 Pekka Riikonen
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; version 2 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#

AUTOMAKE_OPTIONS = 1.0 no-dependencies foreign

check_PROGRAMS = \
	test_silcstrutil test_silcstringprep test_silchashtable \
	test_silclist test_silcfsm test_silcasync test_silcschedule \
	test_silcnet test_silcstack test_silcmime test_silcfdstream \
	test_silcatomic test_silcmutex test_silctime test_silcthread \
	test_silcdll test_silcenv test_silctimer test_silcbitops \
	test_silcregex test_silcbuffmt test_silcdir test_silcthreadqueue \
	test_silcrand test_silcglobal test_silcbufferstream test_silcxml \
	test_silclocalnetstream

TESTS = test_silcstrutil test_silcstringprep test_silchashtable \
	test_silclist test_silcfsm test_silcasync test_silcschedule \
	test_silcnet test_silcstack test_silcmime test_silcfdstream \
	test_silcatomic test_silctime test_silcthread \
	test_silcdll test_silcenv test_silctimer test_silcbitops \
	test_silcregex test_silcbuffmt test_silcdir test_silcthreadqueue \
	test_silcrand test_silcglobal test_silcbufferstream \
	test_silclocalnetstream

LIBS = $(SILC_COMMON_LIBS)
LDADD = -L.. -L../.. -lsrt

include $(top_srcdir)/Makefile.defines.in
