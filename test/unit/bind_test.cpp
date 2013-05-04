// -*- C++ -*- Time-stamp: <2013-05-04 18:01:40 ptr>

/*
 * Copyright (c) 2004-2013
 * Petr Ovtchenkov
 *
 * Copyright (c) 2004-2008
 * Francois Dumont
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

#include "bind_test.h"

#include <algorithm>
#include <functional>
#include <stl/_bind.h>

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif

class pre_increment : public binary_function<int, int, int> {
public:
  int operator()(int incr, int& val) const
  { return val += incr; }
};

class post_increment : public binary_function<int, int, int> {
public:
  int operator()(int& val, int incr) const
  { return val += incr; }
};

int EXAM_IMPL(bind_test::bind1st1)
{
  int array [3] = { 1, 2, 3 };
  int* p = remove_if((int*)array, (int*)array + 3, bind1st(less<int>(), 2));

  EXAM_CHECK(p == &array[2]);
  EXAM_CHECK(array[0] == 1);
  EXAM_CHECK(array[1] == 2);

  for_each((int*)array, (int*)array + 3, bind1st(pre_increment(), 1));
  EXAM_CHECK(array[0] == 2);
  EXAM_CHECK(array[1] == 3);
  EXAM_CHECK(array[2] == 4);

  for_each((int*)array, (int*)array + 3, bind2nd(post_increment(), 1));
  EXAM_CHECK(array[0] == 3);
  EXAM_CHECK(array[1] == 4);
  EXAM_CHECK(array[2] == 5);

  return EXAM_RESULT;
}

int EXAM_IMPL(bind_test::bind2nd1)
{
  int array [3] = { 1, 2, 3 };
  replace_if(array, array + 3, binder2nd<greater<int> >(greater<int>(), 2), 4);

  EXAM_CHECK(array[0]==1);
  EXAM_CHECK(array[1]==2);
  EXAM_CHECK(array[2]==4);

  return EXAM_RESULT;
}

int EXAM_IMPL(bind_test::bind2nd2)
{
  int array [3] = { 1, 2, 3 };
  replace_if(array, array + 3, bind2nd(greater<int>(), 2), 4);
  EXAM_CHECK(array[0]==1);
  EXAM_CHECK(array[1]==2);
  EXAM_CHECK(array[2]==4);

  return EXAM_RESULT;
}

int test_func1 (const int &param1, const int &param2) {
  return param1 + param2;
}

int test_func2 (int &param1, int param2) {
  param1 += param2;
  return param1 + param2;
}

int EXAM_IMPL(bind_test::bind2nd3)
{
  int array[3] = { 1, 2, 3 };
  transform(array, array + 3, array, bind2nd(ptr_fun(test_func1), 1));
  transform(array, array + 3, array, bind1st(ptr_fun(test_func1), -1));
  EXAM_CHECK(array[0] == 1);
  EXAM_CHECK(array[1] == 2);
  EXAM_CHECK(array[2] == 3);

  transform(array, array + 3, array, bind2nd(ptr_fun(test_func2), 10));
  EXAM_CHECK(array[0] == 21);
  EXAM_CHECK(array[1] == 22);
  EXAM_CHECK(array[2] == 23);

  return EXAM_RESULT;
}

class A
{
  public:
    A() : m_n( 0 )
    {}

    void f( int n ) const {
#if defined (STLPORT)
      _STLP_MUTABLE(A, m_n) = n;
#else
      m_n = n;
#endif
    }

    int v() const
    { return m_n; }

  private:
    mutable int m_n;
};

int EXAM_IMPL(bind_test::bind_memfn)
{
  A array[3];

  for_each( array, array + 3, bind2nd( mem_fun_ref(&A::f), 12 ) );

  EXAM_CHECK( array[0].v() == 12 );

  return EXAM_RESULT;
}

int func_plus_2( int v )
{
  return v + 2;
}

int func_plus_2_mod( int v, int d )
{
  return (v + 2) % d;
}

int func_plus_2_mod_ref( const int& v, const int& d )
{
  return (v + 2) % d;
}

int EXAM_IMPL(bind_test::bind_core)
{
  EXAM_CHECK( (bind( func_plus_2, 1 )() == 3) );
  EXAM_CHECK( (bind( func_plus_2_mod, 1, std::placeholders::_1 )(2) == 1) );
  EXAM_CHECK( (bind( func_plus_2_mod_ref, 1, std::placeholders::_1 )(2) == 1) );
  EXAM_CHECK( (bind( func_plus_2_mod_ref, std::placeholders::_1, 2 )(1) == 1) );
#if 0
  {
    tuple<int,std::placeholders::detail::placeholder<1> > t(1,std::placeholders::_1);
    tuple<const int&,std::placeholders::detail::placeholder<1> > t1(1,std::placeholders::_1);

    cerr << std::detail::_get_tuple_or_arg<0,0,tuple<int,std::placeholders::detail::placeholder<1> >,int>::get( std::forward<tuple<int,std::placeholders::detail::placeholder<1> > >(t), 2 ) << endl;

    cerr << std::detail::_get_tuple_or_arg<1,1,tuple<int,std::placeholders::detail::placeholder<1> >,int>::get( std::forward<tuple<int,std::placeholders::detail::placeholder<1> > >(t), 2 ) << endl;

    cerr << std::detail::_get_tuple_or_arg<1,1,tuple<const int&,std::placeholders::detail::placeholder<1> >,const int&>::get( std::forward<tuple<const int&,std::placeholders::detail::placeholder<1> > >(t), 2 ) << endl;

    cerr << func_plus_2_mod(
      std::detail::_get_tuple_or_arg<is_placeholder<
                      typename tuple_element<0,tuple<int,std::placeholders::detail::placeholder<1> > >::type>::value
      ,0,tuple<int,std::placeholders::detail::placeholder<1> >,int>::get( std::forward<tuple<int,std::placeholders::detail::placeholder<1> > >(t), 2 ),
      std::detail::_get_tuple_or_arg<is_placeholder<
                      typename tuple_element<1,tuple<int,std::placeholders::detail::placeholder<1> > >::type>::value
      ,1,tuple<int,std::placeholders::detail::placeholder<1> >,int>::get( std::forward<tuple<int,std::placeholders::detail::placeholder<1> > >(t), 2 )
      )
<< endl;
  }
#endif
  {
    int array [3] = { 1, 2, 3 };
    int* p = remove_if((int*)array, (int*)array + 3, bind(less<int>(), 2, std::placeholders::_1));

    EXAM_CHECK(p == &array[2]);
    EXAM_CHECK(array[0] == 1);
    EXAM_CHECK(array[1] == 2);

    for_each((int*)array, (int*)array + 3, bind(pre_increment(), 1, std::placeholders::_1));
    EXAM_CHECK(array[0] == 2);
    EXAM_CHECK(array[1] == 3);
    EXAM_CHECK(array[2] == 4);

    for_each((int*)array, (int*)array + 3, bind(post_increment(), std::placeholders::_1, 1));
    EXAM_CHECK(array[0] == 3);
    EXAM_CHECK(array[1] == 4);
    EXAM_CHECK(array[2] == 5);
  }

  {
    int array [3] = { 1, 2, 3 };
    replace_if(array, array + 3, bind(less<int>(), 2, std::placeholders::_1), 4);

    EXAM_CHECK(array[0]==1);
    EXAM_CHECK(array[1]==2);
    EXAM_CHECK(array[2]==4);
  }

  { // same as above, just with lambda
    int array [3] = { 1, 2, 3 };
    replace_if(array, array + 3, bind([](int l, int r) { return l < r; }, 2, std::placeholders::_1), 4);

    EXAM_CHECK(array[0]==1);
    EXAM_CHECK(array[1]==2);
    EXAM_CHECK(array[2]==4);
  }

  {
    int array [3] = { 1, 2, 3 };
    replace_if(array, array + 3, bind(greater<int>(), std::placeholders::_1, 2), 4);

    EXAM_CHECK(array[0]==1);
    EXAM_CHECK(array[1]==2);
    EXAM_CHECK(array[2]==4);
  }

  {
    int array[3] = { 1, 2, 3 };
    // bind(ptr_fun(test_func1), std::placeholders::_1, 1);

    /* test_func1 accept const int&, so I can't write here rvalue?
       or this is due to a copy of binder<> not well iplemented?
     */
    const int j = 1;
    const int k = -1;
    transform(array, array + 3, array, bind(ptr_fun(test_func1), std::placeholders::_1, j));
    transform(array, array + 3, array, bind(ptr_fun(test_func1), k, std::placeholders::_1 ));
    // transform(array, array + 3, array, bind(ptr_fun(test_func1), std::placeholders::_1, 1));
    // transform(array, array + 3, array, bind(ptr_fun(test_func1), -1, std::placeholders::_1 ));
    EXAM_CHECK(array[0] == 1);
    EXAM_CHECK(array[1] == 2);
    EXAM_CHECK(array[2] == 3);

    transform(array, array + 3, array, bind(ptr_fun(test_func2), std::placeholders::_1, 10));
    EXAM_CHECK(array[0] == 21);
    EXAM_CHECK(array[1] == 22);
    EXAM_CHECK(array[2] == 23);
  }

  {
    A array[3];

    for_each( array, array + 3, bind( mem_fun_ref(&A::f), std::placeholders::_1, 12 ) );

    EXAM_CHECK( array[0].v() == 12 );
  }

  
  return EXAM_RESULT;
}

/* Check that bind with explicit specification of function/functor
   return type is correct 
 */
int EXAM_IMPL(bind_test::bind_ret_convert)
{
  struct S {
    static double x_less( int a, int b )
    { return a < b ? 1.1 : 0.0; }
  };

  {
    int array [3] = { 1, 2, 3 };
    replace_if(array, array + 3, bind<int,double (*)(int,int),int>(S::x_less, 2, std::placeholders::_1), 4);

    EXAM_CHECK(array[0]==1);
    EXAM_CHECK(array[1]==2);
    EXAM_CHECK(array[2]==4);
  }
  
  return EXAM_RESULT;
}
