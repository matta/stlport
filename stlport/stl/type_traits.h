/*
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

#ifndef _STLP_TYPE_TRAITS_H
#define _STLP_TYPE_TRAITS_H

/*
This header file provides a framework for allowing compile time dispatch
based on type attributes. This is useful when writing template code.
For example, when making a copy of an array of an unknown type, it helps
to know if the type has a trivial copy constructor or not, to help decide
if a memcpy can be used.

The class template __type_traits provides a series of typedefs each of
which is either __true_type or __false_type. The argument to
__type_traits can be any type. The typedefs within this template will
attain their correct values by one of these means:
    1. The general instantiation contain conservative values which work
       for all types.
    2. Specializations may be declared to make distinctions between types.
    3. Some compilers (such as the Silicon Graphics N32 and N64 compilers)
       will automatically provide the appropriate specializations for all
       types.

EXAMPLE:

//Copy an array of elements which have non-trivial copy constructors
template <class T> void copy(T* source, T* destination, int n, __false_type);
//Copy an array of elements which have trivial copy constructors. Use memcpy.
template <class T> void copy(T* source, T* destination, int n, __true_type);

//Copy an array of any type by using the most efficient copy mechanism
template <class T> inline void copy(T* source,T* destination,int n) {
   copy(source, destination, n,
        typename __type_traits<T>::has_trivial_copy_constructor());
}
*/

#ifdef __WATCOMC__
# include <stl/_cwchar.h>
#endif

#ifndef _STLP_TYPE_MANIPS_H
# include <stl/type_manips.h>
#endif

_STLP_BEGIN_NAMESPACE

// Forward declarations.
template <class _Tp> struct __type_traits; 
template <int _IsPOD> struct __type_traits_aux {
   typedef __false_type    has_trivial_default_constructor;
   typedef __false_type    has_trivial_copy_constructor;
   typedef __false_type    has_trivial_assignment_operator;
   typedef __false_type    has_trivial_destructor;
   typedef __false_type    is_POD_type;
};

_STLP_TEMPLATE_NULL
struct __type_traits_aux<0> {
   typedef __false_type    has_trivial_default_constructor;
   typedef __false_type    has_trivial_copy_constructor;
   typedef __false_type    has_trivial_assignment_operator;
   typedef __false_type    has_trivial_destructor;
   typedef __false_type    is_POD_type;
};

_STLP_TEMPLATE_NULL
struct __type_traits_aux<1> { 
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

# ifdef _STLP_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS
// we make general case dependant on the fact the type is actually a pointer.
 
template <class _Tp>
struct __type_traits : __type_traits_aux<_IsPtr<_Tp>::_Ret> {};

# else

template <class _Tp>
struct __type_traits { 
   typedef __true_type     this_dummy_member_must_be_first;
                   /* Do not remove this member. It informs a compiler which
                      automatically specializes __type_traits that this
                      __type_traits template is special. It just makes sure that
                      things work if an implementation is using a template
                      called __type_traits for something unrelated. */

