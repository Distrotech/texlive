## Makefile.am for the TeX Live subdirectory texk/dvipdfmx/
##
## Copyright (C) 2009-2013 Peter Breitenlohner <tex-live@tug.org>
## You may freely use, modify and/or distribute this file.
##
ACLOCAL_AMFLAGS = -I ../../m4

SUBDIRS = src man data

all-local: config.force

config.force: $(ZLIB_DEPEND) $(LIBPNG_DEPEND) $(KPATHSEA_DEPEND)
	echo timestamp >config.force
	$(SHELL) ./config.status --recheck

DISTCLEANFILES = config.force

## Tests
TESTS = dvipdfmx.test
EXTRA_DIST = $(TESTS)

