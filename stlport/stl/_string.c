/*
 *
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
#ifndef _STLP_STRING_C
#define _STLP_STRING_C

#ifndef _STLP_INTERNAL_STRING_H
#  include <stl/_string.h>
#endif

#ifndef _STLP_INTERNAL_CTRAITS_FUNCTIONS_H
#  include <stl/_ctraits_fns.h>
#endif

#if defined(_STLP_DEBUG)
#  if defined (_STLP_USE_MSVC6_MEM_T_BUG_WORKAROUND)
#    define basic_string _STLP_NON_DBG_NO_MEM_T_NAME(str)
#  else
#    define basic_string _STLP_NON_DBG_NAME(str)
#  endif
#else
#  if defined (_STLP_USE_MSVC6_MEM_T_BUG_WORKAROUND)
#    define basic_string _STLP_NO_MEM_T_NAME(str)
#  endif
#endif

#if defined (_STLP_NESTED_TYPE_PARAM_BUG)
#  define __size_type__ size_t
#  define size_type size_t
#  define iterator _CharT*
#else
#  define __size_type__ _STLP_TYPENAME_ON_RETURN_TYPE basic_string<_CharT,_Traits,_Alloc>::size_type
#endif

_STLP_BEGIN_NAMESPACE

// A helper class to use a char_traits as a function object.
template <class _Traits>
struct _Not_within_traits : public unary_function<typename _Traits::char_type, bool> {
  typedef typename _Traits::char_type _CharT;
  const _CharT* _M_first;
  const _CharT* _M_last;

  _Not_within_traits(const _CharT* __f, const _CharT* __l) 
    : _M_first(__f), _M_last(__l) {}

  bool operator()(const _CharT& __x) const {
    return find_if(_M_first, _M_last, 
                   _Eq_char_bound<_Traits>(__x)) == _M_last;
  }
};

// ------------------------------------------------------------
// Non-inline declarations.


// Change the string's capacity so that it is large enough to hold
//  at least __res_arg elements, plus the terminating _CharT().  Note that,
//  if __res_arg < capacity(), this member function may actually decrease
//  the string's capacity.
template <class _CharT, class _Traits, class _Alloc> 
void basic_string<_CharT,_Traits,_Alloc>::reserve(size_type __res_arg) {
  if (__res_arg > max_size())
    this->_M_throw_length_error();

  size_type __n = (max)(__res_arg, size()) + 1;
  if (__n <= capacity() + 1)
    return;

  pointer __new_start = this->_M_end_of_storage.allocate(__n);
  pointer __new_finish = __new_start;

  _STLP_TRY {
    __new_finish = uninitialized_copy(this->_M_Start(), this->_M_Finish(), __new_start);
    _M_construct_null(__new_finish);
  }
  _STLP_UNWIND((_STLP_STD::_Destroy_Range(__new_start, __new_finish), 
                this->_M_end_of_storage.deallocate(__new_start, __n)))

  this->_M_destroy_range();
  this->_M_deallocate_block();
  this->_M_reset(__new_start, __new_finish, __new_start + __n);
}

template <class _CharT, class _Traits, class _Alloc> 
basic_string<_CharT,_Traits,_Alloc>& 
basic_string<_CharT,_Traits,_Alloc>::append(size_type __n, _CharT __c) {
  if (__n > max_size() || size() > max_size() - __n)
    this->_M_throw_length_error();
  if (size() + __n > capacity())
    reserve(size() + (max)(size(), __n));
  if (__n > 0) {
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
    if (this->_M_using_static_buf())
      _Traits::assign(this->_M_finish + 1, __n - 1, __c);
    else
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
    __uninitialized_fill_n(this->_M_finish + 1, __n - 1, __c, _Char_Is_POD());
    _STLP_TRY {
      _M_construct_null(this->_M_finish + __n);
    }
    _STLP_UNWIND(this->_M_destroy_ptr_range(this->_M_finish + 1, this->_M_finish + __n))
    _Traits::assign(*end(), __c);
    this->_M_finish += __n;
  }
  return *this;
}

template <class _CharT, class _Traits, class _Alloc> 
basic_string<_CharT, _Traits, _Alloc>&
basic_string<_CharT, _Traits, _Alloc>::_M_append(const _CharT* __first, const _CharT* __last) {
  if (__first != __last) {
    const size_type __old_size = size();
    ptrdiff_t __n = __last - __first;
    if ((size_type)__n > max_size() || __old_size > max_size() - __n)
      this->_M_throw_length_error();
    if (__old_size + __n > capacity()) {
      const size_type __len = __old_size + (max)(__old_size, (size_t) __n) + 1;
      pointer __new_start = this->_M_end_of_storage.allocate(__len);
      pointer __new_finish = __new_start;
      _STLP_TRY {
        __new_finish = uninitialized_copy(this->_M_Start(), this->_M_Finish(), __new_start);
        __new_finish = uninitialized_copy(__first, __last, __new_finish);
        _M_construct_null(__new_finish);
      }
      _STLP_UNWIND((_STLP_STD::_Destroy_Range(__new_start,__new_finish),
                    this->_M_end_of_storage.deallocate(__new_start,__len)))
      this->_M_destroy_range();
      this->_M_deallocate_block();
      this->_M_reset(__new_start, __new_finish, __new_start + __len);
    }
    else {
      const _CharT* __f1 = __first;
      ++__f1;
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
      if (this->_M_using_static_buf())
        _M_copy(__f1, __last, this->_M_Finish() + 1);
      else
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
      uninitialized_copy(__f1, __last, this->_M_finish + 1);
      _STLP_TRY {
        _M_construct_null(this->_M_finish + __n);
      }
      _STLP_UNWIND(this->_M_destroy_ptr_range(this->_M_finish + 1, this->_M_finish + __n))
      _Traits::assign(*end(), *__first);
      this->_M_finish += __n;
    }
  }
  return *this;  
}

template <class _CharT, class _Traits, class _Alloc> 
basic_string<_CharT,_Traits,_Alloc>& 
basic_string<_CharT,_Traits,_Alloc>::assign(size_type __n, _CharT __c) {
  if (__n <= size()) {
    _Traits::assign(this->_M_Start(), __n, __c);
    erase(begin() + __n, end());
  }
  else {
    if (__n < capacity()) {
      _Traits::assign(this->_M_Start(), size(), __c);
      append(__n - size(), __c);
    }
    else {
      _Self __str(__n, __c);
      this->swap(__str);
    }
  }
  return *this;
}

template <class _CharT, class _Traits, class _Alloc> 
basic_string<_CharT,_Traits,_Alloc>& 
basic_string<_CharT,_Traits,_Alloc>::_M_assign(const _CharT* __f, const _CharT* __l) {
  ptrdiff_t __n = __l - __f;
  if (__STATIC_CAST(size_type,__n) <= size()) {
    _Traits::copy(this->_M_Start(), __f, __n);
    erase(begin() + __n, end());
  }
  else {
    _Traits::copy(this->_M_Start(), __f, size());
    _M_append(__f + size(), __l);
  }
  return *this;
}

template <class _CharT, class _Traits, class _Alloc>
_CharT* basic_string<_CharT,_Traits,_Alloc> ::_M_insert_aux(_CharT* __p,
                                                            _CharT __c) {
  pointer __new_pos = __p;
  if (this->_M_finish + 1 < this->_M_end_of_storage._M_data) {
    _M_construct_null(this->_M_finish + 1);
    _Traits::move(__p + 1, __p, this->_M_finish - __p);
    _Traits::assign(*__p, __c);
    ++this->_M_finish;
  }
  else {
    const size_type __old_len = size();
    const size_type __len = __old_len +
                            (max)(__old_len, __STATIC_CAST(size_type,1)) + 1;
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    _STLP_TRY {
      __new_pos = uninitialized_copy(this->_M_Start(), __p, __new_start);
      _Copy_Construct(__new_pos, __c);
      __new_finish = __new_pos + 1;
      __new_finish = uninitialized_copy(__p, this->_M_finish, __new_finish);
      _M_construct_null(__new_finish);
    }
    _STLP_UNWIND((_STLP_STD::_Destroy_Range(__new_start,__new_finish), 
                  this->_M_end_of_storage.deallocate(__new_start,__len)))
    this->_M_destroy_range();
    this->_M_deallocate_block();
    this->_M_reset(__new_start, __new_finish, __new_start + __len);
  }
  return __new_pos;
}

template <class _CharT, class _Traits, class _Alloc> 
void basic_string<_CharT,_Traits,_Alloc>::insert(iterator __pos, 
                                                 size_t __n, _CharT __c) {
  if (__n != 0) {
    if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n + 1) {
      const size_type __elems_after = this->_M_finish - __pos;
      pointer __old_finish = this->_M_finish;
      if (__elems_after >= __n) {
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
        if (this->_M_using_static_buf())
          _M_copy((this->_M_finish - __n) + 1, this->_M_finish + 1, this->_M_finish + 1);
        else
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
        uninitialized_copy((this->_M_finish - __n) + 1, this->_M_finish + 1,
                           this->_M_finish + 1);
        this->_M_finish += __n;
        _Traits::move(__pos + __n, __pos, (__elems_after - __n) + 1);
        _Traits::assign(__pos, __n, __c);
      }
      else {
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
        if (this->_M_using_static_buf())
          _Traits::assign(this->_M_finish + 1, __n - __elems_after - 1, __c);
        else
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
        __uninitialized_fill_n(this->_M_finish + 1, __n - __elems_after - 1, __c, _Char_Is_POD());
        this->_M_finish += __n - __elems_after;
        _STLP_TRY {
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
          if (this->_M_using_static_buf())
            _M_copy(__pos, __old_finish + 1, this->_M_finish);
          else
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
          uninitialized_copy(__pos, __old_finish + 1, this->_M_finish);
          this->_M_finish += __elems_after;
        }
        _STLP_UNWIND((_STLP_STD::_Destroy_Range(__old_finish + 1, this->_M_finish), 
                      this->_M_finish = __old_finish))
        _Traits::assign(__pos, __elems_after + 1, __c);
      }
    }
    else {
      const size_type __old_size = size();        
      const size_type __len = __old_size + (max)(__old_size, __n) + 1;
      pointer __new_start = this->_M_end_of_storage.allocate(__len);
      pointer __new_finish = __new_start;
      _STLP_TRY {
        __new_finish = uninitialized_copy(this->_M_Start(), __pos, __new_start);
        __new_finish = __uninitialized_fill_n(__new_finish, __n, __c, _Char_Is_POD());
        __new_finish = uninitialized_copy(__pos, this->_M_finish, __new_finish);
        _M_construct_null(__new_finish);
      }
      _STLP_UNWIND((_STLP_STD::_Destroy_Range(__new_start,__new_finish),
                    this->_M_end_of_storage.deallocate(__new_start,__len)))
      this->_M_destroy_range();
      this->_M_deallocate_block();
      this->_M_reset(__new_start, __new_finish, __new_start + __len);    
    }
  }
}

template <class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT,_Traits,_Alloc>::_M_insert(iterator __pos,
                                                    const _CharT* __first, const _CharT* __last,
                                                    bool __self_ref) {
  //this version has to take care about the auto referencing
  if (__first != __last) {
    const ptrdiff_t __n = __last - __first;
    if (this->_M_end_of_storage._M_data - this->_M_finish >= __n + 1) {
      const ptrdiff_t __elems_after = this->_M_finish - __pos;
      pointer __old_finish = this->_M_finish;
      if (__elems_after >= __n) {
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
        if (this->_M_using_static_buf())
          _M_copy((this->_M_finish - __n) + 1, this->_M_finish + 1, this->_M_finish + 1);
        else
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
        uninitialized_copy((this->_M_finish - __n) + 1, this->_M_finish + 1, this->_M_finish + 1);
        this->_M_finish += __n;
        _Traits::move(__pos + __n, __pos, (__elems_after - __n) + 1);
        if (!__self_ref || __last < __pos) {
          _M_copy(__first, __last, __pos);
        }
        else {
          //We have to check that the source buffer hasn't move
          if (__first >= __pos) {
            //The source buffer has move
            __first += __n;
            __last += __n;
            _M_copy(__first, __last, __pos);
          }
          else {
            //The source buffer hasn't move, it has been duplicated
            _M_move(__first, __last, __pos);
          }
        }
      }
      else {
        const_iterator __mid = __first;
        __mid += __elems_after + 1;
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
        if (this->_M_using_static_buf())
          _M_copy(__mid, __last, this->_M_finish + 1);
        else
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
        uninitialized_copy(__mid, __last, this->_M_finish + 1);
        this->_M_finish += __n - __elems_after;
        _STLP_TRY {
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
          if (this->_M_using_static_buf())
            _M_copy(__pos, __old_finish + 1, this->_M_finish);
          else
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
          uninitialized_copy(__pos, __old_finish + 1, this->_M_finish);
          this->_M_finish += __elems_after;
        }
        _STLP_UNWIND((_STLP_STD::_Destroy_Range(__old_finish + 1, this->_M_finish), 
                      this->_M_finish = __old_finish))
        if (!__self_ref)
          _M_copy(__first, __mid, __pos);
        else
          _M_move(__first, __mid, __pos);
      }
    }
    else {
      const size_type __old_size = size();        
      const size_type __len = __old_size + (max)(__old_size, __STATIC_CAST(const size_type,__n)) + 1;
      pointer __new_start = this->_M_end_of_storage.allocate(__len);
      pointer __new_finish = __new_start;
      _STLP_TRY {
        __new_finish = uninitialized_copy(this->_M_Start(), __pos, __new_start);
        __new_finish = uninitialized_copy(__first, __last, __new_finish);
        __new_finish = uninitialized_copy(__pos, this->_M_finish, __new_finish);
        _M_construct_null(__new_finish);
      }
      _STLP_UNWIND((_STLP_STD::_Destroy_Range(__new_start,__new_finish),
                    this->_M_end_of_storage.deallocate(__new_start,__len)))
      this->_M_destroy_range();
      this->_M_deallocate_block();
      this->_M_reset(__new_start, __new_finish, __new_start + __len);
    }
  }
}

template <class _CharT, class _Traits, class _Alloc> 
basic_string<_CharT,_Traits,_Alloc>& 
basic_string<_CharT,_Traits,_Alloc> ::replace(iterator __first, iterator __last, 
                                              size_type __n, _CharT __c) {
  size_type __len = (size_type)(__last - __first);
  
  if (__len >= __n) {
    _Traits::assign(__first, __n, __c);
    erase(__first + __n, __last);
  }
  else {
    _Traits::assign(__first, __len, __c);
    insert(__last, __n - __len, __c);
  }
  return *this;
}

template <class _CharT, class _Traits, class _Alloc> 
basic_string<_CharT,_Traits,_Alloc>& 
basic_string<_CharT,_Traits,_Alloc> ::_M_replace(iterator __first, iterator __last,
                                                 const _CharT* __f, const _CharT* __l,
                                                 bool __self_ref) {
  const ptrdiff_t       __n = __l - __f;
  const difference_type __len = __last - __first;
  if (__len >= __n) {
    if (!__self_ref || __l < __first || __f >= __last)
      _M_copy(__f, __l, __first);
    else
      _M_move(__f, __l, __first);
    erase(__first + __n, __last);
  }
  else {
    if (!__self_ref || (__f >= __last) || (__l <= __first)) {
      //no overlap:
      const_iterator __m = __f + __len;
      _M_copy(__f, __m, __first);
      _M_insert(__last, __m, __l, false ); 
    }
    else {
      //we have to take care of overlaping
      if (__f < __first) {
        const_iterator __m = __f + __len;
        //We have to deal with possible reallocation because we do insert first.
        const difference_type __off_dest = __first - this->begin();
        const difference_type __off_src = __f - this->begin();
        _M_insert(__last, __m, __l, true);
        _Traits::move(begin() + __off_dest, begin() + __off_src, __len);
      }
      else {
        const_iterator __m = __f + __len;
        _Traits::move(__first, __f, __len);
        _M_insert(__last, __m, __l, true);
      }
    }
  }
  return *this;
}

template <class _CharT, class _Traits, class _Alloc> __size_type__ 
basic_string<_CharT,_Traits,_Alloc> ::find(const _CharT* __s, size_type __pos, 
                                           size_type __n) const {
  const size_t __len = size();
  if (__pos >= __len || __pos + __n > __len)
    return npos;
  else {
    const_pointer __result =
      _STLP_STD::search(this->_M_Start() + __pos, this->_M_Finish(), 
                        __s, __s + __n, _Eq_traits<_Traits>());
    return __result != this->_M_Finish() ? __result - this->_M_Start() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc> __size_type__
basic_string<_CharT,_Traits,_Alloc> ::find(_CharT __c, size_type __pos) const {
  if (__pos >= size()) /*__pos + 1 > size()*/
    return npos;
  else {
    const_pointer __result =
      _STLP_STD::find_if(this->_M_Start() + __pos, this->_M_Finish(),
                         _Eq_char_bound<_Traits>(__c));
    return __result != this->_M_Finish() ? __result - this->_M_Start() : npos;
  }
}    