   /* The following restrictions should be observed for the sake of
      compilers which automatically produce type specific specializations 
      of this class:
          - You may reorder the members below if you wish
          - You may remove any of the members below if you wish
          - You must not rename members without making the corresponding
            name change in the compiler
          - Members you add will be treated like regular members unless
            you add the appropriate support in the compiler. */
   typedef __false_type    has_trivial_default_constructor;
   typedef __false_type    has_trivial_copy_constructor;
   typedef __false_type    has_trivial_assignment_operator;
   typedef __false_type    has_trivial_destructor;
   typedef __false_type    is_POD_type;
};

# endif /* _STLP_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS */

// Provide some specializations.  This is harmless for compilers that
//  have built-in __types_traits support, and essential for compilers
//  that don't.
#ifndef _STLP_NO_BOOL
_STLP_TEMPLATE_NULL struct __type_traits<bool> : __type_traits_aux<1> {};
#endif /* _STLP_NO_BOOL */
_STLP_TEMPLATE_NULL struct __type_traits<char> : __type_traits_aux<1> {};
#ifndef _STLP_NO_SIGNED_BUILTINS
_STLP_TEMPLATE_NULL struct __type_traits<signed char> : __type_traits_aux<1> {};
# endif
_STLP_TEMPLATE_NULL struct __type_traits<unsigned char> : __type_traits_aux<1> {};
#if defined ( _STLP_HAS_WCHAR_T ) && ! defined (_STLP_WCHAR_T_IS_USHORT)
_STLP_TEMPLATE_NULL struct __type_traits<wchar_t> : __type_traits_aux<1> {};
#endif /* _STLP_HAS_WCHAR_T */

_STLP_TEMPLATE_NULL struct __type_traits<short> : __type_traits_aux<1> {};
_STLP_TEMPLATE_NULL struct __type_traits<unsigned short> : __type_traits_aux<1> {};
_STLP_TEMPLATE_NULL struct __type_traits<int> : __type_traits_aux<1> {};
_STLP_TEMPLATE_NULL struct __type_traits<unsigned int> : __type_traits_aux<1> {};
_STLP_TEMPLATE_NULL struct __type_traits<long> : __type_traits_aux<1> {};
_STLP_TEMPLATE_NULL struct __type_traits<unsigned long> : __type_traits_aux<1> {};

#ifdef _STLP_LONG_LONG
_STLP_TEMPLATE_NULL struct __type_traits<_STLP_LONG_LONG> : __type_traits_aux<1> {};
_STLP_TEMPLATE_NULL struct __type_traits<unsigned _STLP_LONG_LONG> : __type_traits_aux<1> {};
#endif /* _STLP_LONG_LONG */

_STLP_TEMPLATE_NULL struct __type_traits<float> : __type_traits_aux<1> {};
_STLP_TEMPLATE_NULL struct __type_traits<double> : __type_traits_aux<1> {};

# if !defined ( _STLP_NO_LONG_DOUBLE )
_STLP_TEMPLATE_NULL struct __type_traits<long double> : __type_traits_aux<1> {};
# endif

#ifdef _STLP_CLASS_PARTIAL_SPECIALIZATION
template <class _Tp> struct __type_traits<_Tp*> : __type_traits_aux<1> {};
#endif

// The following could be written in terms of numeric_limits.  
// We're doing it separately to reduce the number of dependencies.

template <class _Tp> struct _Is_integer {
  typedef __false_type _Integral;
};

#ifndef _STLP_NO_BOOL

_STLP_TEMPLATE_NULL struct _Is_integer<bool> {
  typedef __true_type _Integral;
};

#endif /* _STLP_NO_BOOL */

_STLP_TEMPLATE_NULL struct _Is_integer<char> {
  typedef __true_type _Integral;
};

#ifndef _STLP_NO_SIGNED_BUILTINS

_STLP_TEMPLATE_NULL struct _Is_integer<signed char> {
  typedef __true_type _Integral;
};
#endif

_STLP_TEMPLATE_NULL struct _Is_integer<unsigned char> {
  typedef __true_type _Integral;
};

#if defined ( _STLP_HAS_WCHAR_T ) && ! defined (_STLP_WCHAR_T_IS_USHORT)

_STLP_TEMPLATE_NULL struct _Is_integer<wchar_t> {
  typedef __true_type _Integral;
};

#endif /* _STLP_HAS_WCHAR_T */

_STLP_TEMPLATE_NULL struct _Is_integer<short> {
  typedef __true_type _Integral;
};

_STLP_TEMPLATE_NULL struct _Is_integer<unsigned short> {
  typedef __true_type _Integral;
};

_STLP_TEMPLATE_NULL struct _Is_integer<int> {
  typedef __true_type _Integral;
};

_STLP_TEMPLATE_NULL struct _Is_integer<unsigned int> {
  typedef __true_type _Integral;
};

_STLP_TEMPLATE_NULL struct _Is_integer<long> {
  typedef __true_type _Integral;
};

_STLP_TEMPLATE_NULL struct _Is_integer<unsigned long> {
  typedef __true_type _Integral;
};

#ifdef _STLP_LONG_LONG

_STLP_TEMPLATE_NULL struct _Is_integer<_STLP_LONG_LONG> {
  typedef __true_type _Integral;
};

_STLP_TEMPLATE_NULL struct _Is_integer<unsigned _STLP_LONG_LONG> {
  typedef __true_type _Integral;
};

#endif /* _STLP_LONG_LONG */

template <class _Tp> struct _Is_float_point {
  typedef __false_type _FloatingPoint;
};

_STLP_TEMPLATE_NULL struct _Is_float_point<float> {
  typedef __true_type _FloatingPoint;
};

_STLP_TEMPLATE_NULL struct _Is_float_point<double> {
  typedef __true_type _FloatingPoint;
};

# if !defined ( _STLP_NO_LONG_DOUBLE )
_STLP_TEMPLATE_NULL struct _Is_float_point<long double> {
  typedef __true_type _FloatingPoint;
};
# endif

//Base class used for internal purpose
#ifdef _STLP_USE_PARTIAL_SPEC_WORKAROUND
class __stlp_base_class {};
#define _STLP_SIGNAL_BASE_CLASS_1 : public __stlp_base_class
#define _STLP_SIGNAL_BASE_CLASS_N , public __stlp_base_class
#else
#define _STLP_SIGNAL_BASE_CLASS_1
#define _STLP_SIGNAL_BASE_CLASS_N
#endif /* _STLP_USE_PARTIAL_SPEC_WORKAROUND */


template <class _Tp1, class _Tp2>
struct _OKToMemCpy {
  enum { _Same = _IsSame<_Tp1,_Tp2>::_Ret } ;
  typedef typename __type_traits<_Tp1>::has_trivial_assignment_operator _Tr1;
  typedef typename __type_traits<_Tp2>::has_trivial_assignment_operator _Tr2;
  typedef typename __bool2type< _Same >::_Ret _Tr3;
  typedef typename _Land3<_Tr1, _Tr2, _Tr3>::_Ret _Type;
  static _Type _Answer() { return _Type(); }
};

template <class _Tp1, class _Tp2>
inline _OKToMemCpy<_Tp1, _Tp2> _IsOKToMemCpy(_Tp1*, _Tp2*)  {
  return _OKToMemCpy<_Tp1, _Tp2>();
}

template <class _Tp> 
struct _IsPOD {
  typedef typename __type_traits<_Tp>::is_POD_type _Type;
  static _Type _Answer() { return _Type(); }
};

template <class _Tp> 
inline _IsPOD<_Tp>  _Is_POD (_Tp*) { return _IsPOD<_Tp>(); } 

template <class _Tp>
struct _DefaultZeroValue {
  typedef typename _Is_integer<_Tp>::_Integral _Tr1;
  typedef typename _Is_float_point<_Tp>::_FloatingPoint _Tr2;
  typedef typename __bool2type< _IsPtr<_Tp>::_Ret>::_Ret _Tr3;

