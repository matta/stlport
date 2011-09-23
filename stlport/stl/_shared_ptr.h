// -*- C++ -*- Time-stamp: <2011-09-23 16:07:08 ptr>

/*
 * Copyright (c) 2011
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

#include <type_traits>
#include <iostream>
#include <iomanip>

_STLP_BEGIN_NAMESPACE

namespace detail {

struct __owner_aux_abstract
{
    virtual void unlink() = 0;
    virtual void link() = 0;
    virtual long count() = 0;
    virtual __owner_aux_abstract* ref() = 0;
};

template <class T>
class __owner_aux :
    public __owner_aux_abstract
{
  public:
    __owner_aux( T* p ) :
        _p( p ),
        _n( 1 )
      { }

    virtual void unlink()
      {
        if ( --_n == 0 ) {
          delete _p;
          delete this;
        }
      }

    virtual void link()
      { ++_n; }

    virtual long count()
      { return _n; }

    virtual __owner_aux_abstract* ref()
      { return this; }

  protected:
    T* _p;
    long _n;
};

template <class Y>
class __owner_alias_aux :
        public __owner_aux_abstract
{
  public:
    __owner_alias_aux( __owner_aux_abstract* r ) :
        _parent( r == NULL ? NULL : r->ref() )
      {
        if ( _parent != NULL ) {
          _parent->link();
        }
      }

    virtual void unlink()
      {
        if ( _parent != NULL ) {
          _parent->unlink();
        }

        delete this;
      }

    virtual void link()
      {
        if ( _parent != NULL ) {
          _parent->link();
        }
      }

     virtual long count()
      { return _parent == NULL ? 0L : _parent->count(); }

    virtual __owner_aux_abstract* ref()
      { return _parent == NULL ? NULL : _parent->ref(); }

  protected:
    __owner_aux_abstract* _parent;
};

template <class T, class D>
class __owner_aux_deleter :
    public __owner_aux<T>
{
  public:
    __owner_aux_deleter( T* p, D d ) :
        __owner_aux<T>( p ),
        _d( d )
      { }

    virtual void unlink()
      {
        if ( --__owner_aux<T>::_n == 0 ) {
          __owner_aux_deleter<T,D>::_d( __owner_aux<T>::_p );
          delete this;
        }
      }

  private:
    D _d;
};

template <class T, class D, class A>
class __owner_aux_alloc :
    public __owner_aux_deleter<T,D>
{
  public:
    __owner_aux_alloc( T* p, D d, A a ) :
        __owner_aux_deleter<T,D>( p, d ),
        _a( a )
      { }

    virtual void unlink()
      {
        if ( --__owner_aux<T>::_n == 0 ) {
          __owner_aux_deleter<T,D>::_d( __owner_aux<T>::_p );
          _a.deallocate( this, 1 );
        }
      }

  private:
    A _a;
};

} // namespace detail

template <class T> class weak_ptr;
template <class T, class D> class unique_ptr;

template <class T>
class shared_ptr
{
  public:
    typedef T element_type;

    // 20.7.2.2.1, constructors:
    /* constexpr */ shared_ptr() /* noexcept */ :
        _ref( NULL )
      { }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    explicit shared_ptr( Y* p )
      {
        try {
          _p = static_cast<T*>(p);
          _ref = new detail::__owner_aux<T>(_p);
        }
        catch ( ... ) {
          delete p;
          _ref = NULL;
          throw;
        }
      }

    template <class Y, class D,
              class = typename enable_if<is_convertible<Y*,T*>::value && is_copy_constructible<D>::value>::type>
    shared_ptr( Y* p, D d )
      {
        try {
          _p = static_cast<T*>(p);
          _ref = new detail::__owner_aux_deleter<T,D>( _p, d );
        }
        catch ( ... ) {
          d( p );
          _ref = NULL;
          throw;
        }
      }

    template <class Y, class D, class A,
              class = typename enable_if<is_convertible<Y*,T*>::value && is_copy_constructible<D>::value>::type>
    shared_ptr( Y* p, D d, A a )
      {
        try {
          void* m = a.allocate(1);
          _p = static_cast<T*>(p);
          _ref = new (m) detail::__owner_aux_alloc<T,D,A>( _p, d, a );
        }
        catch ( ... ) {
          d( p );
          a.deallocate();
          _ref = NULL;
          throw;
        }
      }

    template <class D,
              class = typename enable_if<is_copy_constructible<D>::value>::type>
    shared_ptr( nullptr_t, D d )
      {
        try {
          _p = NULL;
          _ref = new detail::__owner_aux_deleter<T,D>( NULL, d );
        }
        catch ( ... ) {
          d( NULL );
          _ref = NULL;
          throw;
        }
      }

    template <class D, class A,
              class = typename enable_if<is_copy_constructible<D>::value>::type>
    shared_ptr( nullptr_t, D d, A a )
      {
        try {
          void* m = a.allocate(1);
          _p = NULL;
          _ref = new (m) detail::__owner_aux_alloc<T,D,A>( NULL, d, a );
        }
        catch ( ... ) {
          d( NULL );
          a.deallocate();
          _ref = NULL;
          throw;
        }
      }

    template <class Y>
    shared_ptr( const shared_ptr<Y>& r, T* p ) /* noexcept */
      {
        try {
          _p = p;
          _ref = new detail::__owner_alias_aux<Y>( r._ref );
        }
        catch ( ... ) {
          _ref = NULL;
        }
      }

    shared_ptr( const shared_ptr& r ) /* noexcept */ :
        _ref( r._ref == NULL ? NULL : r._ref->ref() )
      {
        if ( _ref != NULL ) {
          _p = r.get();
          _ref->link();
        } else {
          _p = NULL;
        }
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr( const shared_ptr<Y>& r ) /* noexcept */ :
        _ref( r._ref == NULL ? NULL : r._ref->ref() )
      {
        if ( _ref != NULL ) {
          _p = r.get();
          _ref->link();
        } else {
          _p = NULL;
        }
      }

    shared_ptr( shared_ptr&& r ) /* noexcept */ :
        _p( NULL ),
        _ref( NULL )
      {
        _STLP_STD::swap( _p, r._p );
        _STLP_STD::swap( _ref, r._ref );
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr(shared_ptr<Y>&& r) /* noexcept */ :
        _p( NULL ),
        _ref( NULL )
      {
        _STLP_STD::swap( _p, r._p );
        _STLP_STD::swap( _ref, r._ref );
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    explicit shared_ptr(const weak_ptr<Y>& r);

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr(auto_ptr<Y>&& r);

    template <class Y, class D>
    shared_ptr(unique_ptr<Y, D>&& r);

    /* constexpr */ shared_ptr( nullptr_t ) : shared_ptr()
      { }

    // 20.7.2.2.2, destructor:
    ~shared_ptr()
      {
        if ( _ref != NULL ) {
          _ref->unlink();
        }
      }

    // 20.7.2.2.3, assignment:
    shared_ptr& operator =(const shared_ptr& r) /* noexcept */
      { // shared_ptr(r).swap(*this);
        if ( _ref != r._ref ) {
          _STLP_STD::swap( _ref, r._ref );
        }
        return *this;
      }

    template<class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr& operator =(const shared_ptr<Y>& r) /* noexcept */
      {
        if ( _ref != r._ref ) {
          _STLP_STD::swap( _p, r._p );
          _STLP_STD::swap( _ref, r._ref );          
        }
        return *this;
      }

    shared_ptr& operator =(shared_ptr&& r) /* noexcept */;

    template<class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr& operator =(shared_ptr<Y>&& r) /* noexcept */;

    template<class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr& operator =(auto_ptr<Y>&& r);

    template <class Y, class D, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr& operator =(unique_ptr<Y, D>&& r);

    // 20.7.2.2.4, modifiers:
    void swap( shared_ptr& r ) /* noexcept */
      {
        if ( _ref != r._ref ) {
          _STLP_STD::swap( _p, r._p );
          _STLP_STD::swap( _ref, r._ref );
        }
      }

    void reset() /* noexcept */
      { // shared_ptr().swap( *this );
        if ( _ref != NULL ) {
          _ref->unlink();
          _ref = NULL;
        }
      }

    template<class Y>
    void reset( Y* p )
      { shared_ptr(p).swap( *this ); }

    template<class Y, class D>
    void reset( Y* p, D d )
      { shared_ptr(p, d).swap( *this ); }

    template<class Y, class D, class A>
    void reset( Y* p, D d, A a )
      { shared_ptr(p, d, a).swap( *this ); }

    // 20.7.2.2.5, observers:
    T* get() const /* noexcept */
      { return _p; }
    T& operator*() const /* noexcept */
      { return *_p; }
    T* operator->() const /* noexcept */
      { return get(); }
    long use_count() const /* noexcept */
      { return _ref == NULL ? 0L : _ref->count(); }
    bool unique() const /* noexcept */
      { return _ref == NULL ? false : _ref->count() == 1L; }

    explicit operator bool() const /* noexcept */
      { return _p != NULL; }

    template <class U>
    bool owner_before(shared_ptr<U> const& b) const;

    template <class U>
    bool owner_before(weak_ptr<U> const& b) const;

  private:
    // friend template <class Y> class shared_ptr<Y>;
    template <class Y> friend class shared_ptr;
    T* _p;
    detail::__owner_aux_abstract* _ref;
};

// 20.7.2.2.6, shared_ptr creation
template<class T, class... Args>
shared_ptr<T> make_shared(Args&&... args);

template<class T, class A, class... Args>
shared_ptr<T> allocate_shared(const A& a, Args&&... args);

// 20.7.2.2.7, shared_ptr comparisons:
template<class T, class U>
bool operator ==(const shared_ptr<T>& a, const shared_ptr<U>& b) /* noexcept */;

template<class T, class U>
bool operator !=(const shared_ptr<T>& a, const shared_ptr<U>& b) /* noexcept */;

template<class T, class U>
bool operator <(const shared_ptr<T>& a, const shared_ptr<U>& b) /* noexcept */;

template<class T, class U>
bool operator >(const shared_ptr<T>& a, const shared_ptr<U>& b) /* noexcept */;

template<class T, class U>
bool operator <=(const shared_ptr<T>& a, const shared_ptr<U>& b) /* noexcept */;

template<class T, class U>
bool operator >=(const shared_ptr<T>& a, const shared_ptr<U>& b) /* noexcept */;

template <class T>
bool operator ==(const shared_ptr<T>& a, nullptr_t) /* noexcept */;

template <class T>
bool operator ==(nullptr_t, const shared_ptr<T>& b) /* noexcept */;

template <class T>
bool operator !=(const shared_ptr<T>& a, nullptr_t) /* noexcept */;

template <class T>
bool operator !=(nullptr_t, const shared_ptr<T>& b) /* noexcept */;

template <class T>
bool operator <(const shared_ptr<T>& a, nullptr_t) /* noexcept */;

template <class T>
bool operator <(nullptr_t, const shared_ptr<T>& b) /* noexcept */;

template <class T>
bool operator <=(const shared_ptr<T>& a, nullptr_t) /* noexcept */;

template <class T>
bool operator <=(nullptr_t, const shared_ptr<T>& b) /* noexcept */;

template <class T>
bool operator >(const shared_ptr<T>& a, nullptr_t) /* noexcept */;

template <class T>
bool operator >(nullptr_t, const shared_ptr<T>& b) /* noexcept */;

template <class T>
bool operator >=(const shared_ptr<T>& a, nullptr_t) /* noexcept */;

template <class T>
bool operator >=(nullptr_t, const shared_ptr<T>& b) /* noexcept */;

// 20.7.2.2.8, shared_ptr specialized algorithms:
template<class T>
void swap(shared_ptr<T>& a, shared_ptr<T>& b) /* noexcept */;

// 20.7.2.2.9, shared_ptr casts:
template<class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r) /* noexcept */;

template<class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) /* noexcept */;

template<class T, class U>
shared_ptr<T>
const_pointer_cast(const shared_ptr<U>& r) /* noexcept */;

// 20.7.2.2.10, shared_ptr get_deleter:
template<class D, class T>
D* get_deleter(const shared_ptr<T>& p) /* noexcept */;

// 20.7.2.2.11, shared_ptr I/O:
template<class E, class T, class Y>
basic_ostream<E,T>& operator<< (basic_ostream<E,T>& os, const shared_ptr<Y>& p);

_STLP_END_NAMESPACE
