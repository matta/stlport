/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
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
# include "stlport_prefix.h"

// #include "locale_impl.h"
# include <stl/_numpunct.h>

__STL_BEGIN_NAMESPACE

//----------------------------------------------------------------------
// numpunct<char>
char   numpunct<char>::do_decimal_point() const {return '.';}
char   numpunct<char>::do_thousands_sep() const { return ','; }
string numpunct<char>::do_grouping()  const { return string();}
string numpunct<char>::do_truename()  const { return _M_truename;}
string numpunct<char>::do_falsename() const { return _M_falsename; }

wchar_t numpunct<wchar_t>::do_decimal_point() const { return L'.'; }

wchar_t numpunct<wchar_t>::do_thousands_sep() const
{
  return L',';
}

string numpunct<wchar_t>::do_grouping() const
{
  return string();
}


wstring numpunct<wchar_t>::do_truename() const
{
  return _M_truename;
}
 
wstring numpunct<wchar_t>::do_falsename() const
{
  return _M_falsename;
} 

__STL_STATIC_MEMBER_DECLSPEC  string numpunct<char>::_M_truename("true");
__STL_STATIC_MEMBER_DECLSPEC  string  numpunct<char>::_M_falsename("false");
__STL_STATIC_MEMBER_DECLSPEC  string numpunct<char>::_M_grouping("");

# ifndef __STL_NO_WCHAR_T
__STL_STATIC_MEMBER_DECLSPEC  wstring numpunct<wchar_t>::_M_truename(L"true");
__STL_STATIC_MEMBER_DECLSPEC  wstring numpunct<wchar_t>::_M_falsename(L"false");
__STL_STATIC_MEMBER_DECLSPEC  string numpunct<wchar_t>::_M_grouping("");
# endif

__STL_END_NAMESPACE

// Local Variables:
// mode:C++
// End:

