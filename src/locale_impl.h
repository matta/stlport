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

# ifndef LOCALE_IMPL_H
#  define  LOCALE_IMPL_H

#include <clocale>             // C locale header file.
#include <vector>
#include <string>
#include <stl/_locale.h>
#include "c_locale.h"

#ifdef _STLP_DEBUG
#include <stl/debug/_debug.h>
#endif

# if defined(_STLP_ASSERTIONS) || defined(_STLP_DEBUG)
#  define _STLP_FILE_UNIQUE_ID LOCALE_IMPL_H_NAME
_STLP_INSTRUMENT_FILE();
# endif

_STLP_BEGIN_NAMESPACE

//----------------------------------------------------------------------
// Class _Locale_impl
// This is the base class which implements access only and is supposed to 
// be used for classic locale only
class _STLP_CLASS_DECLSPEC _Locale_impl
{
  public:
    _Locale_impl(const char* s);
    //  _Locale_impl(const _Locale_impl&);
    virtual ~_Locale_impl();

    virtual void incr();
    virtual void decr();

    size_t size() const { return _M_size; }

    static void make_classic_locale();
#ifdef _STLP_LEAKS_PEDANTIC
    static void free_classic_locale();
#endif // _STLP_LEAKS_PEDANTIC
  
    locale::facet** facets;
    size_t _M_size;

    basic_string<char, char_traits<char>, allocator<char> > name;

    static void _STLP_CALL _M_throw_bad_cast();

  private:
    void operator=(const _Locale_impl&);

    // private:
  public:
    class _STLP_CLASS_DECLSPEC Init
    {
      public:
        Init();
        ~Init();
      private:
        static long _S_count;
        // friend class _Locale_impl;
    };

    static void _STLP_CALL _S_initialize();
    static void _STLP_CALL _S_uninitialize();
    friend class Init;
};

inline _Locale_impl*  _STLP_CALL _S_copy_impl(_Locale_impl* I) {
    _STLP_ASSERT( I != 0 );
    I->incr();
    return I;
}

_STLP_END_NAMESPACE

# if defined(_STLP_ASSERTIONS) || defined(_STLP_DEBUG)
#  undef _STLP_FILE_UNIQUE_ID
# endif

#endif

// Local Variables:
// mode:C++
// End:
