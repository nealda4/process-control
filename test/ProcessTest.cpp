
#include <iostream>
#include <process-control/Process.hpp>
#include "ProcessTest.h"
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ProcessTest );
void
ProcessTest::setUp()
{
}
void
ProcessTest::tearDown()
{
}
void
ProcessTest::testConstructor()
{
	CPPUNIT_ASSERT_NO_THROW_MESSAGE("constructor threw exception", ProcessControl::Process("/bin/ls"));
}
void
ProcessTest::testWaitExitNormal()
{
	unsigned retval = static_cast<unsigned>(ProcessControl::Process("/bin/ls").wait());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("process return value != EXIT_SUCCESS (0)", static_cast<unsigned>(0), retval);
}
