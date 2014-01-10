AM_CPPFLAGS = -I$(top_srcdir)/include 

# code coverage target
coverage: check
	lcov --base-directory . --directory src -capture --output-file src.lcov
	genhtml -o coverage src.lcov

SUBDIRS = src include test