template <class _CharT, class _Traits, class _Alloc> __size_type__
basic_string<_CharT,_Traits,_Alloc> ::rfind(const _CharT* __s, size_type __pos, 
                                            size_type __n) const {
  const size_t __len = size();
  if (__n > __len || __pos < __n)
    return npos;
  else if (__n == 0)
    return (min) (__len, __pos);
  else {
    const_pointer __last = this->_M_Start() + (min) (__len - __n, __pos) + __n;
    const_pointer __result = _STLP_STD::__find_end(this->_M_Start(), __last, 
                                                   __s, __s + __n,
                                                   bidirectional_iterator_tag(), bidirectional_iterator_tag(),
                                                   _Eq_traits<_Traits>());
    return __result != __last ? __result - this->_M_Start() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc> __size_type__
basic_string<_CharT,_Traits,_Alloc> ::rfind(_CharT __c, size_type __pos) const {
  const size_type __len = size();
  if (1 > __len || __pos < 1)
    return npos;
  else {
    const_iterator __last = begin() + (min) (__len - 1, __pos) + 1;
    const_reverse_iterator __rresult =
      _STLP_STD::find_if(const_reverse_iterator(__last), rend(),
                         _Eq_char_bound<_Traits>(__c));
    return __rresult != rend() ? (__rresult.base() - 1) - begin() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc> __size_type__
basic_string<_CharT,_Traits,_Alloc> ::find_first_of(const _CharT* __s, size_type __pos, 
                                                    size_type __n) const {
  if (__pos >= size()) /*__pos + 1 > size()*/
    return npos;
  else {
    const_iterator __result = __find_first_of(begin() + __pos, end(),
                                              __s, __s + __n,
                                              _Eq_traits<_Traits>());
    return __result != end() ? __result - begin() : npos;
  }
}


template <class _CharT, class _Traits, class _Alloc> __size_type__
basic_string<_CharT,_Traits,_Alloc> ::find_last_of(const _CharT* __s, size_type __pos, 
                                                   size_type __n) const {
  const size_type __len = size();
  if (1 > __len || __pos < 1)
    return npos;
  else {
    const const_iterator __last = begin() + (min) (__len - 1, __pos) + 1;
    const const_reverse_iterator __rresult =
      __find_first_of(const_reverse_iterator(__last), rend(),
                      __s, __s + __n,
                      _Eq_traits<_Traits>());
    return __rresult != rend() ? (__rresult.base() - 1) - begin() : npos;
  }
}


template <class _CharT, class _Traits, class _Alloc> __size_type__
basic_string<_CharT,_Traits,_Alloc> ::find_first_not_of(const _CharT* __s, size_type __pos, 
                                                        size_type __n) const {
  typedef typename _Traits::char_type _CharType;
  if (__pos >= size()) /*__pos + 1 >= size()*/
    return npos;
  else {
    const_pointer __result = _STLP_STD::find_if(this->_M_Start() + __pos, this->_M_Finish(),
                                                _Not_within_traits<_Traits>(__CONST_CAST(const _CharType*, __s), 
                                                                            __CONST_CAST(const _CharType*, __s) + __n));
    return __result != this->_M_finish ? __result - this->_M_Start() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc> __size_type__
basic_string<_CharT,_Traits,_Alloc> ::find_first_not_of(_CharT __c, size_type __pos) const {
  if (1 > size())
    return npos;
  else {
    const_pointer __result = _STLP_STD::find_if(this->_M_Start() + __pos, this->_M_Finish(),
                                                _Neq_char_bound<_Traits>(__c));
    return __result != this->_M_finish ? __result - this->_M_Start() : npos;
  }
}    

template <class _CharT, class _Traits, class _Alloc> __size_type__
basic_string<_CharT,_Traits,_Alloc> ::find_last_not_of(const _CharT* __s, size_type __pos, 
                                                       size_type __n) const {
  typedef typename _Traits::char_type _CharType;
  const size_type __len = size();
  if (1 > __len || __pos < 1)
    return npos;
  else {
    const_iterator __last = begin() + (min) (__len - 1, __pos) + 1;
    const_reverse_iterator __rlast = const_reverse_iterator(__last);
    const_reverse_iterator __rresult =
      _STLP_STD::find_if(__rlast, rend(),
                         _Not_within_traits<_Traits>((const _CharType*)__s, 
                                                     (const _CharType*)__s + __n));
    return __rresult != rend() ? (__rresult.base() - 1) - begin() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc> __size_type__
basic_string<_CharT, _Traits, _Alloc> ::find_last_not_of(_CharT __c, size_type __pos) const {
  const size_type __len = size();
  if (1 > __len || __pos < 1)
    return npos;
  else {
    const_iterator __last = begin() + (min) (__len - 1, __pos) + 1;
    const_reverse_iterator __rlast = const_reverse_iterator(__last);
    const_reverse_iterator __rresult =
      _STLP_STD::find_if(__rlast, rend(),
                         _Neq_char_bound<_Traits>(__c));
    return __rresult != rend() ? (__rresult.base() - 1) - begin() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc> 
void _STLP_CALL _S_string_copy(const basic_string<_CharT,_Traits,_Alloc>& __s,
                               _CharT* __buf, size_t __n) {
  if (__n > 0) {
    __n = (min) (__n - 1, __s.size());
    _STLP_STD::copy(__s.begin(), __s.begin() + __n, __buf);
    __buf[__n] = _CharT();
  }
}
_STLP_END_NAMESPACE

#include <stl/_range_errors.h>

_STLP_BEGIN_NAMESPACE

// _String_base methods
template <class _Tp, class _Alloc>
void _String_base<_Tp,_Alloc>::_M_throw_length_error() const {
  __stl_throw_length_error("basic_string");
}

template <class _Tp, class _Alloc> 
void _String_base<_Tp, _Alloc>::_M_throw_out_of_range() const {
  __stl_throw_out_of_range("basic_string");
}

template <class _Tp, class _Alloc> 
void _String_base<_Tp, _Alloc>::_M_allocate_block(size_t __n) {
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
  if (__n <= _DEFAULT_SIZE) {
    this->_M_finish = this->_M_buffers._M_static_buf; 
    this->_M_end_of_storage._M_data = this->_M_buffers._M_static_buf + _DEFAULT_SIZE; 
  } else
#endif /*_STLP_USE_SHORT_STRING_OPTIM  */
  if ((__n <= (max_size()+1)) && (__n > 0)){
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
    this->_M_buffers._M_dynamic_buf = _M_end_of_storage.allocate(__n);
    this->_M_finish = this->_M_buffers._M_dynamic_buf; 
#else
    this->_M_start  = _M_end_of_storage.allocate(__n); 
    this->_M_finish = this->_M_start; 
#endif /*_STLP_USE_SHORT_STRING_OPTIM  */
    this->_M_end_of_storage._M_data = this->_M_finish + __n; 
  }
  else 
    this->_M_throw_length_error(); 
} 
 
template <class _CharT, class _Traits, class _Alloc> 
basic_string<_CharT, _Traits, _Alloc>::basic_string(const _CharT* __s, 
                                                    const allocator_type& __a)
  : _String_base<_CharT,_Alloc>(__a) {
  _STLP_FIX_LITERAL_BUG(__s)
  _M_range_initialize(__s, __s + traits_type::length(__s));
}


template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT, _Traits, _Alloc>::basic_string(const basic_string<_CharT, _Traits, _Alloc> & __s)
  : _String_base<_CharT,_Alloc>(__s.get_allocator()) {
  _M_range_initialize(__s._M_Start(), __s._M_Finish());
}
  
#if defined ( __SUNPRO_CC) && ! defined(_STLP_STATIC_CONST_INIT_BUG)
template <class _CharT, class _Traits, class _Alloc>
const size_t basic_string<_CharT, _Traits, _Alloc>::npos;
#endif

_STLP_END_NAMESPACE

#undef basic_string

#undef __size_type__
#if defined (_STLP_NESTED_TYPE_PARAM_BUG)
#  undef size_type
#  undef iterator
#endif

#endif /*  _STLP_STRING_C */

// Local Variables:
// mode:C++
// End:
