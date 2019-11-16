# Copyright 2019 Joao Eriberto Mota Filho <eriberto@eriberto.pro.br>
# This file is under GPL-2+ license.

SUBDIRS = src

man_MANS = man/dnsmap.1 man/dnsmap-bulk.1
EXTRA_DIST = man/dnsmap.1 man/dnsmap-bulk.1

distclean-local:
	-rm -rf autom4te.cache/ \
	m4/*

	-rm -f *~ \
	aclocal.m4 \
	compile \
	config.* \
	configure \
	depcomp \
	INSTALL \
	install-sh \
	ltmain.sh \
	Makefile.in \
	missing \
	src/Makefile.in
