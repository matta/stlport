#include <algorithm>
#include <vector>
#include "cppunit/cppunit_proxy.h"

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif

//
// TestCase class
//
class CountTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(CountTest);
  CPPUNIT_TEST(count0);
  CPPUNIT_TEST(count1);
  CPPUNIT_TEST(countif1);
  CPPUNIT_TEST_SUITE_END();

protected:
  void count0();
  void count1();
  void countif1();
  static int odd(int a_);
};

CPPUNIT_TEST_SUITE_REGISTRATION(CountTest);

//
// tests implementation
//
void CountTest::count0()
{
  int numbers[10] = { 1, 2, 4, 1, 2, 4, 1, 2, 4, 1 };

  int result = 0;
  count(numbers, numbers + 10, 1, result);
  CPPUNIT_ASSERT(result==4);
}
void CountTest::count1()
{
  vector <int> numbers(100);
  for(int i = 0; i < 100; i++)
  numbers[i] = i % 3;
  int elements = 0;
  count(numbers.begin(), numbers.end(), 2, elements);
  CPPUNIT_ASSERT(elements==33);
}
void CountTest::countif1()
{
  vector <int> numbers(100);
  for(int i = 0; i < 100; i++)
    numbers[i] = i % 3;
  int elements = 0;
  count_if(numbers.begin(), numbers.end(), odd, elements);
  CPPUNIT_ASSERT(elements==33);
}
int CountTest::odd(int a_)
{
  return a_ % 2;
}
