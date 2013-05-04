/*
 *
 * Copyright (c) 2013
 * Petr Ovtchenkov
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

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef _STLP_INTERNAL_TUPLE_H
#define _STLP_INTERNAL_TUPLE_H

#include <cstddef>
#include <type_traits>

_STLP_BEGIN_NAMESPACE

namespace detail {

// ------------- Index expansion to args

template <size_t... Seq>
struct _sequence
{ };

template <bool F, size_t N, size_t... Seq>
struct _sequence_0_N_aux
{
    typedef typename _sequence_0_N_aux<N==sizeof...(Seq),N,Seq...,sizeof...(Seq)>::type type;
};

template <size_t N, size_t... Seq>
struct _sequence_0_N_aux<true,N,Seq...>
{
    typedef _sequence<Seq...> type;
};

template <size_t N>
struct _sequence_0_N
{
    typedef typename _sequence_0_N_aux<N==0,N-1>::type type;
};

// -------------
} // namespace detail

template <class... Types> class tuple;
template <class T1, class T2> class tuple<T1,T2>;
template <size_t I, class T> class tuple_element;
//template <size_t I, class T> class tuple_element<I,const T>;
//template <size_t I, class T> class tuple_element<I,volatile T>;
//template <size_t I, class T> class tuple_element<I,const volatile T>;

template <size_t I, class... Types> class tuple_element<I, tuple<Types...> >;
template <size_t I, class... Types> class tuple_element<I, const tuple<Types...> >;
template <size_t I, class... Types> class tuple_element<I, volatile tuple<Types...> >;
template <size_t I, class... Types> class tuple_element<I, const volatile tuple<Types...> >;

template <size_t I, class...Args>
constexpr typename tuple_element<I,tuple<Args...> >::type& get( tuple<Args...>& b ) noexcept;
template <size_t I, class...Args>
constexpr typename tuple_element<I,const tuple<Args...> >::type& get( const tuple<Args...>& b ) noexcept;
template <size_t I, class...Args>
constexpr typename tuple_element<I,volatile tuple<Args...> >::type& get( volatile tuple<Args...>& b ) noexcept;
template <size_t I, class...Args>
constexpr typename tuple_element<I,const volatile tuple<Args...> >::type& get( const volatile tuple<Args...>& b ) noexcept;
template <size_t I, class...Args>
constexpr typename tuple_element<I,tuple<Args...> >::type&& get( tuple<Args...>&& b ) noexcept;

namespace detail {

template <size_t Idx, class T, class... Types> struct _tuple_base;
template <size_t Idx, class T> struct _tuple_base<Idx,T>;

template <size_t Idx, class T, class... Types>
struct _tuple_base :
   public _tuple_base<Idx+1, Types...>
{
    typedef T type;

    constexpr _tuple_base()
      {
        static_assert( _STLP_STD::is_default_constructible<type>::value == true, "not default constructible" );
      }
    explicit constexpr _tuple_base( typename _STLP_STD::enable_if<_STLP_STD::is_copy_constructible<type>::value,type>::type f, Types... types ) :
        _tuple_base<Idx+1, Types...>( _STLP_STD::forward<Types>(types)... ),
        value( _STLP_STD::forward<T>(f) )
      {
        // static_assert( _STLP_STD::is_copy_constructible<type>::value == true, "not copy constructible" );
      }
    template <class U, class... UTypes>
    explicit constexpr _tuple_base( typename _STLP_STD::enable_if<_STLP_STD::is_constructible<type,U&&>::value,U&&>::type f, UTypes&&... types ) :
        _tuple_base<Idx+1, Types...>( _STLP_STD::forward<UTypes>(types)... ),
        value( _STLP_STD::forward<U>(f) )
      {
        // static_assert( _STLP_STD::is_constructible<type,U&&>::value == true, "not constructible" );
      }

    constexpr _tuple_base( const _tuple_base& ) = default;
    constexpr _tuple_base( _tuple_base&& ) = default;
    template <class... UTypes,
              typename = typename _STLP_STD::enable_if<
                _STLP_STD::is_constructible<type,
                                            typename _STLP_STD::tuple_element<Idx,const _STLP_STD::tuple<UTypes...> >::type
                                            >::value,
                true_type>::type>
    constexpr _tuple_base( const tuple<UTypes...>& t ) :
        _tuple_base<Idx+1, Types...>( t ),
        value( _STLP_STD::forward<typename _STLP_STD::tuple_element<Idx,const _STLP_STD::tuple<UTypes...> >::type>(get<Idx>(t)) )
      {  }
    template <class... UTypes,
              typename = typename _STLP_STD::enable_if<
                _STLP_STD::is_constructible<type,
                                            typename _STLP_STD::tuple_element<Idx,_STLP_STD::tuple<UTypes...>&& >::type
                                            >::value,
                true_type>::type>
    constexpr _tuple_base( tuple<UTypes...>&& t ) :
        _tuple_base<Idx+1, Types...>( t ),
        value( _STLP_STD::forward<typename _STLP_STD::tuple_element<Idx,_STLP_STD::tuple<UTypes...>&& >::type>(get<Idx>(t)) )
      {  }
      
    type value;
};

template <size_t Idx, class T>
struct _tuple_base<Idx, T>
{
    typedef T type;

    constexpr _tuple_base()
      {
        static_assert( _STLP_STD::is_default_constructible<type>::value == true, "not default constructible" );
      }
    explicit constexpr _tuple_base( typename _STLP_STD::enable_if<_STLP_STD::is_copy_constructible<type>::value,type>::type f ) :
        value( _STLP_STD::forward<T>(f) )
      {
        // static_assert( _STLP_STD::is_copy_constructible<type>::value == true, "not copy constructible" );
      }
    template <class U>
    explicit constexpr _tuple_base( typename _STLP_STD::enable_if<_STLP_STD::is_constructible<type,U&&>::value,U&&>::type f ) :
        value( _STLP_STD::forward<U>(f) )
      {
        // static_assert( _STLP_STD::is_constructible<type,U&&>::value == true, "not constructible" );
      }

    constexpr _tuple_base( const _tuple_base& ) = default;
    constexpr _tuple_base( _tuple_base&& ) = default;
    template <class... UTypes,
              typename = typename _STLP_STD::enable_if<
                _STLP_STD::is_constructible<type,
                                            typename _STLP_STD::tuple_element<Idx,const _STLP_STD::tuple<UTypes...> >::type>::value,
                true_type>::type>
    constexpr _tuple_base( const tuple<UTypes...>& t ) :
        value( _STLP_STD::forward<typename _STLP_STD::tuple_element<Idx,const _STLP_STD::tuple<UTypes...> >::type>(get<Idx>(t)) )
      {  }
    template <class... UTypes,
              typename = typename _STLP_STD::enable_if<
                _STLP_STD::is_constructible<type,
                                            typename _STLP_STD::tuple_element<Idx,_STLP_STD::tuple<UTypes...>&& >::type>::value,
                true_type>::type>
    constexpr _tuple_base( tuple<UTypes...>&& t ) :
        value( _STLP_STD::forward<typename _STLP_STD::tuple_element<Idx,_STLP_STD::tuple<UTypes...>&& >::type>(get<Idx>(t)) )
      {  }

    type value;
};

#if 0
template <size_t Idx, class... Types>
struct _tuple_base<Idx, void, Types...> :
    public _tuple_base<Idx+1, Types...>
{
    typedef void type;

    constexpr _tuple_base() = default;
    explicit constexpr _tuple_base( Types... types ) :
      _tuple_base<Idx+1, Types...>( _STLP_STD::forward<Types>(types)... )
    { }
};

template <size_t Idx>
struct _tuple_base<Idx, void>
{
    typedef void type;
 
    constexpr _tuple_base() = default;
};
#endif

template <bool, size_t Count, size_t Idx, class T, class... Args>
struct _indexed_type
{
  typedef typename _indexed_type<(Count + 1) == Idx, Count + 1, Idx, Args...>::type type;
  typedef typename _indexed_type<(Count + 1) == Idx, Count + 1, Idx, Args...>::vtype vtype;
};

template <size_t Count, size_t Idx, class T, class... Args>
struct _indexed_type<true,Count,Idx,T,Args...>
{
    typedef _tuple_base<Idx,T,Args...> vtype; // private
    typedef T type;
};

} // namespace detail

template <class... Types>
class tuple :
    public detail::_tuple_base<0, Types...>
{
  public:
    constexpr tuple()
      { }
    explicit constexpr tuple( Types... types ) :
        detail::_tuple_base<0, Types...>( _STLP_STD::forward<Types>(types)... )
      { }

#if 1
    template <class... UTypes, typename = typename enable_if<sizeof...(Types) == sizeof...(UTypes),true_type>::type >
    explicit constexpr tuple( UTypes&&... types ) :
        detail::_tuple_base<0, Types...>( _STLP_STD::forward<UTypes>(types)... )
      {
        // static_assert( sizeof...(Types) == sizeof...(UTypes), "wrong number of arguments" );
      }
#endif

    constexpr tuple( const tuple& t ) = default; // :
//        detail::_tuple_base<0, Types...>( static_cast<const detail::_tuple_base<0, Types...>& >(t) )
//      { }
    constexpr tuple( tuple&& ) = default;
    template <class... UTypes>
    constexpr tuple( const tuple<UTypes...>& t ) :
        detail::_tuple_base<0, Types...>( t )
      { }
    template <class... UTypes>
    constexpr tuple( tuple<UTypes...>&& t ) :
        detail::_tuple_base<0, Types...>( _STLP_STD::forward<tuple<UTypes...> >(t) )
      { }
};

// --
#if 1
template <class T1, class T2>
class tuple<T1, T2> :
    public detail::_tuple_base<0, T1, T2>
{
  public:
    constexpr tuple()
      { }
    explicit constexpr tuple( T1 t1, T2 t2 ) :
        detail::_tuple_base<0, T1, T2>( _STLP_STD::forward<T1>(t1), _STLP_STD::forward<T2>(t2) )
      { }

#if 1
    template <class U1, class U2>
    explicit constexpr tuple( U1&& t1, U2&& t2 ) :
        detail::_tuple_base<0, T1, T2>( _STLP_STD::forward<U1>(t1), _STLP_STD::forward<U2>(t2) )
      {
        // static_assert( sizeof...(Types) == sizeof...(UTypes), "wrong number of arguments" );
      }
#endif

    constexpr tuple( const tuple& t ) = default; // :
//        detail::_tuple_base<0, Types...>( static_cast<const detail::_tuple_base<0, Types...>& >(t) )
//      { }
    constexpr tuple( tuple&& ) = default;
    template <class U1, class U2>
    constexpr tuple( const tuple<U1,U2>& t ) :
        detail::_tuple_base<0, T1, T2>( t )
      { }
    template <class U1, class U2>
    constexpr tuple( tuple<U1,U2>&& t ) :
        detail::_tuple_base<0, T1, T2>( _STLP_STD::forward<tuple<U1,U2> >(t) )
      { }
    template <class U1, class U2>
    constexpr tuple( const pair<U1,U2>& t ) :
        detail::_tuple_base<0, T1, T2>( _STLP_STD::forward<const U1>(t.first), _STLP_STD::forward<const U2>(t.second) )
      { }
    template <class U1, class U2>
    constexpr tuple( pair<U1,U2>&& t ) :
        detail::_tuple_base<0, T1, T2>( _STLP_STD::forward<U1&&>(t.first), _STLP_STD::forward<U2&&>(t.second) )
      { }
};

#endif
// --
 
template <size_t I, class... Types>
class tuple_element<I, tuple<Types...> >
{
  public:
    typedef typename enable_if<(I<sizeof...(Types)),typename detail::_indexed_type<I == 0, 0, I, Types...> >::type::type type;

  private:
    typedef typename enable_if<(I<sizeof...(Types)),typename detail::_indexed_type<I == 0, 0, I, Types...> >::type::vtype vtype;

    friend type& get<I>( tuple<Types...>& b ) noexcept;
};

/*
template <size_t I, class T>
class tuple_element<I, const T>
{
  public:
    typedef typename add_const<typename T::type>::type type;
};
*/

template <size_t I, class... Types>
class tuple_element<I, const tuple<Types...> >
{
  public:
    typedef typename add_const<typename enable_if<(I<sizeof...(Types)),typename detail::_indexed_type<I == 0, 0, I, Types...> >::type::type>::type type;

  private:
    typedef typename enable_if<(I<sizeof...(Types)),typename detail::_indexed_type<I == 0, 0, I, Types...> >::type::vtype vtype;

    friend type& get<I>( const tuple<Types...>& b ) noexcept;
};

template <size_t I, class... Args>
class tuple_element<I, volatile tuple<Args...> >
{
  public:
    typedef typename add_volatile<typename enable_if<(I<sizeof...(Args)),typename detail::_indexed_type<I == 0, 0, I, Args...> >::type::type>::type type;

  private:
    typedef typename enable_if<(I<sizeof...(Args)),typename detail::_indexed_type<I == 0, 0, I, Args...> >::type::vtype vtype;

    friend type& get<I>( volatile tuple<Args...>& b ) noexcept;
};

template <size_t I, class... Args>
class tuple_element<I, const volatile tuple<Args...> >
{
  public:
    typedef typename add_cv<typename enable_if<(I<sizeof...(Args)),typename detail::_indexed_type<I == 0, 0, I, Args...> >::type::type>::type type;

