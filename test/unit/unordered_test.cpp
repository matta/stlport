#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "cppunit/cppunit_proxy.h"

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif

//
// TestCase class
//
class UnorderedTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(UnorderedTest);
  CPPUNIT_TEST(uset);
  CPPUNIT_TEST(umultiset);
  CPPUNIT_TEST(umap);
  CPPUNIT_TEST(umultimap);
  CPPUNIT_TEST_SUITE_END();

protected:
  void uset();
  void umultiset();
  void umap();
  void umultimap();
};

CPPUNIT_TEST_SUITE_REGISTRATION(UnorderedTest);

const int NB_ELEMS = 2000;

//
// tests implementation
//
void UnorderedTest::uset()
{
  typedef unordered_set<int, hash<int>, equal_to<int> > usettype;
  usettype us;

  //Small compilation check of the copy constructor:
  usettype us2(us);
  //And assignment operator
  us = us2;

  int i;
  pair<usettype::iterator, bool> ret;
  for (i = 0; i < NB_ELEMS; ++i) {
    ret = us.insert(i);
    CPPUNIT_ASSERT( ret.second );
    CPPUNIT_ASSERT( *ret.first == i );

    ret = us.insert(i);
    CPPUNIT_ASSERT( !ret.second );
    CPPUNIT_ASSERT( *ret.first == i );
  }

  vector<int> us_val;

  usettype::local_iterator lit, litEnd;
  for (i = 0; i < NB_ELEMS; ++i) {
    lit = us.begin(us.bucket(i));
    litEnd = us.end(us.bucket(i));

    usettype::size_type bucket_pos = us.bucket(*lit);
    for (; lit != litEnd; ++lit) {
      CPPUNIT_ASSERT( us.bucket(*lit) == bucket_pos );
      us_val.push_back(*lit);
    }
  }

  sort(us_val.begin(), us_val.end());
  for (i = 0; i < NB_ELEMS; ++i) {
    CPPUNIT_ASSERT( us_val[i] == i );
  }
}

void UnorderedTest::umultiset()
{
  typedef unordered_multiset<int, hash<int>, equal_to<int> > usettype;
  usettype us;

  int i;
  usettype::iterator ret;
  for (i = 0; i < NB_ELEMS; ++i) {
    ret = us.insert(i);
    CPPUNIT_ASSERT( *ret == i );

    ret = us.insert(i);
    CPPUNIT_ASSERT( *ret == i );
  }

  CPPUNIT_ASSERT( us.size() == 4000 );
  vector<int> us_val;

  usettype::local_iterator lit, litEnd;
  for (i = 0; i < NB_ELEMS; ++i) {
    lit = us.begin(us.bucket(i));
    litEnd = us.end(us.bucket(i));

    usettype::size_type bucket_pos = us.bucket(*lit);
    for (; lit != litEnd; ++lit) {
      CPPUNIT_ASSERT( us.bucket(*lit) == bucket_pos );
      us_val.push_back(*lit);
    }
  }

  sort(us_val.begin(), us_val.end());
  for (i = 0; i < NB_ELEMS; ++i) {
    CPPUNIT_ASSERT( us_val[2 * i] == i );
    CPPUNIT_ASSERT( us_val[2 * i + 1] == i );
  }
}

void UnorderedTest::umap()
{
  typedef unordered_map<int, int, hash<int>, equal_to<int> > umaptype;
  umaptype us;

  //Compilation check of the [] operator:
  umaptype us2;
  us[0] = us2[0];
  us.clear();

  {
    //An other compilation check
    typedef unordered_map<int, umaptype> uumaptype;
    uumaptype uus;
    umaptype const& uref = uus[0];
    //Avoids warning:
    (void*)&uref;
    umaptype ucopy = uus[0];
  }

  int i;
  pair<umaptype::iterator, bool> ret;
  for (i = 0; i < NB_ELEMS; ++i) {
    pair<const int, int> p1(i, i);
    ret = us.insert(p1);
    CPPUNIT_ASSERT( ret.second );
    CPPUNIT_ASSERT( *ret.first == p1 );

    pair<const int, int> p2(i, i + 1);
    ret = us.insert(p2);
    CPPUNIT_ASSERT( !ret.second );
    CPPUNIT_ASSERT( *ret.first == p1 );
  }

  {
    //Lets look for some values to see if everything is normal:
    umaptype::iterator umit;
    for (int j = 0; j < NB_ELEMS; j += NB_ELEMS / 100) {
      umit = us.find(j);

      CPPUNIT_ASSERT( umit != us.end() );
      CPPUNIT_ASSERT( (*umit).second == j );
    }
  }

  CPPUNIT_ASSERT( us.size() == (size_t)NB_ELEMS );
  vector<pair<int, int> > us_val;

  umaptype::local_iterator lit, litEnd;
  for (i = 0; i < NB_ELEMS; ++i) {
    lit = us.begin(us.bucket(i));
    litEnd = us.end(us.bucket(i));

    umaptype::size_type bucket_pos = us.bucket((*lit).first);
    for (; lit != litEnd; ++lit) {
      CPPUNIT_ASSERT( us.bucket((*lit).first) == bucket_pos );
      us_val.push_back(*lit);
    }
  }

  sort(us_val.begin(), us_val.end());
  for (i = 0; i < NB_ELEMS; ++i) {
    CPPUNIT_ASSERT( us_val[i] == make_pair(i, i) );
  }
}

void UnorderedTest::umultimap()
{
  typedef unordered_multimap<int, int, hash<int>, equal_to<int> > umaptype;
  umaptype us;

  int i;
  umaptype::iterator ret;
  for (i = 0; i < NB_ELEMS; ++i) {
    pair<const int, int> p(i, i);
    ret = us.insert(p);
    CPPUNIT_ASSERT( *ret == p );

    ret = us.insert(p);
    CPPUNIT_ASSERT( *ret == p );
  }

  CPPUNIT_ASSERT( us.size() == 4000 );
  typedef pair<int, int> ptype;
  vector<ptype> us_val;

  umaptype::local_iterator lit, litEnd;
  for (i = 0; i < NB_ELEMS; ++i) {
    lit = us.begin(us.bucket(i));
    litEnd = us.end(us.bucket(i));

    umaptype::size_type bucket_pos = us.bucket((*lit).first);
    for (; lit != litEnd; ++lit) {
      CPPUNIT_ASSERT( us.bucket((*lit).first) == bucket_pos );
      us_val.push_back(ptype((*lit).first, (*lit).second));
    }
  }

  sort(us_val.begin(), us_val.end());
  for (i = 0; i < NB_ELEMS; ++i) {
    pair<int, int> p(i, i);
    CPPUNIT_ASSERT( us_val[i * 2] == p );
    CPPUNIT_ASSERT( us_val[i * 2 + 1] == p );
  }
}
