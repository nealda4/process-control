#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libprocess], [0.0.0], [nealda4@gmail.com])

AC_CONFIG_SRCDIR([src/Process.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE

# Checks for programs.
LT_INIT
AC_PROG_CXX
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG

# Checks for libraries.
AM_PATH_CPPUNIT

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AX_CXX_COMPILE_STDCXX_11

# Checks for library functions.

# Configuration Options
AC_ARG_ENABLE(profiling, [AS_HELP_STRING([--enable-profiling], [build with profiling options])],
	[
		if test "x$enableval" = "xyes" ; then
			COMPILE_FLAGS=`echo $COMPILE_FLAGS | sed 's/-O.//'`
			COMPILE_FLAGS="$COMPILE_FLAGS  -p -pg"
			profile="true"
			AC_MSG_NOTICE([enabling profiling])
		else
			profile="false"
		fi
	],
	[
		profile="false"
	] 
)

AC_ARG_ENABLE(gcov, [AS_HELP_STRING([--enable-gcov], [Enable gcov code coverage])],
	[
		if test "x$enableval" = "xyes" ; then
			COMPILE_FLAGS="$COMPILE_FLAGS  --coverage"
			LDFLAGS="$LDFLAGS -lgcov"
			gcov="true"
			AC_MSG_NOTICE([enabling gcov coverage])
		else
			gcov="false"
		fi
	],
	[
		gcov="false"
	] 
)

CFLAGS="$CFLAGS $COMPILE_FLAGS"
CPPFLAGS="$CPPFLAGS $COMPILE_FLAGS"

AC_CONFIG_FILES([Makefile
	include/Makefile
	src/Makefile
	test/Makefile])

AC_OUTPUT
