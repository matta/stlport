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

_STLP_BEGIN_NAMESPACE

//----------------------------------------------------------------------
// Class _Locale_impl
// This is the base class which implements access only and is supposed to 
// be used for classic locale only
class _STLP_CLASS_DECLSPEC _Locale_impl :
    public _Refcount_Base
{
  public:
    _Locale_impl(const char* s);
    _Locale_impl(const _Locale_impl&);
    _Locale_impl(size_t n, const char* s);

  private:
    ~_Locale_impl();

  public:

    size_t size() const { return facets_vec.size(); }

    static void make_classic_locale();
#ifdef _STLP_LEAKS_PEDANTIC
    static void free_classic_locale();
#endif // _STLP_LEAKS_PEDANTIC
  
    basic_string<char, char_traits<char>, allocator<char> > name;

    static void _STLP_CALL _M_throw_bad_cast();

  private:
    void operator=(const _Locale_impl&);

  public:
    class _STLP_CLASS_DECLSPEC Init
    {
      public:
        Init();
        ~Init();
      private:
        static _Refcount_Base _S_count;
    };

    static void _STLP_CALL _S_initialize();
    static void _STLP_CALL _S_uninitialize();
    friend class Init;

  public: // _Locale
    // void remove(size_t index);
    locale::facet* insert(locale::facet*, size_t index);
    void insert(_Locale_impl* from, const locale::id& n);

    // Helper functions for byname construction of locales.
    void insert_ctype_facets(const char* name);
    void insert_numeric_facets(const char* name);
    void insert_time_facets(const char* name);
    void insert_collate_facets(const char* name);
    void insert_monetary_facets(const char* name);
    void insert_messages_facets(const char* name);

  private:

    vector<locale::facet*> facets_vec;

  private:
    friend struct _Locale_classic_free;
    friend _Locale_impl * _STLP_CALL _copy_Locale_impl( _Locale_impl * );
    friend _Locale_impl * _STLP_CALL _copy_Nameless_Locale_impl( _Locale_impl * );
    friend void _STLP_CALL _release_Locale_impl( _Locale_impl *& loc );
#if defined (_STLP_USE_MSVC6_MEM_T_BUG_WORKAROUND)
    friend class _STLP_NO_MEM_T_NAME(loc);
#else
    friend class locale;
#endif
};

_STLP_DECLSPEC _Locale_impl * _STLP_CALL _get_Locale_impl( _Locale_impl *loc );
void _STLP_CALL _release_Locale_impl( _Locale_impl *& loc );
_Locale_impl * _STLP_CALL _copy_Locale_impl( _Locale_impl *loc );
_STLP_DECLSPEC _Locale_impl * _STLP_CALL _copy_Nameless_Locale_impl( _Locale_impl * );

_STLP_END_NAMESPACE

#endif

// Local Variables:
// mode:C++
// End:
