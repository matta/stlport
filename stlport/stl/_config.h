 /*
  *
  * Copyright (c) 1994
  * Hewlett-Packard Company
  *
  * Copyright (c) 1996,1997
  * Silicon Graphics Computer Systems, Inc.
  *
  * Copyright (c) 1997
  * Moscow Center for SPARC Technology
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

#ifndef _STLP_CONFIG_H
#define _STLP_CONFIG_H

/*
 * Purpose of this file:
 *
 * Defines all STLport settings.
 * This file is actually a wrapper : it includes compiler-specific
 * settings from <config/stlcomp.h>
 * and user-defined settings from <stl_user_config.h>.
 * See <config/stl_mycomp.h> and <stl_user_config.h> for the description
 * of those macros
 *
 */

/* Other macros defined by this file:

 * bool, true, and false, if _STLP_NO_BOOL is defined.
 * typename, as a null macro if it's not already a keyword.
 * explicit, as a null macro if it's not already a keyword.
 * namespace-related macros (_STLP_STD, _STLP_BEGIN_NAMESPACE, etc.)
 * exception-related macros (_STLP_TRY, _STLP_UNWIND, etc.)
 * _STLP_ASSERT, either as a test or as a null macro, depending on
   whether or not _STLP_ASSERTIONS is defined.
*/

/* Include useful information about system:
 */
#ifdef __linux__
#  include <features.h>
#endif


/* Definition of the STLport version informations */
#include <stl/_stlport_version.h>

/* Definition of the 2 STLport debug levels */
#define _STLP_STLPORT_DBG_LEVEL 1
#define _STLP_STANDARD_DBG_LEVEL 2

/* Placeholder for user to override settings.
 * It could be also used to mask settings from
 * different directories.
 */
#include <stl_user_config.h>

#if defined (__BUILDING_STLPORT)
/* For the STLport implementation we can use everything:
 */
#  if defined (_STLP_NO_ANACHRONISMS)
#    undef _STLP_NO_ANACHRONISMS
#  endif
#  if defined (_STLP_NO_EXTENSIONS)
#    undef _STLP_NO_EXTENSIONS
#  endif
/* Moreover there are things that has no sens:
 */
#  if defined (_STLP_NO_IOSTREAMS)
#    error If you do not use iostreams you do not need to build the STLport library.
#  endif
#endif

/* ========================================================= */
/* This file is used for compatibility; it accepts old-style config
   switches */
#include <stl/_config_compat.h>

/* Common configuration file for this particular installation. */

#include <stl/_site_config.h>

/* Use per-version compiler recognition */
#include <config/stlcomp.h>

/* ========================================================= */

/* some fixes to configuration. This also includes modifications
 * of STLport switches depending on compiler flags,
 * or settings applicable to a group of compilers, such as
 * to all who use EDG front-end.
 */
#include <config/stl_confix.h>

#ifdef _STLP_USE_BOOST_SUPPORT
/* We are going to use the boost library support. To limit the problem
 * of self referencing headers we have to specify clearly to the boost
 * library that the Standard lib is STLport:
 */
#  ifndef BOOST_STDLIB_CONFIG
#    define BOOST_STDLIB_CONFIG <boost/config/stdlib/stlport.hpp>
#  endif
#endif


/*
 * Performs integrity check on user-specified parameters
 * and site-specific settings.
 */
/* # include <stl/_check_config.h>
 */

/* SGI terms */

#if !defined (_STLP_NO_MEMBER_TEMPLATES) && !defined (_STLP_MEMBER_TEMPLATES)
#  define _STLP_MEMBER_TEMPLATES 1
#endif

#if !defined (_STLP_NO_FRIEND_TEMPLATES) && !defined (_STLP_FRIEND_TEMPLATES)
#  define _STLP_FRIEND_TEMPLATES 1
#endif

#if !defined (_STLP_NO_MEMBER_TEMPLATE_CLASSES) && !defined (_STLP_MEMBER_TEMPLATE_CLASSES)
#  define _STLP_MEMBER_TEMPLATE_CLASSES 1
#endif

#if defined (_STLP_NO_MEMBER_TEMPLATE_CLASSES) && !defined (_STLP_DONT_SUPPORT_REBIND_MEMBER_TEMPLATE)
#  define _STLP_DONT_SUPPORT_REBIND_MEMBER_TEMPLATE 1
#endif

#if !defined (_STLP_NO_CLASS_PARTIAL_SPECIALIZATION) && !defined (_STLP_CLASS_PARTIAL_SPECIALIZATION)
#  define _STLP_CLASS_PARTIAL_SPECIALIZATION 1
#endif

#if !defined (_STLP_FUNCTION_TMPL_PARTIAL_ORDER) && !defined (_STLP_NO_FUNCTION_TMPL_PARTIAL_ORDER)
#  define _STLP_FUNCTION_TMPL_PARTIAL_ORDER 1
#endif

#if !defined (_STLP_DONT_USE_SHORT_STRING_OPTIM) && !defined (_STLP_USE_SHORT_STRING_OPTIM)
#  define _STLP_USE_SHORT_STRING_OPTIM 1
#endif

#if !defined (_STLP_DONT_USE_TEMPLATE_EXPRESSION) && !defined (_STLP_NO_MEMBER_TEMPLATE_CLASSES)
#  define _STLP_USE_TEMPLATE_EXPRESSION
#endif

#if defined (_STLP_USE_PTR_SPECIALIZATIONS) && \
    (defined (_STLP_NO_CLASS_PARTIAL_SPECIALIZATION) && defined (_STLP_DONT_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS))
#  error "Sorry but according the STLport settings your compiler can not support the pointer specialization feature."
#endif

#if !defined (_STLP_BIG_ENDIAN) && ! defined (_STLP_LITTLE_ENDIAN)
#  if defined (_MIPSEB) || defined (__sparc) || defined (_AIX) || \
      defined (__hpux) || defined (macintosh) || defined (_MAC)
#    define _STLP_BIG_ENDIAN 1
#  elif defined (__i386) || defined (_M_IX86) || defined (_M_ARM) || \
        defined (__amd64__) || defined (_M_AMD64) || defined (__x86_64__) || \
        defined (__alpha__)
#    define _STLP_LITTLE_ENDIAN 1
#  elif defined (__ia64__)
    /* itanium allows both settings (for instance via gcc -mbig-endian) - hence a seperate check is required */
#    if defined (__BIG_ENDIAN__)
#      define _STLP_BIG_ENDIAN 1
#    else
#      define _STLP_LITTLE_ENDIAN 1
#    endif
#  endif
#endif /* _STLP_BIG_ENDIAN */

/* ==========================================================
 * final workaround tuning based on given flags
 * ========================================================== */

#ifndef _STLP_UINT32_T
#  define _STLP_UINT32_T unsigned long
#endif
#ifndef _STLP_ABORT
#  define _STLP_ABORT() abort()
#endif

#if !defined (_STLP_HAS_NO_NAMESPACES)
#  if defined _STLP_NO_NAMESPACES
#    undef _STLP_USE_NAMESPACES
#  else
/* assume it as the default, turn it off later if NO_NAMESPACES selected */
#    undef _STLP_USE_NAMESPACES
#    define _STLP_USE_NAMESPACES 1
#  endif
#endif

#if defined (_STLP_NO_IOSTREAMS)
#  define _STLP_USE_NO_IOSTREAMS
#endif

/* Operating system recognition (basic) */
#if defined (__unix) || defined (__linux__) || defined (__QNX__) || defined (_AIX)  || defined (__NetBSD__) || defined(__OpenBSD__) || defined (__Lynx__)
#  define _STLP_UNIX 1
#  if defined (__linux__)
/* This is defined wether library in use is glibc or not.
   This may be treated as presence of GNU libc compatible
   header files (these define is not really intended to check
   for the presence of a particular library, but rather is used
   to define an INTERFACE.) */
#    ifndef _STLP_USE_GLIBC
#      define _STLP_USE_GLIBC 1
#    endif

#    if defined(__UCLIBC__) && !defined(_STLP_USE_UCLIBC)
#      define _STLP_USE_UCLIBC 1
#    endif

#    ifdef _STLP_USE_UCLIBC
     /* see into uClib (c runtime lib) configuration */
#      if !defined(__UCLIBC_HAS_WCHAR__)
#        ifndef _STLP_NO_WCHAR_T
#          define _STLP_NO_WCHAR_T
#        endif
#        ifndef _STLP_NO_MBSTATE_T
#          define _STLP_NO_MBSTATE_T
#        endif
#        ifndef _STLP_NO_NATIVE_WIDE_STREAMS
#          define _STLP_NO_NATIVE_WIDE_STREAMS
#        endif
#      endif /* __UCLIBC_HAS_WCHAR__ */
     /* Hmm, bogus _GLIBCPP_USE_NAMESPACES seems undefined... */
#      define _STLP_VENDOR_GLOBAL_CSTD 1
#      if defined(_STLP_REAL_LOCALE_IMPLEMENTED)
      /* locale in uClibc is very restricted */
      /* recheck if __UCLIBC_HAS_LOCALE__ defined...*/
#        undef _STLP_REAL_LOCALE_IMPLEMENTED
#      endif
#      ifndef _STLP_DONT_USE_PTHREAD_SPINLOCK
      /* in uClibc (0.9.26) pthread_spinlock* declared in headers
       * but absent in library */
#        define _STLP_DONT_USE_PTHREAD_SPINLOCK
#      endif
#    endif /* _STLP_USE_UCLIBC */
#  endif /* __linux__ */
#elif defined(macintosh) || defined (_MAC)
#  define _STLP_MAC  1
#elif defined (_WIN32) || defined (__WIN32) || defined (WIN32) || defined (__WIN32__)
#  define _STLP_WIN32 1
#elif defined (__WIN16) || defined (WIN16) || defined (_WIN16)
#  define _STLP_WIN16
#endif /* __unix */

#if defined (_STLP_WIN16)
#  define _STLP_LDOUBLE_80
#elif defined(_STLP_WIN32)
#  if defined (_STLP_MSVC) || defined (__ICL) || defined (__BORLANDC__) || defined (__CYGWIN__)
#    define _STLP_LDOUBLE_64
#  else
#    define _STLP_LDOUBLE_96
#  endif
#elif defined (_STLP_UNIX)
#  if defined (__CYGWIN__)
#    define _STLP_LDOUBLE_96
#  endif
#endif

#if !defined (_STLP_LDOUBLE_64) && !defined (_STLP_LDOUBLE_80) && !defined (_STLP_LDOUBLE_96) && !defined (_STLP_LDOUBLE_128)
#  define _STLP_LDOUBLE_128
#endif /*_STLP_LDOUBLE_??*/

#if !defined (_STLP_NO_LONG_DOUBLE)
#  define _STLP_LONG_DOUBLE long double
#else
#  define _STLP_LONG_DOUBLE double
#endif

#if !defined(_STLP_MAKE_HEADER)
#  define _STLP_MAKE_HEADER(path, header) <path/header>
#endif

#if !defined (_STLP_NATIVE_HEADER)
#  if !defined (_STLP_NATIVE_INCLUDE_PATH)
#    define _STLP_NATIVE_INCLUDE_PATH ../include
#  endif
#  define _STLP_NATIVE_HEADER(header) _STLP_MAKE_HEADER(_STLP_NATIVE_INCLUDE_PATH,header)
#endif

/* For some compilers, C headers like <stdio.h> are located in separate directory */
#if !defined (_STLP_NATIVE_C_HEADER)
#  if !defined (_STLP_NATIVE_C_INCLUDE_PATH)
#    define _STLP_NATIVE_C_INCLUDE_PATH _STLP_NATIVE_INCLUDE_PATH
#  endif
#  define _STLP_NATIVE_C_HEADER(header)  _STLP_MAKE_HEADER(_STLP_NATIVE_C_INCLUDE_PATH,header)
#endif

/* For some compilers, C-library headers like <cstdio> are located in separate directory */
#if !defined (_STLP_NATIVE_CPP_C_HEADER)
#  if !defined (_STLP_NATIVE_CPP_C_INCLUDE_PATH)
#    define _STLP_NATIVE_CPP_C_INCLUDE_PATH _STLP_NATIVE_INCLUDE_PATH
#  endif
#  define _STLP_NATIVE_CPP_C_HEADER(header)  _STLP_MAKE_HEADER(_STLP_NATIVE_CPP_C_INCLUDE_PATH,header)
#endif

/* Some compilers locate basic C++ runtime support headers (<new>, <typeinfo>, <exception>) in separate directory */
#if !defined ( _STLP_NATIVE_CPP_RUNTIME_HEADER )
#  if !defined (_STLP_NATIVE_CPP_RUNTIME_INCLUDE_PATH)
#    define _STLP_NATIVE_CPP_RUNTIME_INCLUDE_PATH _STLP_NATIVE_INCLUDE_PATH
#  endif
#  define _STLP_NATIVE_CPP_RUNTIME_HEADER(header)  _STLP_MAKE_HEADER(_STLP_NATIVE_CPP_RUNTIME_INCLUDE_PATH,header)
#endif

/*  shared library tune-up */

#if defined (__BUILDING_STLPORT)
/*  if we are rebuilding right now, place everything here */
#  undef  _STLP_DESIGNATED_DLL
#  define _STLP_DESIGNATED_DLL 1
#endif

/* Use own namespace always if possible and not explicitly instructed otherwise */
#if defined (_STLP_USE_NAMESPACES) && !defined (_STLP_BROKEN_USING_DIRECTIVE) && \
   !defined (_STLP_NO_OWN_NAMESPACE)
#  undef  _STLP_USE_OWN_NAMESPACE
#  define _STLP_USE_OWN_NAMESPACE  1
#else
#  undef _STLP_WHOLE_NATIVE_STD
#endif

