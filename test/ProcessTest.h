#ifndef PROCESSTEST_H
#define PROCESSTEST_H
#include <cppunit/extensions/HelperMacros.h>
class ProcessTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ProcessTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testConstructorExecFailure );
  CPPUNIT_TEST( testWaitExitSuccess );
  CPPUNIT_TEST( testWaitExitFailure );
  CPPUNIT_TEST( testWaitExitWithSignal );
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown();
  void testConstructor();
  void testConstructorExecFailure();
  void testWaitExitSuccess();
  void testWaitExitFailure();
  void testWaitExitWithSignal();
};
#endif  // PROCESSTEST_H
