#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libprocess], [0.0.0], [nealda4@gmail.com])
AC_CONFIG_SRCDIR([src/Process.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
PKG_PROG_PKG_CONFIG

# Checks for libraries.
AM_PATH_CPPUNIT

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AX_CXX_COMPILE_STDCXX_11

# Checks for library functions.

AC_CONFIG_FILES([Makefile
	include/Makefile
	src/Makefile
	test/Makefile])

AC_OUTPUT