#if !defined (_NOTHREADS) && !defined (_STLP_THREADS_DEFINED)

#  if defined (_PTHREADS)
#    define _STLP_PTHREADS
#    define _STLP_THREADS
#  endif
#  if defined (_UITHREADS)
#    define _STLP_UITHREADS
#    define _STLP_THREADS
#  endif

#  if defined (__sgi) && !defined (__KCC) && !defined (__GNUC__)
#    define _STLP_SGI_THREADS
#  elif defined (__DECC) || defined (__DECCXX)
#    define _STLP_DEC_THREADS
#  elif defined (_STLP_WIN32) && !defined (_STLP_PTHREADS)
#    define _STLP_WIN32THREADS 1
#  elif ((defined (__sun) && !defined (__linux__)) || defined (_UITHREADS) ) && \
        !defined(_STLP_PTHREADS)
#    define _STLP_UITHREADS
#  elif defined (__OS2__)
#    define _STLP_OS2THREADS
#  elif defined (__BEOS__)
#    define _STLP_BETHREADS
#  elif defined (__MWERKS__) && defined (N_PLAT_NLM) /* (__dest_os == __netware_os) */
#    define _STLP_NWTHREADS
#  else
#    define _STLP_PTHREADS
#  endif /* __sgi */
#  define _STLP_THREADS_DEFINED
#endif

#if (defined (_REENTRANT) || defined (_THREAD_SAFE) || \
    (defined (_POSIX_THREADS) && defined (__OpenBSD__))) && \
    !defined (_STLP_THREADS)
#  define _STLP_THREADS
#endif /* _REENTRANT */

#if defined (__linux__) && defined (_STLP_PTHREADS)
/* #  include <features.h> */

#  if defined (__USE_XOPEN2K) && !defined (_STLP_DONT_USE_PTHREAD_SPINLOCK)
#    define _STLP_USE_PTHREAD_SPINLOCK
#    define _STLP_STATIC_MUTEX _STLP_mutex
#  endif /* __USE_XOPEN2K */
#endif /* __linux__ && _STLP_PTHREADS */

#if defined (__OpenBSD__) && defined (_POSIX_THREADS) && !defined (_STLP_DONT_USE_PTHREAD_SPINLOCK)
#  define _STLP_USE_PTHREAD_SPINLOCK
#  define _STLP_STATIC_MUTEX _STLP_mutex
#endif

#ifndef _STLP_STATIC_MUTEX
#  define _STLP_STATIC_MUTEX _STLP_mutex_base
#endif

#if defined (_MFC_VER) && !defined (_STLP_USE_MFC)
#  define _STLP_USE_MFC 1
#endif

#if defined (_STLP_THREADS)
#  define _STLP_VOLATILE volatile
/* windows.h _MUST be included before bool definition ;( */
#  if defined (_STLP_WIN32THREADS) && defined (_STLP_NO_BOOL)
#    undef  NOMINMAX
#    define NOMINMAX
#    ifdef _STLP_USE_MFC
#      include <afx.h>
#    else
#      include <windows.h>
#    endif
#    define _STLP_WINDOWS_H_INCLUDED
#  endif
#else
#  define _STLP_VOLATILE
#endif

#if !defined (_STLP_USE_NEW_C_HEADERS) && !defined (_STLP_HAS_NO_NEW_C_HEADERS)
#  define _STLP_USE_NEW_C_HEADERS
#endif
/* disable new-style headers if requested */
#if defined (_STLP_NO_NEW_C_HEADERS)
#  undef _STLP_USE_NEW_C_HEADERS
#endif

#if !defined (_STLP_STATIC_TEMPLATE_DATA)
#  define _STLP_STATIC_TEMPLATE_DATA 1
#endif

#if defined (_STLP_BASE_TYPEDEF_BUG)
#  undef  _STLP_BASE_TYPEDEF_OUTSIDE_BUG
#  define _STLP_BASE_TYPEDEF_OUTSIDE_BUG 1
#endif

#if defined (_STLP_NESTED_TYPE_PARAM_BUG) || (defined (_STLP_MSVC) && (_STLP_MSVC < 1100))
#  define _STLP_GLOBAL_NESTED_RETURN_TYPE_PARAM_BUG
#endif

/* SUNpro 4.2 inline string literal bug */
#ifdef _STLP_INLINE_STRING_LITERAL_BUG
#  define _STLP_FIX_LITERAL_BUG(__x) __x = __x;
#else
#  define _STLP_FIX_LITERAL_BUG(__x)
#endif

#if defined (_STLP_NON_TYPE_TMPL_PARAM_BUG)
#  undef  _STLP_NO_DEFAULT_NON_TYPE_PARAM
#  define _STLP_NO_DEFAULT_NON_TYPE_PARAM 1
#endif

#define _STLP_NEW new
#define _STLP_PLACEMENT_NEW new

#ifdef _STLP_DEBUG
#  define _STLP_ASSERTIONS 1
#endif

#if !defined (_STLP_STATIC_ASSERT)
/* Some compiler support 0 size array so we use negative size array to generate
 * a compilation time error.
 */
#  define _STLP_STATIC_ASSERT(expr) typedef char __static_assert[expr ? 1 : -1];
#endif

/* apple mpw exception handling bug */
#ifndef _STLP_MPWFIX_TRY
#  define _STLP_MPWFIX_TRY
#endif
#ifndef _STLP_MPWFIX_CATCH
#  define _STLP_MPWFIX_CATCH
#endif
#ifndef _STLP_MPWFIX_CATCH_ACTION
#  define _STLP_MPWFIX_CATCH_ACTION(action)
#endif

/* if _STLP_DEBUG or _STLP_ASSERTIONS are set, stl/debug/_debug.h defines those */

#if !defined (_STLP_ASSERTIONS) && !defined (_STLP_DEBUG) && !defined (_STLP_DEBUG_ALLOC)
#  define _STLP_ASSERT(expr)
#endif

#if !defined (_STLP_DEBUG)
#  define _STLP_VERBOSE_ASSERT(expr,diagnostic)
#  define _STLP_DEBUG_CHECK(expr)
#  define _STLP_DEBUG_DO(expr)
#endif

#if !defined (_STLP_WEAK)
#  define _STLP_WEAK
#endif

/* default parameters as template types derived from arguments ( not always supported ) */
#if defined (_STLP_LIMITED_DEFAULT_TEMPLATES)
#  define __DFL_TMPL_PARAM( classname, defval ) class classname
#  define __DFL_TMPL_ARG(classname) , classname
#else
#  if !defined (_STLP_DEFAULT_TYPE_PARAM)
#    define _STLP_DEFAULT_TYPE_PARAM 1
#  endif
#  define __DFL_TMPL_PARAM( classname, defval ) class classname = defval
#  define __DFL_TMPL_ARG(classname)
#endif

/* default parameters as complete types */
#if defined (_STLP_DEFAULT_TYPE_PARAM)
#  define __DFL_TYPE_PARAM( classname, defval ) class classname = defval
#  define __DFL_NON_TYPE_PARAM(type,name,val) type name = val
#  define __DFL_TYPE_ARG(classname)
#else
#  define __DFL_TYPE_PARAM( classname, defval ) class classname
#  define __DFL_NON_TYPE_PARAM(type,name,val) type name
#  define __DFL_TYPE_ARG(classname) , classname
#endif

/* SGI compatibility */

#ifdef _STLP_NO_WCHAR_T
#  ifndef _STLP_NO_NATIVE_WIDE_STREAMS
#    define  _STLP_NO_NATIVE_WIDE_STREAMS 1
#  endif
#else
#  define _STLP_HAS_WCHAR_T 1
#endif

#if !defined (_STLP_NO_AT_MEMBER_FUNCTION)
#  define _STLP_CAN_THROW_RANGE_ERRORS 1
#endif

#if !defined (_STLP_USE_RAW_SGI_ALLOCATORS)
#  define _STLP_DEFAULT_ALLOCATOR(_Tp) allocator< _Tp >
#  define _STLP_DEFAULT_ALLOCATOR_SELECT( _Tp ) __DFL_TMPL_PARAM(_Alloc, allocator< _Tp >)
#  define _STLP_DEFAULT_PAIR_ALLOCATOR(_Key, _Tp) allocator< pair < _Key, _Tp > >
#  if defined (_STLP_LIMITED_DEFAULT_TEMPLATES)
#    define _STLP_DEFAULT_PAIR_ALLOCATOR_SELECT(_Key, _Tp ) class _Alloc
#    define _STLP_USE_WRAPPER_FOR_ALLOC_PARAM 1
#  else
#    define _STLP_DEFAULT_PAIR_ALLOCATOR_SELECT(_Key, _Tp ) \
            class _Alloc = allocator< pair < _Key, _Tp > >
#  endif
#else
#  define _STLP_DEFAULT_ALLOCATOR( _Tp ) __sgi_alloc
#  define _STLP_DEFAULT_ALLOCATOR_SELECT( _Tp ) __DFL_TYPE_PARAM(_Alloc,__sgi_alloc)
#  define _STLP_DEFAULT_PAIR_ALLOCATOR( _Key, _Tp ) __sgi_alloc
#  define _STLP_DEFAULT_PAIR_ALLOCATOR_SELECT(_Key, _Tp ) __DFL_TYPE_PARAM(_Alloc,__sgi_alloc)
#  if defined (_STLP_LIMITED_DEFAULT_TEMPLATES) && !defined (_STLP_DEFAULT_TYPE_PARAM)
#    define _STLP_USE_WRAPPER_FOR_ALLOC_PARAM 1
#  endif
#endif

/* debug mode tool */
#if defined (_STLP_DEBUG)
#  define _STLP_NON_DBG_NAME(X) _NonDbg_##X
#endif

/* pointer specialization tool */
#if defined (_STLP_USE_PTR_SPECIALIZATIONS)
#  define _STLP_PTR_IMPL_NAME(X) _Impl_##X
#endif

#if defined (_STLP_USE_MSVC6_MEM_T_BUG_WORKAROUND)
#  define _STLP_NO_MEM_T_NAME(X) _NoMemT_##X
#  if defined (_STLP_DEBUG)
#    define _STLP_NON_DBG_NO_MEM_T_NAME(X) _NonDbg_NoMemT_##X
#  endif
#endif

/* this always mean the C library is in global namespace */
#if defined (_STLP_HAS_NO_NEW_C_HEADERS) && !defined (_STLP_VENDOR_GLOBAL_CSTD)
#  define _STLP_VENDOR_GLOBAL_CSTD 1
#endif

/* Depending of whether compiler supports namespaces,
 * tune the parameters for vendor-supplied libraries.
 * This section is guarded by _STLP_HAS_NO_NAMESPACES, not by _STLP_USE_NAMESPACES,
 * since it depends only on the native features, not on user's preference whether
 * to use namespace for STLport or not.
 */
#if !defined (_STLP_HAS_NO_NAMESPACES)
/* Import some vendor's headers into corresponding STLport ones if they might be needed
 * (if we wrap native iostreams and use namepace other than std::) */
#  if defined (_STLP_WHOLE_NATIVE_STD)
#    define  _STLP_IMPORT_VENDOR_STD 1
#    undef   _STLP_MINIMUM_IMPORT_STD
#  endif

/* if using stlport:: namespace or if C library stuff is not in vendor's std::,
 * try importing 'em.
 * MSVC has ambiguity problem when we try to import C-style std:: stuff back into global namespace */
#  if defined (_STLP_USE_NAMESPACES) && (defined(_STLP_USE_OWN_NAMESPACE) || defined (_STLP_VENDOR_GLOBAL_CSTD))
#    define  _STLP_IMPORT_VENDOR_CSTD 1
#  endif

#  if defined (_STLP_NO_USING_FOR_GLOBAL_FUNCTIONS) && !defined (_STLP_DO_IMPORT_CSTD_FUNCTIONS)
#    define _STLP_NO_CSTD_FUNCTION_IMPORTS
#  endif

#  define _STLP_USING_NAMESPACE(x) using namespace x ;

namespace std { }
namespace __std_alias = std;

/* assume std:: namespace for C++ std library if not being told otherwise */
#  if defined (_STLP_VENDOR_GLOBAL_STD)
#    define _STLP_VENDOR_STD
#  else
#    define _STLP_VENDOR_STD __std_alias
#  endif

/* tune things that come from C library */
#  if  defined (_STLP_VENDOR_GLOBAL_CSTD) || !defined(_STLP_USE_NEW_C_HEADERS)
/*  in old-style headers, C functions go to global scope. */
#    define _STLP_VENDOR_CSTD
#    define _STLP_USING_VENDOR_CSTD
#  else
#    define _STLP_VENDOR_CSTD  _STLP_VENDOR_STD
#    define _STLP_USING_VENDOR_CSTD _STLP_USING_NAMESPACE(_STLP_VENDOR_CSTD)
#  endif /* _STLP_VENDOR_CSTD */
/* exception, typeinfo, new - always come from the vendor */
#  if !defined (_STLP_VENDOR_EXCEPT_STD)
#    if defined (_STLP_VENDOR_GLOBAL_EXCEPT_STD)
#      define _STLP_VENDOR_EXCEPT_STD
#    else
#      define _STLP_VENDOR_EXCEPT_STD _STLP_VENDOR_STD
#    endif
#  endif
#  define _STLP_OLD_IO_NAMESPACE
#  if !defined (_STLP_VENDOR_MB_NAMESPACE)
#    define _STLP_VENDOR_MB_NAMESPACE _STLP_VENDOR_CSTD
#  endif
#else
/* compiler has no namespace support */
#  define _STLP_VENDOR_STD
#  define _STLP_VENDOR_CSTD
#  define _STLP_USING_NAMESPACE(x)
#  define _STLP_USING_VENDOR_CSTD
#  define _STLP_VENDOR_EXCEPT_STD
#endif

#if defined (_STLP_USE_NAMESPACES)

#  if defined (_STLP_USE_OWN_NAMESPACE)
#    if !defined (_STLP_STD_NAME)
#      if !defined (_STLP_DEBUG)
#        if !defined (_STLP_USING_CROSS_NATIVE_RUNTIME_LIB)
#          ifndef _STLP_THREADS
#            define _STLP_STD_NAME  stlpmtx_std
#          else
#            define _STLP_STD_NAME  stlp_std
#          endif
#        else
#          ifndef _STLP_THREADS
#            define _STLP_STD_NAME  stlpxmtx_std
#          else
#            define _STLP_STD_NAME  stlpx_std
#          endif
#        endif
#      else
/*
 * The STLport debug mode is binary incompatible with the other modes,
 * lets make it clear on the STLport namespace to generate link errors rather
 * than runtime.
 */
#        if !defined (_STLP_USING_CROSS_NATIVE_RUNTIME_LIB)
#          ifndef _STLP_THREADS
#            define _STLP_STD_NAME  stlpdmtx_std
#          else
#            define _STLP_STD_NAME  stlpd_std
#          endif
#        else
#          ifndef _STLP_THREADS
#            define _STLP_STD_NAME  stlpdxmtx_std
#          else
#            define _STLP_STD_NAME  stlpdx_std
#          endif
#        endif
#      endif
#    endif
namespace _STLP_STD_NAME { }
#  else
#    if defined (_STLP_DEBUG)
namespace stdD = std;
#    endif
#    define _STLP_STD_NAME std
#  endif /* _STLP_USE_OWN_NAMESPACE */

#  if !defined (_STLP_USING_NAMESPACE_BUG)
#    define _STLP_PRIV_NAME stlp_priv
namespace _STLP_PRIV_NAME {
  using namespace _STLP_STD_NAME;
}
#  else
#    define _STLP_PRIV_NAME priv
#  endif

#  define _STLP_BEGIN_NAMESPACE namespace _STLP_STD_NAME {
#  define _STLP_END_NAMESPACE }

#  if !defined (_STLP_DONT_USE_PRIV_NAMESPACE)
#    if !defined (_STLP_USING_NAMESPACE_BUG)
/* We prefer to make private namespace a totaly seperated namespace...
 */
#      define _STLP_PRIV ::_STLP_PRIV_NAME::
#      define _STLP_MOVE_TO_PRIV_NAMESPACE } namespace _STLP_PRIV_NAME {
#      define _STLP_MOVE_TO_STD_NAMESPACE } namespace _STLP_STD_NAME {
#    else
/* but sometimes we can't:
 */
#      define _STLP_PRIV _STLP_PRIV_NAME::
#      define _STLP_MOVE_TO_PRIV_NAMESPACE namespace _STLP_PRIV_NAME {
#      define _STLP_MOVE_TO_STD_NAMESPACE }
#    endif
#  else
#    define _STLP_PRIV
#    define _STLP_MOVE_TO_PRIV_NAMESPACE
#    define _STLP_MOVE_TO_STD_NAMESPACE
#  endif

/* decide whether or not we use separate namespace for rel ops */
#  if defined (_STLP_NO_RELOPS_NAMESPACE)
#    define _STLP_BEGIN_RELOPS_NAMESPACE _STLP_BEGIN_NAMESPACE namespace rel_ops {}
#    define _STLP_END_RELOPS_NAMESPACE }
#  else
/* Use std::rel_ops namespace */
#    define _STLP_BEGIN_RELOPS_NAMESPACE _STLP_BEGIN_NAMESPACE namespace rel_ops {
#    define _STLP_END_RELOPS_NAMESPACE } }
#    define _STLP_USE_SEPARATE_RELOPS_NAMESPACE
#  endif /* Use std::rel_ops namespace */

#  define _STLP_STD ::_STLP_STD_NAME

/* Official STLport namespace when std is not redefined.
 * Here we don't use a macro as stlport is used as file name by boost
 * and folder name under beos:
 */
namespace stlport = _STLP_STD_NAME;

/* Backward compatibility:
 */
namespace _STL = _STLP_STD_NAME;
#undef __STLPORT_NAMESPACE
#define __STLPORT_NAMESPACE _STLP_STD_NAME

#else /* _STLP_USE_NAMESPACES */
/* STLport is being put into global namespace */
#  define _STLP_STD
#  define _STLP_PRIV
#  define _STLP_BEGIN_NAMESPACE
#  define _STLP_BEGIN_ANONYMOUS_NAMESPACE
#  define _STLP_END_NAMESPACE
#  define _STLP_MOVE_TO_PRIV_NAMESPACE
#  define _STLP_MOVE_TO_STD_NAMESPACE

/* boris : it was found out that _STLP_USE_SEPARATE_RELOPS_NAMESPACE
   causes less problems than having relational operator templates in global namespace
   Please define _STLP_NO_RELOPS_NAMESPACE in stl_user_config.h if your code rely on them. */
#  if !defined (_STLP_NO_RELOPS_NAMESPACE)
#    define _STLP_USE_SEPARATE_RELOPS_NAMESPACE
#  endif
#  define _STLP_BEGIN_RELOPS_NAMESPACE
#  define _STLP_END_RELOPS_NAMESPACE
#  undef  _STLP_USE_OWN_NAMESPACE
#endif  /* _STLP_USE_NAMESPACES */

#define STLPORT_CSTD _STLP_VENDOR_CSTD
#define STLPORT      _STLP_STD_NAME

#if defined(_STLP_BOGUS_TEMPLATE_TYPE_MATCHING_BUG)
#  define _STLP_SIMPLE_TYPE(T) _stl_trivial_proxy<T>
#else
#  define _STLP_SIMPLE_TYPE(T) T
#endif

#ifndef _STLP_RAND48
#  define _STLP_NO_DRAND48
#endif

/* advanced keywords usage */
#define __C_CAST(__x, __y) ((__x)(__y))
#ifndef  _STLP_NO_NEW_STYLE_CASTS
#  define __CONST_CAST(__x,__y) const_cast<__x>(__y)
#  define __STATIC_CAST(__x,__y) static_cast<__x>(__y)
#  define __REINTERPRET_CAST(__x,__y) reinterpret_cast<__x>(__y)
#  define __DYNAMIC_CAST(__x,__y) dynamic_cast<__x>(__y)
#else
#  define __STATIC_CAST(__x,__y) __C_CAST(__x, __y)
#  define __CONST_CAST(__x,__y) __C_CAST(__x, __y)
#  define __REINTERPRET_CAST(__x,__y) __C_CAST(__x, __y)
#  define __DYNAMIC_CAST(__x,__y) __C_CAST(__x, __y)
#endif

#if defined (_STLP_NEED_TYPENAME) && ! defined (typename)
#  define typename
#endif

#if defined (_STLP_NEED_TYPENAME) || defined (_STLP_NO_TYPENAME_ON_RETURN_TYPE )
#  define _STLP_TYPENAME_ON_RETURN_TYPE
#else
#  define _STLP_TYPENAME_ON_RETURN_TYPE typename
#endif

#ifdef _STLP_NO_TYPENAME_IN_TEMPLATE_HEADER
#  define _STLP_HEADER_TYPENAME
#else
#  define _STLP_HEADER_TYPENAME typename
#endif

#ifndef _STLP_NO_MEMBER_TEMPLATE_KEYWORD
#  define _STLP_TEMPLATE template
#else
#  define _STLP_TEMPLATE
#endif

#if defined (_STLP_NEED_EXPLICIT) && !defined (explicit)
#  define explicit
#endif

#if !defined (_STLP_NEED_MUTABLE)
#  define _STLP_ASSIGN_MUTABLE(type,x,y) x = y
#else
#  define _STLP_ASSIGN_MUTABLE(type,x,y) __CONST_CAST(type,x)=y
#  define mutable
#endif

#if defined (_STLP_NO_SIGNED_BUILTINS)
/* old HP-UX doesn't understand "signed" keyword */
#  define signed
#endif

#if defined (_STLP_LOOP_INLINE_PROBLEMS)
#  define _STLP_INLINE_LOOP
#else
#  define _STLP_INLINE_LOOP inline
#endif

#define _STLP_PRIVATE public

#ifndef _STLP_NO_PARTIAL_SPECIALIZATION_SYNTAX
#  define _STLP_TEMPLATE_NULL template<>
#else
#  define _STLP_TEMPLATE_NULL
#endif

#ifdef _STLP_FUNCTION_TMPL_PARTIAL_ORDER
#  define _STLP_OPERATOR_TEMPLATE
#else
#  define _STLP_OPERATOR_TEMPLATE _STLP_TEMPLATE_NULL
#endif

#ifndef _STLP_CLASS_PARTIAL_SPECIALIZATION
/* unless we have other compiler problem, try simulating partial spec here */
#  if !defined (_STLP_DONT_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS)
#    define _STLP_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS
#  endif
/* For your own iterators, please use inheritance from iterator<> instead of these obsolete queries. */
#  if  (defined (_STLP_NESTED_TYPE_PARAM_BUG) || !defined (_STLP_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS))
#    if ! defined ( _STLP_USE_OLD_HP_ITERATOR_QUERIES )
#      define _STLP_USE_OLD_HP_ITERATOR_QUERIES
#    endif
#  elif defined ( _STLP_NO_ANACHRONISMS )
#    undef _STLP_USE_OLD_HP_ITERATOR_QUERIES
#  endif
#endif

#ifndef _STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS
#  define _STLP_NULL_TMPL_ARGS <>
# else
#  define _STLP_NULL_TMPL_ARGS
#endif

#if !defined (_STLP_ALLOCATOR_TYPE_DFL)
#  if defined (_STLP_DONT_SUP_DFLT_PARAM)
#    define _STLP_NEEDS_EXTRA_TEMPLATE_CONSTRUCTORS
#  endif
#  if defined (_STLP_NEEDS_EXTRA_TEMPLATE_CONSTRUCTORS)
#    define _STLP_ALLOCATOR_TYPE_DFL
#  else
#    define _STLP_ALLOCATOR_TYPE_DFL = allocator_type()
#  endif
#endif

/* When the compiler do not correctly initialized the basic types value in default parameters we prefer
 * to avoid them to be able to correct this bug.
 */
#if defined (_STLP_DEF_CONST_DEF_PARAM_BUG)
#  define _STLP_DONT_SUP_DFLT_PARAM 1
#endif

#if defined (__SGI_STL_NO_ARROW_OPERATOR) && ! defined (_STLP_NO_ARROW_OPERATOR)
#  define _STLP_NO_ARROW_OPERATOR
#endif

#if !defined (_STLP_CLASS_PARTIAL_SPECIALIZATION)
#  if !(defined (_STLP_NO_ARROW_OPERATOR)) && \
       !defined (_STLP_NO_MSVC50_COMPATIBILITY) && !defined (_STLP_MSVC50_COMPATIBILITY)
/* this one is needed for proper reverse_iterator<> operator ->() handling */
#    define _STLP_MSVC50_COMPATIBILITY 1
#  endif
#endif

#if defined ( _STLP_CLASS_PARTIAL_SPECIALIZATION )
#  if (defined(__IBMCPP__) && (500 <= __IBMCPP__) && (__IBMCPP__ < 600) )
#    define _STLP_DECLARE_REVERSE_ITERATORS(__reverse_iterator) \
   typedef typename _STLP_STD :: reverse_iterator<const_iterator> const_reverse_iterator; \
   typedef typename _STLP_STD :: reverse_iterator<iterator> reverse_iterator
#  elif (defined (__sgi) && ! defined (__GNUC__)) || defined (__SUNPRO_CC) || defined (__xlC__)
#    define _STLP_DECLARE_REVERSE_ITERATORS(__reverse_iterator) \
   typedef _STLP_STD:: _STLP_TEMPLATE reverse_iterator<const_iterator> const_reverse_iterator; \
   typedef _STLP_STD:: _STLP_TEMPLATE reverse_iterator<iterator> reverse_iterator
#  else
#    define _STLP_DECLARE_REVERSE_ITERATORS(__reverse_iterator) \
   typedef _STLP_STD::reverse_iterator<const_iterator> const_reverse_iterator; \
   typedef _STLP_STD::reverse_iterator<iterator> reverse_iterator
#  endif
#else /* _STLP_CLASS_PARTIAL_SPECIALIZATION */
#  if defined (_STLP_MSVC50_COMPATIBILITY)
#    define _STLP_DECLARE_REVERSE_ITERATORS(__reverse_iterator) \
  typedef _STLP_STD::__reverse_iterator<const_iterator, value_type, const_reference, \
    const_pointer, difference_type>  const_reverse_iterator; \
  typedef _STLP_STD::__reverse_iterator<iterator, value_type, reference, pointer, difference_type> \
    reverse_iterator
#  else
#    define _STLP_DECLARE_REVERSE_ITERATORS(__reverse_iterator) \
  typedef _STLP_STD::__reverse_iterator<const_iterator, value_type, const_reference, \
    difference_type>  const_reverse_iterator; \
  typedef _STLP_STD::__reverse_iterator<iterator, value_type, \
    reference, difference_type> \
    reverse_iterator
#  endif
#endif /* _STLP_CLASS_PARTIAL_SPECIALIZATION */

#define _STLP_DECLARE_BIDIRECTIONAL_REVERSE_ITERATORS \
        _STLP_DECLARE_REVERSE_ITERATORS(reverse_bidirectional_iterator)
#define _STLP_DECLARE_RANDOM_ACCESS_REVERSE_ITERATORS \
        _STLP_DECLARE_REVERSE_ITERATORS(reverse_iterator)

#define __IMPORT_CONTAINER_TYPEDEFS(_Super)                              \
    typedef typename _Super::value_type value_type;                      \
    typedef typename _Super::size_type size_type;                        \
    typedef typename _Super::difference_type difference_type;            \
    typedef typename _Super::reference reference;                        \
    typedef typename _Super::const_reference const_reference;            \
    typedef typename _Super::pointer pointer;                            \
    typedef typename _Super::const_pointer const_pointer;                \
    typedef typename _Super::allocator_type allocator_type;


#define __IMPORT_ITERATORS(_Super)                                       \
    typedef typename _Super::iterator iterator;                          \
    typedef typename _Super::const_iterator const_iterator;

#define __IMPORT_REVERSE_ITERATORS(_Super)                                   \
    typedef typename _Super::const_reverse_iterator  const_reverse_iterator; \
    typedef typename _Super::reverse_iterator reverse_iterator;

#define  __IMPORT_SUPER_COPY_ASSIGNMENT(__derived_name, _Self, _SUPER)       \
    __derived_name(const _Super& __x) : _SUPER(__x) {}                       \
    _Self& operator=(const _Super& __x) {                                    \
        *(_Super*)this = __x;                                                \
        return *this;                                                        \
    }                                                                        \
    __derived_name(const _Self& __x) : _SUPER(__x) {}                        \
    _Self& operator=(const _Self& __x) {                                     \
        *(_Super*)this = __x;                                                \
        return *this;                                                        \
    }

#define __IMPORT_WITH_ITERATORS(_Super) \
  __IMPORT_CONTAINER_TYPEDEFS(_Super) __IMPORT_ITERATORS(_Super)

#define __IMPORT_WITH_REVERSE_ITERATORS(_Super) \
  __IMPORT_WITH_ITERATORS(_Super) __IMPORT_REVERSE_ITERATORS(_Super)

#if defined (_STLP_TRIVIAL_CONSTRUCTOR_BUG)
#  define __TRIVIAL_CONSTRUCTOR(__type) __type() {}
#else
#  define __TRIVIAL_CONSTRUCTOR(__type)
#endif

#if defined (_STLP_TRIVIAL_DESTRUCTOR_BUG)
#  define __TRIVIAL_DESTRUCTOR(__type) ~__type() {}
#else
#  define __TRIVIAL_DESTRUCTOR(__type)
#endif

#define __TRIVIAL_STUFF(__type)  \
  __TRIVIAL_CONSTRUCTOR(__type) __TRIVIAL_DESTRUCTOR(__type)

#if defined (_STLP_HAS_NO_EXCEPTIONS)
#  define _STLP_NO_EXCEPTIONS
#endif

#if !defined (_STLP_DONT_USE_EXCEPTIONS) && !defined (_STLP_NO_EXCEPTIONS) && !defined (_STLP_USE_EXCEPTIONS)
#  define _STLP_USE_EXCEPTIONS
#endif

#if defined (_STLP_USE_EXCEPTIONS)
#  define _STLP_TRY try
#  define _STLP_CATCH_ALL catch(...)
#  ifndef _STLP_THROW
#    define _STLP_THROW(x) throw x
#  endif
#  define _STLP_RETHROW throw

#  define _STLP_UNWIND(action) catch(...) { action; throw; }

#  ifdef _STLP_THROW_RETURN_BUG
#    define _STLP_RET_AFTER_THROW(data) return data;
#  else
#    define _STLP_RET_AFTER_THROW(data)
#  endif

/* We do not use exception throw specifications unless we are forced to */
#  if !defined (_STLP_THROWS)
#    define _STLP_THROWS(x)
#  endif
#  if !defined (_STLP_NOTHROW)
#    define _STLP_NOTHROW
#  endif
#else
#  define _STLP_TRY
#  define _STLP_CATCH_ALL if (false)
#  ifndef _STLP_THROW
#    define _STLP_THROW(x)
#  endif
#  define _STLP_RETHROW {}
#  define _STLP_UNWIND(action)
#  define _STLP_THROWS(x)
#  define _STLP_NOTHROW
#  define _STLP_RET_AFTER_THROW(data)
#endif

/*
 * Here we check _STLP_NO_EXCEPTIONS which means that the compiler has no
 * exception support but not the _STLP_USE_EXCEPTIONS which simply means
 * that the user do not want to use them.
 */
#if !defined (_STLP_NO_EXCEPTIONS) && !defined (_STLP_NO_EXCEPTION_SPEC)
#  define _STLP_THROWS_INHERENTLY(x) throw x
#  define _STLP_NOTHROW_INHERENTLY throw()
#else
#  define _STLP_THROWS_INHERENTLY(x)
#  define _STLP_NOTHROW_INHERENTLY
#endif

#if defined(_STLP_NO_BOOL)
#  if (defined (__IBMCPP__) && (__IBMCPP__ < 400)) && ! defined (_AIX)
#    include <isynonym.hpp>
#    if defined (__OS400__)
typedef int bool;
#    elif !( defined (__xlC__) || defined (_AIX))
typedef Boolean bool;
#    endif
#  else
#    if defined(_STLP_YVALS_H)
#      include <yvals.h>
#    else
#      if defined (_STLP_DONT_USE_BOOL_TYPEDEF)
#        define bool int
#      else
typedef int bool;
#      endif
#      define true 1
#      define false 0
#    endif
#  endif /* __IBMCPP__ */
#else
#  define _STLP_BOOL_KEYWORD 1
#endif /* _STLP_NO_BOOL */

#ifndef _STLP_MPW_EXTRA_CONST
#  define _STLP_MPW_EXTRA_CONST
#endif

#ifndef _STLP_DEFAULTCHAR
#  define _STLP_DEFAULTCHAR char
#endif

#if defined (_STLP_DEBUG_ALLOC) && ! defined (_STLP_ASSERTIONS)
#  define _STLP_ASSERTIONS 1
#endif

#if defined (_STLP_DEBUG) || defined (_STLP_ASSERTIONS)
#  if defined (_STLP_DEBUG_MODE_THROWS) && !defined (_STLP_DEBUG_TERMINATE)
#    define _STLP_DEBUG_TERMINATE
#  endif
#endif

/* uninitialized value filler */
#ifndef _STLP_SHRED_BYTE
/* This value is designed to cause problems if an error occurs */
#  define _STLP_SHRED_BYTE 0xA3
#endif /* _STLP_SHRED_BYTE */

/* shared library tune-up */
#ifndef _STLP_IMPORT_DECLSPEC
#  define _STLP_IMPORT_DECLSPEC
#endif

/* a keyword used to instantiate export template */
#ifndef _STLP_EXPORT_TEMPLATE_KEYWORD
#  define _STLP_EXPORT_TEMPLATE_KEYWORD
#endif
#ifndef _STLP_IMPORT_TEMPLATE_KEYWORD
#  define _STLP_IMPORT_TEMPLATE_KEYWORD
#endif

#ifdef _STLP_USE_NO_IOSTREAMS
/*
 * If we do not use iostreams we do not use the export/import
 * techniques to avoid build of the STLport library.
 */
