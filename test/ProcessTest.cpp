
#include <iostream>
#include <system_error>
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
	CPPUNIT_ASSERT_NO_THROW_MESSAGE("constructor threw exception", ProcessControl::Process("./spawned_process"));
}
void
ProcessTest::testConstructorExecFailure()
{
	CPPUNIT_ASSERT_THROW_MESSAGE("constructor did not throw exception",
			ProcessControl::Process("sfasdf 242"),
			std::exception);
}
void
ProcessTest::testWaitExitSuccess()
{
	unsigned retval = static_cast<unsigned>(ProcessControl::Process("./spawned_process 0").wait());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("process return value != EXIT_SUCCESS (0)", static_cast<unsigned>(0), retval);
}
void
ProcessTest::testWaitExitFailure()
{
	unsigned retval = static_cast<unsigned>(ProcessControl::Process("./spawned_process 1").wait());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("process return value != EXIT_FAILURE (1)", static_cast<unsigned>(1), retval);
}
void
ProcessTest::testWaitExitWithSignal()
{
	CPPUNIT_ASSERT_THROW_MESSAGE("spawned process exited abnormally but wait did not throw",
			ProcessControl::Process("./spawned_process").wait(),
			std::runtime_error);
}
