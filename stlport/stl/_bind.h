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

#ifndef _STLP_INTERNAL_BIND_H
#define _STLP_INTERNAL_BIND_H

#include <cstddef>
#include <type_traits>
#include <functional>
#include <stl/_tuple.h>

_STLP_BEGIN_NAMESPACE

namespace detail {

/*
  extract type by index; don't follow type extractor
  if condition not allow this.
 */

template <bool, size_t Count, size_t Idx, class T, class... Args>
struct _indexed_type_ex
{
    typedef typename _indexed_type_ex<(Count+1) == Idx, Count + 1, Idx, Args...>::type type;
    static type&& get(T&&, Args&&... args)
      { return _indexed_type_ex<(Count+1) == Idx, Count + 1, Idx, Args...>::get(_STLP_STD::forward<Args>(args)...); } 
};

template <size_t Count, size_t Idx, class T, class... Args>
struct _indexed_type_ex<true,Count,Idx,T,Args...>
{
    typedef T type;
    static type&& get(T&& t, Args... )
      { return _STLP_STD::forward<type>( t ); } 
};

template <size_t S, size_t Idx, class... Args>
struct _indexed_type_ex_aux0
{
    typedef typename _indexed_type_ex<(0==(Idx-1)), 0, Idx-1, Args...>::type type;
    static type&& get(Args&&... args)
      { return _indexed_type_ex<0==(Idx-1), 0, Idx-1, Args...>::get(_STLP_STD::forward<Args>(args)...); } 
};

template <size_t Idx, class... Args>
struct _indexed_type_ex_aux0<0,Idx,Args...>
{
    // typedef void type;
};

template <size_t Idx, class... Args>
struct _indexed_type_ex_aux
{
    typedef typename _indexed_type_ex_aux0<sizeof...(Args), Idx, Args...>::type type;
    static type&& get(Args&&... args)
      { return _indexed_type_ex_aux0<sizeof...(Args), Idx, Args...>::get(_STLP_STD::forward<Args>(args)...); } 
};

template <class... Args>
struct _indexed_type_ex_aux<0,Args...>
{
    typedef void type;
};

} // namespace detail

namespace detail {

template <class F, class ...BoundArgs> class binder;
template <class R, class F, class ...BoundArgs> class binder_r;

} // namespace detail

template <class F, class ...BoundArgs>
detail::binder<F,BoundArgs...> bind(F&& f, BoundArgs&&... args);
template <class R, class F, class ...BoundArgs>
detail::binder_r<R,F,BoundArgs...> bind(F&& f, BoundArgs&&... args);

namespace detail {

/*
  select and extract either V-1 element of args (V>0), or
  Idx's element of the tuple.
 */

template <int V, size_t Idx, class Tuple, class... Args>
class _get_tuple_or_arg
{
  public:
    static typename _indexed_type_ex_aux<V,Args...>::type&& get( Tuple&&, Args&&... args )
      { return _indexed_type_ex_aux<V,Args...>::get(_STLP_STD::forward<Args>(args)...); }
};

template <size_t Idx, class Tuple, class... Args>
class _get_tuple_or_arg<0,Idx,Tuple,Args...>
{
  public:
    static typename tuple_element<Idx,Tuple>::type&& get( Tuple&& t, Args&&... args )
      { return _STLP_STD::get<Idx>(_STLP_STD::forward<Tuple>(t)); }
};

template <class F, class ...BoundArgs>
class binder
{
  private:

    typedef typename _STLP_STD::decay<F>::type fd_type;

    binder( F&& f, BoundArgs&&... bound_args ) :
        _Func( _STLP_STD::move(f) ),
        _ba( _STLP_STD::forward<BoundArgs>(bound_args)... )
      { }

