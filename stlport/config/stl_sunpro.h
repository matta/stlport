// STLport configuration file
// It is internal STLport header - DO NOT include it directly

# include <config/stl_solaris.h>

#  define _STLP_LONG_LONG  long long
#  define _STLP_LINK_TIME_INSTANTIATION 1

# if ! defined(_BOOL)
#  define _STLP_NO_BOOL 1
# endif

#  if (__SUNPRO_CC >= 0x500 ) && (!defined (__SUNPRO_CC_COMPAT) || (__SUNPRO_CC_COMPAT == 5 )) \
    && defined (_STLP_NO_OWN_IOSTREAMS) && ! defined (_STLP_HAS_NO_NEW_IOSTREAMS)
#    error "The wrapper (_STLP_NO_OWN_IOSTREAMS) mode does not work well without _STLP_HAS_NO_NEW_IOSTREAMS. Please set this flag. You will also have to use -liostream option on link phase."
// #   define _STLP_HAS_NO_NEW_IOSTREAMS
#  endif

// compatibility mode stuff
# if (__SUNPRO_CC >= 0x510) && (!defined (__SUNPRO_CC_COMPAT) || (__SUNPRO_CC_COMPAT == 5 ))
#  define _STLP_NATIVE_INCLUDE_PATH ../CC/Cstd
#  define _STLP_NATIVE_CPP_RUNTIME_INCLUDE_PATH ../CC
# elif (__SUNPRO_CC >= 0x500) && (!defined (__SUNPRO_CC_COMPAT) || (__SUNPRO_CC_COMPAT == 5 ))
#  define _STLP_NATIVE_INCLUDE_PATH ../CC
# elif (defined (__SUNPRO_CC_COMPAT) && __SUNPRO_CC_COMPAT == 4)
#  define _STLP_LIMITED_DEFAULT_TEMPLATES 1
#  define _STLP_NATIVE_INCLUDE_PATH ../CC4
# else
#  define _STLP_NATIVE_INCLUDE_PATH ../CC
# endif

#  if (__SUNPRO_CC >= 0x500 ) && ( defined (_STLP_NO_NEW_IOSTREAMS) || defined (_STLP_HAS_NO_NEW_IOSTREAMS) )
// if the project is set up to use libiostream (_STLP_NO_NEW_IOSTREAMS should be defined then),
// use classic iostreams
#   define _STLP_NATIVE_OLD_STREAMS_INCLUDE_PATH ../CCios
#  endif

# if (__SUNPRO_CC < 0x530)
// those are tested and proved not to work...

#  define _STLP_STATIC_ARRAY_BUG 1
#  define _STLP_STATIC_CONST_INIT_BUG 1
#  define _STLP_NO_CLASS_PARTIAL_SPECIALIZATION 1
#  define _STLP_NO_MEMBER_TEMPLATE_CLASSES 1
#  define _STLP_USE_OLD_HP_ITERATOR_QUERIES
# endif

# if (__SUNPRO_CC < 0x520)
// this is not really true ; but use of new-style native headers bring in namespace problems,
// so it works better with our wrapper only.
#  define _STLP_HAS_NO_NEW_C_HEADERS 1
# endif

// those do not depend on compatibility
# if (__SUNPRO_CC < 0x510)
#  define _STLP_NO_TYPENAME_ON_RETURN_TYPE 1 
#  define _STLP_NONTEMPL_BASE_MATCH_BUG 1
# endif

# if (__SUNPRO_CC < 0x510) || (defined (__SUNPRO_CC_COMPAT) && (__SUNPRO_CC_COMPAT < 5))

#  define _STLP_NO_QUALIFIED_FRIENDS 1

// no partial , just for explicit one
#  define _STLP_PARTIAL_SPEC_NEEDS_TEMPLATE_ARGS
#  define _STLP_NON_TYPE_TMPL_PARAM_BUG 1

#  define _STLP_NO_MEMBER_TEMPLATES 1
#  define _STLP_NO_FRIEND_TEMPLATES 1

#  define _STLP_NO_FUNCTION_TMPL_PARTIAL_ORDER 1
#  define _STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS
#  define _STLP_NO_MEMBER_TEMPLATE_KEYWORD 1
# endif

// Features that depend on compatibility switch
# if ( __SUNPRO_CC < 0x500 ) || (defined (__SUNPRO_CC_COMPAT) && (__SUNPRO_CC_COMPAT < 5))

#  ifndef _STLP_HAS_NO_NEW_IOSTREAMS
#   define _STLP_HAS_NO_NEW_IOSTREAMS 1
#  endif
#  define _STLP_NO_NEW_NEW_HEADER 1
// #  define _STLP_NO_RELOPS_NAMESPACE
#  define _STLP_HAS_NO_NAMESPACES 1
#  define _STLP_NEED_MUTABLE  1
#  define _STLP_NO_BAD_ALLOC 1
#  define _STLP_NO_EXCEPTION_HEADER 1
#  define _STLP_NATIVE_C_INCLUDE_PATH ../include
# elif (__SUNPRO_CC < 0x510)
// #  define _STLP_NATIVE_C_HEADER(header) <../CC/##header##.SUNWCCh>
#  define _STLP_NATIVE_CPP_C_HEADER(header) <../CC/##header##.SUNWCCh>
#  define _STLP_NATIVE_C_INCLUDE_PATH /usr/include
# else
#  define _STLP_NATIVE_C_INCLUDE_PATH /usr/include
#  define _STLP_NATIVE_CPP_C_INCLUDE_PATH ../CC/std
# endif



#  if ( __SUNPRO_CC < 0x500 )

# undef _STLP_NATIVE_C_HEADER
# undef _STLP_NATIVE_CPP_C_HEADER

#   define wint_t __wint_t 
// famous CC 4.2 bug
#   define _STLP_INLINE_STRING_LITERAL_BUG 1
// /usr/include
#   define _STLP_NATIVE_C_INCLUDE_PATH ../include

// 4.2 cannot handle iterator_traits<_Tp>::iterator_category as a return type ;(
#  define _STLP_USE_OLD_HP_ITERATOR_QUERIES

// 4.2 does not like it
#  undef  _STLP_PARTIAL_SPEC_NEEDS_TEMPLATE_ARGS

#  define _STLP_LIMITED_DEFAULT_TEMPLATES 1

#  define _STLP_NEED_TYPENAME 1
#  define _STLP_NEED_EXPLICIT 1
#  define _STLP_UNINITIALIZABLE_PRIVATE 1
#  define _STLP_NO_BAD_ALLOC 1
#  define _STLP_NO_ARROW_OPERATOR 1

#  define _STLP_DEFAULT_CONSTRUCTOR_BUG 1
#  define _STLP_GLOBAL_NESTED_RETURN_TYPE_PARAM_BUG 1

#   if ( __SUNPRO_CC < 0x420 )
#    define _STLP_NO_PARTIAL_SPECIALIZATION_SYNTAX 1
#    define _STLP_NO_NEW_STYLE_CASTS 1
#    define _STLP_NO_METHOD_SPECIALIZATION 1
#    define _STLP_DONT_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS
#    if ( __SUNPRO_CC > 0x401 )
#     if (__SUNPRO_CC==0x410)
#      define _STLP_BASE_TYPEDEF_OUTSIDE_BUG  1
#     endif
#    else
   // SUNPro C++ 4.0.1
#     define _STLP_BASE_MATCH_BUG          1
#     define _STLP_BASE_TYPEDEF_BUG        1
#      if ( __SUNPRO_CC < 0x401 )
        __GIVE_UP_WITH_STL(SUNPRO_401)
#      endif
#    endif /* 4.0.1 */
#   endif /* 4.2 */

#  endif /* <  5.0 */