#  undef _STLP_USE_DECLSPEC
/* We also undef USE_DYNAMIC_LIB macro as this macro add some code
 * to use the dynamic (shared) STLport library for some platform/compiler
 * configuration leading to problem when do not link to the STLport lib.
 */
#  undef _STLP_USE_DYNAMIC_LIB
#endif

#if  defined (_STLP_DLLEXPORT_NEEDS_PREDECLARATION) && defined (_STLP_USE_DECLSPEC)
#  if ! defined (_STLP_USE_TEMPLATE_EXPORT)
/* this setting turns on "extern template" extension use */
#    define _STLP_USE_TEMPLATE_EXPORT
#  endif
#  if defined (_STLP_DESIGNATED_DLL) && ! defined (_STLP_NO_FORCE_INSTANTIATE)
#    define _STLP_NO_FORCE_INSTANTIATE
#  endif
#endif

#if defined (_STLP_DESIGNATED_DLL) /* This is a lib which will contain STLport exports */
#  define  _STLP_EXPORT _STLP_EXPORT_TEMPLATE_KEYWORD
#else
#  define  _STLP_EXPORT _STLP_IMPORT_TEMPLATE_KEYWORD
#endif

#ifndef _STLP_EXPORT_TEMPLATE
#  define  _STLP_EXPORT_TEMPLATE _STLP_EXPORT template
#endif

#if defined (_STLP_USE_DECLSPEC) /* using export/import technique */

#  ifndef _STLP_EXPORT_DECLSPEC
#    define _STLP_EXPORT_DECLSPEC
#  endif
#  ifndef _STLP_IMPORT_DECLSPEC
#    define _STLP_IMPORT_DECLSPEC
#  endif
#  ifndef _STLP_CLASS_EXPORT_DECLSPEC
#    define _STLP_CLASS_EXPORT_DECLSPEC
#  endif
#  ifndef _STLP_CLASS_IMPORT_DECLSPEC
#    define _STLP_CLASS_IMPORT_DECLSPEC
#  endif
#  if defined (_STLP_DESIGNATED_DLL) /* This is a lib which will contain STLport exports */
#    define  _STLP_DECLSPEC        _STLP_EXPORT_DECLSPEC
#    define  _STLP_CLASS_DECLSPEC  _STLP_CLASS_EXPORT_DECLSPEC
#  else
#    define  _STLP_DECLSPEC        _STLP_IMPORT_DECLSPEC   /* Other modules, importing STLport exports */
#    define  _STLP_CLASS_DECLSPEC  _STLP_CLASS_IMPORT_DECLSPEC
#  endif

#else /* Not using DLL export/import specifications */

#  define _STLP_DECLSPEC
#  define _STLP_CLASS_DECLSPEC

#endif

#define _STLP_EXPORT_TEMPLATE_CLASS _STLP_EXPORT template class _STLP_CLASS_DECLSPEC

#if defined (_STLP_MSVC) || defined (__ICL)
#  define _STLP_STATIC_MEMBER_DECLSPEC
#else
#  define _STLP_STATIC_MEMBER_DECLSPEC _STLP_DECLSPEC
#endif

#if !defined (_STLP_CALL)
#  define _STLP_CALL
#endif

#ifndef _STLP_USE_NO_IOSTREAMS

#  if defined (__DECCXX) && ! defined (__USE_STD_IOSTREAM)
#    define __USE_STD_IOSTREAM
#  endif

/* We only need to expose details of streams implementation
   if we use non-standard i/o or are building STLport*/
#  if defined (__BUILDING_STLPORT) || defined (_STLP_NO_FORCE_INSTANTIATE) || !defined(_STLP_NO_CUSTOM_IO)
#    define _STLP_EXPOSE_STREAM_IMPLEMENTATION 1
#  endif

/* We only need to expose details of global implementation if we are building STLport
   or have not instantiated everything in the lib */
#  if defined (__BUILDING_STLPORT) || defined (_STLP_NO_FORCE_INSTANTIATE)
#    undef  _STLP_EXPOSE_GLOBALS_IMPLEMENTATION
#    define _STLP_EXPOSE_GLOBALS_IMPLEMENTATION 1
#  endif

#else /* _STLP_USE_NO_IOSTREAMS */
/* when we are not using SGI iostreams, we must expose globals, but not streams implementation */
#  define _STLP_EXPOSE_GLOBALS_IMPLEMENTATION
#endif /* _STLP_USE_NO_IOSTREAMS */

#ifdef _STLP_PARTIAL_SPEC_NEEDS_TEMPLATE_ARGS
#  define _STLP_PSPEC2(t1,t2) < t1,t2 >
#  define _STLP_PSPEC3(t1,t2,t3) < t1,t2,t3 >
#else
#  define _STLP_PSPEC2(t1,t2)  /* nothing */
#  define _STLP_PSPEC3(t1,t2,t3)  /* nothing */
#endif

/* Activation of the partial template workaround:
 */
#if !defined(_STLP_DONT_USE_PARTIAL_SPEC_WRKD) &&\
   (!defined(_STLP_CLASS_PARTIAL_SPECIALIZATION) || !defined(_STLP_FUNCTION_TMPL_PARTIAL_ORDER))
#  define _STLP_USE_PARTIAL_SPEC_WORKAROUND
#endif

#ifndef _STLP_USE_NO_IOSTREAMS
#  define _STLP_NEW_IO_NAMESPACE _STLP_STD
#  define _STLP_NO_WIDE_STREAMS  _STLP_NO_WCHAR_T
#endif

#ifdef _STLP_USE_SEPARATE_RELOPS_NAMESPACE
#  define _STLP_RELOPS_OPERATORS(_TMPL, _TP) \
_TMPL inline bool _STLP_CALL operator!=(const _TP& __x, const _TP& __y) {return !(__x == __y);}\
_TMPL inline bool _STLP_CALL operator>(const _TP& __x, const _TP& __y)  {return __y < __x;}\
_TMPL inline bool _STLP_CALL operator<=(const _TP& __x, const _TP& __y) { return !(__y < __x);}\
_TMPL inline bool _STLP_CALL operator>=(const _TP& __x, const _TP& __y) { return !(__x < __y);}
#else
#  define _STLP_RELOPS_OPERATORS(_TMPL, _TP)
#endif

#if defined ( _STLP_USE_ABBREVS )
#  include <stl/_abbrevs.h>
#endif

/* A really useful macro */
#define _STLP_ARRAY_SIZE(A) sizeof(A) / sizeof(A[0])
#define _STLP_ARRAY_AND_SIZE(A) A, sizeof(A) / sizeof(A[0])

/* some cleanup */
#undef _STLP_DONT_USE_BOOL_TYPEDEF
#undef _STLP_YVALS_H
#undef _STLP_LOOP_INLINE_PROBLEMS
#undef _STLP_NEED_EXPLICIT
#undef _STLP_NEED_TYPENAME
#undef _STLP_NO_NEW_STYLE_CASTS
#undef __AUTO_CONFIGURED

#endif /* _STLP_CONFIG_H */

/*
 Local Variables:
 mode:C++
 End:
*/
