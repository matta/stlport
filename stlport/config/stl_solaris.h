
// system-dependent defines 

# if defined (__SunOS_5_8) && ( __cplusplus >= 199711L)
#  define _STLP_HAS_NATIVE_FLOAT_ABS
# endif

#if defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 >= 4)
# define _STLP_RAND48 1
#endif

#if (defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 == 4)) || defined (__SunOS_5_6)
# define _STLP_WCHAR_SUNPRO_EXCLUDE 1
# define _STLP_NO_NATIVE_WIDE_FUNCTIONS 1
#endif

# if !(defined ( __KCC ) && __KCC_VERSION > 3400 ) && \
  ((defined(__SunOS_5_5_1) || defined(__SunOS_5_6) ))
#  ifndef _STLP_NO_NATIVE_MBSTATE_T
#   define _STLP_NO_NATIVE_MBSTATE_T 1
#  endif
# endif /* KCC */

// For SPARC we use lightweight synchronization
# if defined (__sparc) /* &&  (defined (_REENTRANT) || defined (_PTHREADS)) */ \
     && (defined (__GNUC__) || ! defined (_STLP_NO_OWN_IOSTREAMS) ) \
     && !defined(_NOTHREADS) && ! defined (_STLP_NO_SPARC_SOLARIS_THREADS)
#  define _STLP_SPARC_SOLARIS_THREADS
# endif

# if defined (_REENTRANT)
#  define _STLP_THREADS
# endif
