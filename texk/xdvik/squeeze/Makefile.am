## Makefile.am for the TeX Live subdirectory texk/xdvik/squeeze/
##
## Copyright (C) 2009, 2012 Peter Breitenlohner <tex-live@tug.org>
## You may freely use, modify and/or distribute this file.
##
ACLOCAL_AMFLAGS = -I ../../../m4

AM_CFLAGS = $(WARNING_CFLAGS)

noinst_PROGRAMS = squeeze

nodist_noinst_DATA = stamp-squeeze

stamp-squeeze: squeeze$(EXEEXT)
	$(AM_V_at)echo timestamp >$@

CLEANFILES = stamp-squeeze

