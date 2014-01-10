AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libprocess.la
libprocess_la_SOURCES = Process.cpp
libprocess_la_DEPENDENCIES = libprocess.map
libprocess_la_LDFLAGS = -Wl,--version-script=$(srcdir)/libprocess.map