#ifndef PROCESSTEST_H
#define PROCESSTEST_H
#include <cppunit/extensions/HelperMacros.h>
class ProcessTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ProcessTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testWaitExitNormal );
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown();
  void testConstructor();
  void testWaitExitNormal();
};
#endif  // PROCESSTEST_H
