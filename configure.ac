# Copyright 2019 Joao Eriberto Mota Filho <eriberto@eriberto.pro.br>
# This file is under GPL-2+ license.

AC_PREREQ([2.69])
AC_INIT([dnsmap], [0.35], [https://github.com/resurrecting-open-source-projects/dnsmap/issues])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/dnsmap.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([gethostbyname inet_ntoa memset strstr])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
