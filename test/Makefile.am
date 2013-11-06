AM_CPPFLAGS = -I$(top_srcdir)/include

# TESTS are run by make check
TESTS = process_test 

# Programs needed by tests
HELPERS = spawned_process

# make check builds check_PROGRAMS
check_PROGRAMS = $(TESTS) $(HELPERS) 

process_test_SOURCES = TestRunner.cpp ProcessTest.h ProcessTest.cpp
process_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
process_test_LDFLAGS = $(CPPUNIT_LIBS) -L$(top_builddir)/src -lprocess -ldl 

spawned_process_SOURCES = SpawnedProcess.cpp