/*
 * File to have Windows CE .NET (eMebedded Visual C++ 4) working with STLport
 * April 2004
 * Origin : Zdenek Nemec - zero@mapfactor.com
 */

# ifndef _STLP_WINCE_4_H
#   define _STLP_WINCE_4_H

// this flag is being used by STLport to support 
// old-fashioned Windows CE SDK (see stl_wince.h)
// DO NOT USE with eMebedded Visual C++ 4!
# ifdef _STLP_WINCE
#   undef _STLP_WINCE
# endif

// new STLport Windows CE .NET define
# undef _STLP_WCE_NET
# define _STLP_WCE_NET UNDER_CE

// always threaded in Windows CE .NET
# ifndef _MT                            
#   define _MT
# endif

// workaround Pocket PC 2003 missing RTTI support in OS image
// ----------------------------------------------------------
// Turn this on if you are targeting Pocket PC 2003 or 
// Windows CE .NET 4.2 image compiled whitout SYSGEN_RTTI 
// (at this time this bug seems to be in every SDK except 
// version 4.0) you propably want to build your own image 
// with Platform Builder to enable RTTI and exception.
//
// This define is off by default since it's recommended
// to use Microsoft RTTI patch. The patch is available at 
// http://support.microsoft.com/default.aspx?scid=kb;[LN];830482
// Do not forget to turn off /GX an /GR compiler options 
// and link agaist Ccrtrtti.lib(rtti patch lib)
// if you do not want c++ exceptions and/or this patch.

//# undef _STLP_RTTI_BUG
//# define _STLP_RTTI_BUG

// Redirect cout, cerr and clog
// ----------------------------
// If defined redirect cout, cerr and clog to 
// files stdout.txt, stderr.txt and stdlog.txt

// #define _STLP_REDIRECT_STDSTREAMS

/*------ no user serviceable parts after this point ------*/

// force exception std to std instead of __std_alias
# ifdef _STLP_VENDOR_EXCEPT_STD
#   undef _STLP_VENDOR_EXCEPT_STD
# endif
# define _STLP_VENDOR_EXCEPT_STD std

// ensure _DEBUG is defined
# if defined(DEBUG) && !defined(_DEBUG)
#   define _DEBUG
# endif

// threads
# undef _REENTRANT
#   define _REENTRANT
# undef _NOTHREADS

// use old fashioned headers (ctime vs. time.h)
# undef _STLP_NO_NEW_C_HEADERS
# define _STLP_NO_NEW_C_HEADERS

// RTTI Bug support 
# ifdef _STLP_RTTI_BUG
#   define _STLP_NO_EXCEPTION_HEADER
#   define _STLP_NO_EXCEPTIONS
#   undef _STLP_USE_EXCEPTIONS
#   ifdef _STLP_WINCE_USE_OUTPUTDEBUGSTRING
#       define _STLP_WINCE_TRACE(msg)   OutputDebugString(msg)
#   else
#       define _STLP_WINCE_TRACE(msg)   MessageBox(NULL,(msg),NULL,MB_OK)
#   endif
#   ifndef __THROW_BAD_ALLOC
#       define __THROW_BAD_ALLOC { _STLP_WINCE_TRACE(L"out of memory"); ExitThread(1); }
#   endif
# else
#   define _STLP_WINCE_TRACE(msg)   OutputDebugString(msg)
# endif

// Missing Windows CE .NET things
# ifndef _SIZE_T_DEFINED
    typedef unsigned int size_t;
#   define _SIZE_T_DEFINED
# endif

# ifndef _WCHAR_T_DEFINED
    typedef unsigned short wchar_t;
#   define _WCHAR_T_DEFINED
# endif

# ifndef _TIME_T_DEFINED
    typedef unsigned long time_t;
#   define _TIME_T_DEFINED
# endif

// ptrdiff_t is not defined in Windows CE SDK
# ifndef _PTRDIFF_T_DEFINED
    typedef int ptrdiff_t;
#   define _PTRDIFF_T_DEFINED
# endif

// clock_t is not defined in Windows CE SDK
# ifndef _CLOCK_T_DEFINED
    typedef long clock_t;
#   define _CLOCK_T_DEFINED
# endif

// Struct tm is not defined in Windows CE SDK
# ifndef _TM_DEFINED
struct tm {
        int tm_sec;     /* seconds after the minute - [0,59] */
        int tm_min;     /* minutes after the hour - [0,59] */
        int tm_hour;    /* hours since midnight - [0,23] */
        int tm_mday;    /* day of the month - [1,31] */
        int tm_mon;     /* months since January - [0,11] */
        int tm_year;    /* years since 1900 */
        int tm_wday;    /* days since Sunday - [0,6] */
        int tm_yday;    /* days since January 1 - [0,365] */
        int tm_isdst;   /* daylight savings time flag */
        };
# define _TM_DEFINED
# endif

// Struct for Windows CE FILE implementation reverse hack
// this is ugly and could be danegerous in future Windows CE .NET SDKs
# ifndef _FILECE_DEFINED
typedef struct  {     
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        
        int   _charbuf;
        int   _bufsiz;
        unsigned    _res1;
        unsigned    _res2;
        
        unsigned    _res3;
        unsigned    _res4;
        unsigned    _res5;
        void*       _file;

        unsigned    _res6;
        unsigned    _res7;
        unsigned    _res8;
        unsigned    _res9;
} _iobufce;
typedef _iobufce FILECE;
# define _FILECE_DEFINED
# endif

// Some useful routines that are missing in Windows CE SDK
# ifdef __cplusplus
extern "C"
{
# endif

  char *      __cdecl getenv(const char *);
  struct tm * __cdecl gmtime(const time_t *);
  int         __cdecl remove(const char *);
  int         __cdecl rename(const char *, const char *);
  time_t      __cdecl time(time_t *);

# ifdef __cplusplus
}

# ifndef __PLACEMENT_NEW_INLINE
inline void *__cdecl operator new(size_t, void *_P) { return (_P); }
inline void __cdecl operator delete(void *, void *)    {return; }
#   define __PLACEMENT_NEW_INLINE
# endif

// Only defined as macros in Windows CE SDK
#include _STLP_NATIVE_C_HEADER(ctype.h)

inline int (isalpha)(int c) { return _isctype(c, _ALPHA); }
inline int (isupper)(int c) { return _isctype(c, _UPPER); }
inline int (islower)(int c) { return _isctype(c, _LOWER); }
inline int (isdigit)(int c) { return _isctype(c, _DIGIT); }
inline int (isxdigit)(int c) { return _isctype(c, _HEX); }
inline int (isspace)(int c) { return _isctype(c, _SPACE); }
inline int (ispunct)(int c) { return _isctype(c, _PUNCT); }
inline int (isalnum)(int c) { return _isctype(c, _ALPHA|_DIGIT); }
inline int (isprint)(int c) { return _isctype(c, _BLANK|_PUNCT|_ALPHA|_DIGIT); }
inline int (isgraph)(int c) { return _isctype(c, _PUNCT|_ALPHA|_DIGIT); }
inline int (iscntrl)(int c) { return _isctype(c, _CONTROL); }
inline int (isascii)(int c) { return ((unsigned)(c) < 0x80); }

#   undef _isctype

inline int (iswalpha)(int c) { return iswctype(c, _ALPHA); }
inline int (iswupper)(int c) { return iswctype(c, _UPPER); }
inline int (iswlower)(int c) { return iswctype(c, _LOWER); }
inline int (iswdigit)(int c) { return iswctype(c, _DIGIT); }
inline int (iswxdigit)(int c) { return iswctype(c, _HEX); }
inline int (iswspace)(int c) { return iswctype(c, _SPACE); }
inline int (iswpunct)(int c) { return iswctype(c, _PUNCT); }
inline int (iswalnum)(int c) { return iswctype(c, _ALPHA|_DIGIT); }
inline int (iswprint)(int c) { return iswctype(c, _BLANK|_PUNCT|_ALPHA|_DIGIT); }
inline int (iswgraph)(int c) { return iswctype(c, _PUNCT|_ALPHA|_DIGIT); }
inline int (iswcntrl)(int c) { return iswctype(c, _CONTROL); }
inline int (iswascii)(int c) { return ((unsigned)(c) < 0x80); }

# endif /* __cplusplus */

# if !defined(WIN32_LEAN_AND_MEAN)       // Minimise windows includes
#   define WIN32_LEAN_AND_MEAN
# endif
# if !defined(VC_EXTRALEAN)
#   define VC_EXTRALEAN
# endif
# if !defined(STRICT)
#   define STRICT
# endif
# if !defined(NOMINMAX)
#   define NOMINMAX
# endif

# ifndef __WINDOWS__
#   include <windows.h>
# endif

# ifndef _ABORT_DEFINED
#   define _STLP_ABORT() TerminateProcess(GetCurrentProcess(), 0)
#   define _ABORT_DEFINED
# endif

# ifndef _ASSERT_DEFINED
#   define assert(expr) _STLP_ASSERT(expr)
#   define _ASSERT_DEFINED
# endif

// they say it's needed 
#include <windows.h>

# endif /* _STLP_WINCE_4_H */