    template <class... Args, size_t... Seq>
    auto _call( _STLP_STD::tuple<BoundArgs...>& t, const _sequence<Seq...>&, Args&&... args ) ->
    decltype(declval<fd_type>()(
                    declval<typename conditional<
                        is_placeholder<BoundArgs>::value==0,
                        BoundArgs,
                        typename _indexed_type_ex_aux<
                          is_placeholder<BoundArgs>::value,
                          Args...>::type
                          >::type>()...))
      {
        return _Func( _get_tuple_or_arg<
                      is_placeholder<
                      typename tuple_element<Seq,_STLP_STD::tuple<BoundArgs...> >::type>::value,
                      Seq,
                      _STLP_STD::tuple<BoundArgs...>,
                      Args...>::get(_STLP_STD::forward<_STLP_STD::tuple<BoundArgs...> >(_ba),_STLP_STD::forward<Args>(args)...)...);
      }

  public:
    template <class... Args>
    auto operator()(Args&&... args) ->
              decltype(declval<fd_type>()(
                    declval<typename conditional<
                        is_placeholder<BoundArgs>::value==0,
                        BoundArgs,
                        typename _indexed_type_ex_aux<
                          is_placeholder<BoundArgs>::value,
                          Args...>::type
                          >::type>()...))
      { return _call(_ba,typename _sequence_0_N<sizeof...(BoundArgs)>::type(),_STLP_STD::forward<Args>(args)...); }

    friend detail::binder<F,BoundArgs...> bind<F,BoundArgs...>(F&&, BoundArgs&&...);

  private:
    fd_type _Func;
    _STLP_STD::tuple<BoundArgs...> _ba;
};

template <class R, class F, class ...BoundArgs>
class binder_r
{
  private:
    typedef typename _STLP_STD::decay<F>::type fd_type;

    binder_r( F&& f, BoundArgs&&... bound_args ) :
        _Func( _STLP_STD::move(f) ),
        _ba( _STLP_STD::forward<BoundArgs>(bound_args)... )
      { }

    template <class... Args, size_t... Seq>
    R _call( _STLP_STD::tuple<BoundArgs...>& t, const _sequence<Seq...>&, Args&&... args )
      {
        return _Func( _get_tuple_or_arg<
                      is_placeholder<
                      typename tuple_element<Seq,_STLP_STD::tuple<BoundArgs...> >::type>::value,
                      Seq,
                      _STLP_STD::tuple<BoundArgs...>,
                      Args...>::get(_STLP_STD::forward<_STLP_STD::tuple<BoundArgs...> >(_ba),_STLP_STD::forward<Args>(args)...)...);
      }

  public:
    template <class... Args>
    R operator()(Args&&... args)
      { return _call(_ba,typename _sequence_0_N<sizeof...(BoundArgs)>::type(),_STLP_STD::forward<Args>(args)...); }

    friend detail::binder_r<R,F,BoundArgs...> bind<R,F,BoundArgs...>(F&&, BoundArgs&&...);

  private:
    fd_type _Func;
    _STLP_STD::tuple<BoundArgs...> _ba;
};

} // namespace detail

template <class F, class ...BoundArgs>
detail::binder<F,BoundArgs...> bind(F&& f, BoundArgs&&... args)
{
  static_assert( _STLP_STD::is_constructible<typename detail::binder<F,BoundArgs...>::fd_type,F>::value == true, "not constructible" );
  return detail::binder<F,BoundArgs...>(_STLP_STD::forward<F>(f),_STLP_STD::forward<BoundArgs>(args)...);
}

template <class R, class F, class ...BoundArgs>
detail::binder_r<R,F,BoundArgs...> bind(F&& f, BoundArgs&&... args)
{ return detail::binder_r<R,F,BoundArgs...>(_STLP_STD::forward<F>(f),_STLP_STD::forward<BoundArgs>(args)...); }


_STLP_END_NAMESPACE // namespace _STLP_STD

#endif // _STLP_INTERNAL_BIND_H

// Local Variables:
// mode: C++
// End:
