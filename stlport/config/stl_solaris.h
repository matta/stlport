
// system-dependent defines 

# if defined(__SunOS_5_5_1)
#  define _STLP_NO_NATIVE_WIDE_FUNCTIONS 1
# endif

# if defined (__SunOS_5_8) && ( __cplusplus >= 199711L)
#  define _STLP_HAS_NATIVE_FLOAT_ABS
# endif

#if defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 >= 4)
# define _STLP_RAND48 1
#endif

// For SPARC Ultra and higher, we use lightweight synchronization
# if defined (__sparc) &&  ( defined  (__sparc_v9__) || defined (__sparcv9) || defined (_STLP_ULTRASPARC)) \
     && ! defined (__STLP_NO_SGI_IOSTREAMS) && !defined(_NOTHREADS)
#  define _STLP_SPARC_SOLARIS_THREADS
# endif
