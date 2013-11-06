
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
	ProcessControl::Process("${PWD}/spawned-process");
  //CPPUNIT_FAIL( "not implemented" );
}
void
ProcessTest::testWaitExitNormal()
{
  CPPUNIT_ASSERT_THROW_MESSAGE("expecting exception", ProcessControl::Process("${PWD}/spawned-process").wait(), std::exception);
}