  typedef typename _Lor3<_Tr1, _Tr2, _Tr3>::_Ret _Type;
  static _Type _Answer() { return _Type(); }
};

template <class _Tp>
inline _DefaultZeroValue<_Tp> _HasDefaultZeroValue(_Tp*) {
  return _DefaultZeroValue<_Tp>();
}

#ifdef _STLP_USE_PARTIAL_SPEC_WORKAROUND
template <class _Tp>
struct _SwapImplemented {
  typedef typename _IsConvertibleType<_Tp, __stlp_base_class>::_Type _Ret;
};
#endif /* _STLP_USE_PARTIAL_SPEC_WORKAROUND */

template <class _Tp>
struct __action_on_move {
	typedef __false_type swap;
};

class __enable_swap_on_move {};

template <class _Tp1, class _Tp2> 
struct _SwapOnMove {
	typedef typename __action_on_move<_Tp1>::swap _Enabled1;
	typedef typename _IsConvertibleType<_Tp1, __enable_swap_on_move>::_Type _Enabled2;
#ifdef _STLP_USE_PARTIAL_SPEC_WORKAROUND
  typedef typename _IsConvertibleType<_Tp1, __stlp_base_class>::_Type _Enabled3;
#else
  typedef __false_type _Enabled3;
#endif
	typedef typename _Lor3<_Enabled1, _Enabled2, _Enabled3>::_Ret _Enabled;

  enum { _Same = _IsSame<_Tp1,_Tp2>::_Ret };
  typedef typename __bool2type< _Same >::_Ret _Cond;

  typedef typename _Land3<_Enabled, _Cond, __true_type>::_Ret _Type;
	static _Type _Answer() {return _Type();}
};

template<class _Tp1, class _Tp2>
_SwapOnMove<_Tp1, _Tp2> _DoSwapOnMove (_Tp1*, _Tp2*)
{return _SwapOnMove<_Tp1, _Tp2>();}


#  ifdef _STLP_CLASS_PARTIAL_SPECIALIZATION
#   if defined (__BORLANDC__) || defined (__SUNPRO_CC) || ( defined (__MWERKS__) && (__MWERKS__ <= 0x2303)) || ( defined (__sgi) && defined (_COMPILER_VERSION)) || defined (__DMC__)
#   define _IS_POD_ITER(_It, _Tp) __type_traits< typename iterator_traits< _Tp >::value_type >::is_POD_type()
#   else
#   define _IS_POD_ITER(_It, _Tp) typename __type_traits< typename iterator_traits< _Tp >::value_type >::is_POD_type()
#   endif
#  else
#   define _IS_POD_ITER(_It, _Tp) _Is_POD( _STLP_VALUE_TYPE( _It, _Tp ) )._Answer()
#  endif

_STLP_END_NAMESPACE

#endif /* __TYPE_TRAITS_H */

// Local Variables:
// mode:C++
// End:

