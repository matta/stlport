/*
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

#ifndef _STLP_INTERNAL_MBSTATE_T
#define _STLP_INTERNAL_MBSTATE_T

#if !defined (_STLP_NO_WCHAR_T)
#  if defined (__cplusplus)
/* This header is included from the outside world when building the lib
 * so we cannot directly include stl/_cwchar.h. */
#    include <cwchar>
#  else
#    include <wchar.h>
#  endif
#endif

#if defined (_STLP_NO_WCHAR_T) || \
    defined (__MRC__) || defined (__SC__) || defined (__BORLANDC__) || \
    defined(__OpenBSD__) || defined(__FreeBSD__) || \
   (defined (__GNUC__) && (defined (__APPLE__) || defined( __Lynx__ )))
#  include _STLP_NATIVE_C_HEADER(stddef.h)
#  if defined (__Lynx__)
#    ifndef _WINT_T
typedef long int wint_t;
#      define _WINT_T
#    endif /* _WINT_T */
#  endif
#  if defined(__OpenBSD__)
typedef _BSD_WINT_T_ wint_t;
#  endif /* __OpenBSD__ */
#endif

#if (defined (__OpenBSD__) || defined (__FreeBSD__)) && defined (__GNUC__) && !defined (_GLIBCPP_HAVE_MBSTATE_T)
#  define __mbstate_t_defined /* mbstate_t defined in native <cwchar>, so not defined in C! */
#endif

#if !defined (_STLP_USE_NO_IOSTREAMS) && defined (_STLP_NO_NATIVE_MBSTATE_T) && !defined (_STLP_NO_MBSTATE_T) && !defined (_MBSTATE_T) && !defined (__mbstate_t_defined)
#  define _STLP_USE_OWN_MBSTATE_T
#  define _MBSTATE_T
#endif

#if defined (_STLP_USE_OWN_MBSTATE_T)
/* to be compatible across different SUN platforms */
#  ifdef __sun
#    define __stl_mbstate_t __mbstate_t
#  endif

struct __stl_mbstate_t;

#  if defined (__cplusplus)
struct __stl_mbstate_t {
  __stl_mbstate_t( long __st = 0 ) { _M_state[0] = __st ; }
  __stl_mbstate_t& operator=(const long __st) {
    _M_state[0] = __st;
    return *this;
  }
  __stl_mbstate_t(const __stl_mbstate_t& __x) {_M_state[0]= __x._M_state[0]; }
  __stl_mbstate_t& operator=(const __stl_mbstate_t& __x) {
    _M_state[0]= __x._M_state[0];
    return *this;
  }
#    if defined (__sun)
#      ifdef _LP64
  long _M_state[4];
#      else
  int _M_state[6];
#      endif
#    else
  long _M_state[1];
#    endif
};

inline bool operator==(const __stl_mbstate_t& __x, const __stl_mbstate_t& __y)
{ return ( __x._M_state[0] == __y._M_state[0] ); }

inline bool operator!=(const __stl_mbstate_t& __x, const __stl_mbstate_t& __y)
{ return ( __x._M_state[0] == __y._M_state[0] ); }
#  else
#    if defined(__BORLANDC__)
typedef char __stl_mbstate_t;
#    endif
#  endif

_STLP_BEGIN_NAMESPACE

typedef __stl_mbstate_t mbstate_t;

_STLP_END_NAMESPACE

#endif /* _STLP_USE_OWN_MBSTATE_T */

#if !defined (_STLP_NO_WCHAR_T)
#  ifndef WCHAR_MIN
#    define WCHAR_MIN 0
/* SUNpro has some bugs with casts. wchar_t is size of int there anyway. */
#    if defined (__SUNPRO_CC) || defined (__DJGPP)
#      define WCHAR_MAX (~0)
#    else
#      define WCHAR_MAX ((wchar_t)~0)
#    endif
#  endif
#  if defined (__GNUC__) && defined (__alpha__)
/* Definition of WCHAR_MIN and MAX are wrong for alpha platform
 * as gcc consider wchar_t as an unsigned type. Static assertion are
 * here to check that a future alpha SDK or a future gcc won't change the
 * situation making this workaround useless.
 */
_STLP_STATIC_ASSERT(((wchar_t)-1 > 0) && (WCHAR_MIN < 0))
#    undef WCHAR_MIN
#    define WCHAR_MIN 0
#    undef WCHAR_MAX
#    define WCHAR_MAX ((wchar_t)~0)
#  endif
#endif

/*
 * # if defined  (_STLP_IMPORT_VENDOR_CSTD) && ! defined (_STLP_VENDOR_GLOBAL_CSTD)
 * _STLP_BEGIN_NAMESPACE
 * using namespace _STLP_VENDOR_CSTD;
 * _STLP_END_NAMESPACE
 * #endif
 */ /* _STLP_IMPORT_VENDOR_CSTD */

#endif /* _STLP_INTERNAL_MBSTATE_T */
