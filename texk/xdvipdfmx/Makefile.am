## Makefile.am for the TeX Live subdirectory texk/xdvipdfmx/
##
## Copyright (C) 2009-2013 Peter Breitenlohner <tex-live@tug.org>
## You may freely use, modify and/or distribute this file.
##
ACLOCAL_AMFLAGS = -I ../../m4

SUBDIRS = . src

all-local: config.force

config.force: $(ZLIB_DEPEND) $(LIBPNG_DEPEND) $(FREETYPE2_DEPEND) $(KPATHSEA_DEPEND)
	echo timestamp >config.force
	$(SHELL) ./config.status --recheck

DISTCLEANFILES = config.force

EXTRA_DIST = \
	data

# in case of an SVN repository
dist-hook:
	rm -rf `find $(distdir) -name .svn`

## Tests
TESTS = xdvipdfmx.test
EXTRA_DIST += $(TESTS)

