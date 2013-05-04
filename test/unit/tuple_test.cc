// -*- C++ -*- Time-stamp: <2013-03-26 23:53:28 ptr>

/*
 * Copyright (c) 2013
 * Petr Ovtchenkov
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#include "tuple_test.h"

// #include <algorithm>
// #include <functional>

#include <stl/_tuple.h>

#include <iostream>
#include <iomanip>
#include <typeinfo>

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif

namespace std {

// using namespace detail;

namespace detail {

// Extract and pass stored values as arguments to function:
 
void ff( int& i, double& d, char& c )
{
  cerr << i << ' ' << d << ' ' << c << endl;
}

template <class... Args, size_t... Seq>
void a_call( tuple<Args...>& t, const _sequence<Seq...>& )
{
  // ff( forward<Args>(get<Seq>(b))... );
  ff( get<Seq>(t)... );
}

} // namespace detail

} // namespace std

int EXAM_IMPL(tuple_test::internals)
{
  detail::_tuple_base<0, int, double, char> a;

  // static_cast<detail::_tuple_base<2, char>& >(a).value = 'y';
  a.detail::_tuple_base<2,char>::value = 'z';
  a.detail::_tuple_base<1,double,char>::value = 0.1;
  a.detail::_tuple_base<0,int,double,char>::value = 10;

  EXAM_CHECK( (a.detail::_tuple_base<2,char>::value == 'z') );
  EXAM_CHECK( ((a.detail::_tuple_base<1,double,char>::value > 0.05) && (a.detail::_tuple_base<1,double,char>::value < 0.15)) );
  EXAM_CHECK( (a.detail::_tuple_base<0,int,double,char>::value == 10) );
/*
  cerr << a.detail::_tuple_base<2,char>::value << ' '
       << a.detail::_tuple_base<1,double,char>::value << ' '
       << a.detail::_tuple_base<0,int,double,char>::value
       << endl;
*/

  EXAM_CHECK( (typeid(tuple_element<0,tuple<int,double,char> >::type) == typeid(int&) ) );
  EXAM_CHECK( (typeid(tuple_element<1,tuple<int,double,char> >::type) == typeid(double&) ) );

  tuple<int,double,char> b;

  // tuple_element<0,tuple<int,double,char> >::type v1;
  // tuple_element<1,tuple<int,double,char> >::type v2;

  // v1 = 'A';
  // v2 = 0.2;

  // cerr << v1 << ' '
  //      << v2 << ' ' << " " << typeid(tuple_element<2,tuple<int,double,char> >::type).name()
  //      << endl;

  get<0>( b ) = 20;
  get<1>( b ) = 3.14;
  get<2>( b ) = 'q';

  // cerr << get<0>( b ) << ' '
  //      << get<1>( b ) << ' '
  //      << get<2>( b ) << endl;

  EXAM_CHECK( (get<0>( b ) == 20) );
  EXAM_CHECK( (get<1>( b ) > 3.0) && (get<1>( b ) < 3.5) );
  EXAM_CHECK( (get<2>( b ) == 'q') );

  // ------------- Index expansion to args

  cerr << typeid(detail::_sequence_0_N<3>::type).name() << endl;
  // -------------

  // ff( get<0, 1, 2>( b ) );
  detail::a_call( b, detail::_sequence_0_N<3>::type() );

  // EXAM_CHECK(array[2] == 5);

  return EXAM_RESULT;
}

static int ft( const tuple<int>& t )
{ return get<0>(t); }

static int ft2( tuple<int>&& t )
{ return get<0>(t); }

int EXAM_IMPL(tuple_test::tuple_cv)
{
  tuple<int> v;

  get<0>(v) = 2;

  EXAM_CHECK( (ft(v) == 2) );

  ft2( tuple<int>() );

  return EXAM_RESULT;
}

int EXAM_IMPL(tuple_test::tuple_ctor)
{
  {
    tuple<int,char> v( 100500, '+' );

    EXAM_CHECK( (get<0>(v) == 100500) );
    EXAM_CHECK( (get<1>(v) == '+') );
  }
  {
    tuple<int,char> v( '+', 0x20 );

    EXAM_CHECK( (get<0>(v) == 43) );
    EXAM_CHECK( (get<1>(v) == ' ') );
  }
  {
    tuple<int,char> v( 100500, '+' );
    tuple<int,char> w( v );

    EXAM_CHECK( (get<0>(w) == 100500) );
    EXAM_CHECK( (get<1>(w) == '+') );
  }
  {
    tuple<int,char> v( 32, '+' );
    tuple<char,int> w( v );

    EXAM_CHECK( (get<0>(w) == ' ') );
    EXAM_CHECK( (get<1>(w) == 43) );
  }
  {
    tuple<char,int> w( tuple<int,char>(32, '+') );

    EXAM_CHECK( (get<0>(w) == ' ') );
    EXAM_CHECK( (get<1>(w) == 43) );
  }
  {
    pair<char,int> p;
    p.first = '+';
    p.second = 32;

    tuple<char,int> w( p );

    EXAM_CHECK( (get<0>(w) == '+') );
    EXAM_CHECK( (get<1>(w) == 32) );
  }
  {
    tuple<char,int> w( make_pair( '+', 32 ) );

    EXAM_CHECK( (get<0>(w) == '+') );
    EXAM_CHECK( (get<1>(w) == 32) );
  }
  {
    int i = 1;
    tuple<int,int&> w( i, ref(i) );

    EXAM_CHECK( (get<0>(w) == 1) );
    EXAM_CHECK( (get<1>(w) == 1) );
    i = 2;
    EXAM_CHECK( (get<1>(w) == 2) );
    EXAM_CHECK( (get<0>(w) == 1) );
  }
  {
    int i = 1;
    tuple<int,const int&> w( i, cref(i) );

    EXAM_CHECK( (get<0>(w) == 1) );
    EXAM_CHECK( (get<1>(w) == 1) );
    i = 2;
    EXAM_CHECK( (get<1>(w) == 2) );
    EXAM_CHECK( (get<0>(w) == 1) );
  }

  return EXAM_RESULT;
}
