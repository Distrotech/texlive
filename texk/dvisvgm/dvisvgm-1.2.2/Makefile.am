## This file is part of dvisvgm
## Copyright (C) 2005-2013 Martin Gieseking <martin.gieseking@uos.de>
##
## Process this file with automake.

AUTOMAKE_OPTIONS = foreign
EXTRA_DIST = COPYING LGPL-2.1.txt
if HAVE_LIBPOTRACE
SUBDIRS =
else
SUBDIRS = potracelib
endif

SUBDIRS += src tests doc