  private:
    typedef typename enable_if<(I<sizeof...(Args)),typename detail::_indexed_type<I == 0, 0, I, Args...> >::type::vtype vtype;

    friend type& get<I>( const volatile tuple<Args...>& b ) noexcept;
};

template <size_t I, class...Args>
constexpr typename tuple_element<I,tuple<Args...> >::type& get( tuple<Args...>& t ) noexcept
{
  return t.tuple_element<I,tuple<Args...> >::vtype::value;
}

template <size_t I, class...Args>
constexpr typename tuple_element<I,const tuple<Args...> >::type& get( const tuple<Args...>& t ) noexcept
{
  return t.tuple_element<I,const tuple<Args...> >::vtype::value;
}

template <size_t I, class...Args>
constexpr typename tuple_element<I,volatile tuple<Args...> >::type& get( volatile tuple<Args...>& t ) noexcept
{
  return t.tuple_element<I,volatile tuple<Args...> >::vtype::value;
}

template <size_t I, class...Args>
constexpr typename tuple_element<I,const volatile tuple<Args...> >::type& get( const volatile tuple<Args...>& t ) noexcept
{
  return t.tuple_element<I,const volatile tuple<Args...> >::vtype::value;
}

template <size_t I, class...Args>
constexpr typename tuple_element<I,tuple<Args...> >::type&& get( tuple<Args...>&& t ) noexcept
{
  return _STLP_STD::forward<typename tuple_element<I,tuple<Args...> >::type&&>(get<I>(t));
}

template <class... Types>
class tuple_size;

template <class... Types>
class tuple_size<class tuple<Types...> > :
    public integral_constant<size_t,sizeof...(Types)>
{ };

template <class... Types>
class tuple_size<class const tuple<Types...> > :
    public integral_constant<size_t,sizeof...(Types)>
{ };

template <class... Types>
class tuple_size<class volatile tuple<Types...> > :
    public integral_constant<size_t,sizeof...(Types)>
{ };

template <class... Types>
class tuple_size<class const volatile tuple<Types...> > :
    public integral_constant<size_t,sizeof...(Types)>
{ };

_STLP_END_NAMESPACE // namespace _STLP_STD

#endif // _STLP_INTERNAL_TUPLE_H

// Local Variables:
// mode: C++
// End:
