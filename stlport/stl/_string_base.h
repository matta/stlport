/*
 * Copyright (c) 1997-1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * Copyright (c) 2003 
 * Francois Dumont
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

#ifndef _STLP_STRING_BASE_H
#define _STLP_STRING_BASE_H

// ------------------------------------------------------------
// Class _String_base.  

// _String_base is a helper class that makes it it easier to write an
// exception-safe version of basic_string.  The constructor allocates,
// but does not initialize, a block of memory.  The destructor
// deallocates, but does not destroy elements within, a block of
// memory.  The destructor assumes that _M_start either is null, or else
// points to a block of memory that was allocated using _String_base's 
// allocator and whose size is _M_end_of_storage._M_data - _M_start.

_STLP_BEGIN_NAMESPACE

template <class _Tp, class _Alloc> 
class _String_base {
protected:
  _STLP_FORCE_ALLOCATORS(_Tp, _Alloc)
  typedef typename _Alloc_traits<_Tp, _Alloc>::allocator_type allocator_type;
  //dums: Some compiler(MSVC6) require it to be public!
public:
  enum {_DEFAULT_SIZE = 8};
private:
#ifdef _STLP_USE_SHORT_STRING_OPTIM
  union {
    _Tp*  _M_dynamic_buf;
    _Tp   _M_static_buf[_DEFAULT_SIZE];
  } _M_buffers;
#else
  _Tp*    _M_start;
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
protected:
#ifdef _STLP_USE_SHORT_STRING_OPTIM
  bool _M_using_static_buf() const {
    return (_M_end_of_storage._M_data == _M_buffers._M_static_buf + _DEFAULT_SIZE);
  }
  _Tp const* _M_Start() const {
    return _M_using_static_buf()?_M_buffers._M_static_buf:_M_buffers._M_dynamic_buf;
  }
  _Tp* _M_Start() {
    return _M_using_static_buf()?_M_buffers._M_static_buf:_M_buffers._M_dynamic_buf;
  }
#else
  _Tp const* _M_Start() const {return _M_start;}
  _Tp* _M_Start() {return _M_start;}
#endif /* _STLP_USE_SHORT_STRING_OPTIM */

  _Tp*    _M_finish;
  _STLP_alloc_proxy<_Tp*, _Tp, allocator_type> _M_end_of_storage;

  _Tp const* _M_Finish() const {return _M_finish;}
  _Tp* _M_Finish() {return _M_finish;}

  // Precondition: 0 < __n <= max_size().
  void _M_allocate_block(size_t __n = _DEFAULT_SIZE);
  void _M_deallocate_block() {
#ifdef _STLP_USE_SHORT_STRING_OPTIM
    if (!_M_using_static_buf() && (_M_buffers._M_dynamic_buf != 0))
      _M_end_of_storage.deallocate(_M_buffers._M_dynamic_buf, _M_end_of_storage._M_data - _M_buffers._M_dynamic_buf); 
#else
    if (_M_start != 0)
      _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); 
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
  }
  
  size_t max_size() const { return (size_t(-1) / sizeof(_Tp)) - 1; }

  _String_base(const allocator_type& __a)
#ifdef _STLP_USE_SHORT_STRING_OPTIM
    : _M_finish(_M_buffers._M_static_buf), _M_end_of_storage(__a, _M_buffers._M_static_buf + _DEFAULT_SIZE) {}
#else
    : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) {}
#endif
  
  _String_base(const allocator_type& __a, size_t __n)
#ifdef _STLP_USE_SHORT_STRING_OPTIM
    : _M_finish(_M_buffers._M_static_buf), _M_end_of_storage(__a, _M_buffers._M_static_buf + _DEFAULT_SIZE)
    {if (__n > _DEFAULT_SIZE) _M_allocate_block(__n);}
#else
    : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
    {_M_allocate_block(__n); }
#endif

  _String_base(__partial_move_source<_String_base> src)
#ifdef _STLP_USE_SHORT_STRING_OPTIM
    : _M_end_of_storage(src.get()._M_end_of_storage)
    {
      if (src.get()._M_using_static_buf()) {
        _M_buffers = src.get()._M_buffers;
        _M_finish = _M_buffers._M_static_buf + (src.get()._M_finish - src.get()._M_buffers._M_static_buf);
        _M_end_of_storage._M_data = _M_buffers._M_static_buf + _DEFAULT_SIZE;
      } 
      else {
        _M_buffers._M_dynamic_buf = src.get()._M_buffers._M_dynamic_buf;
        _M_finish = src.get()._M_finish;
        _M_end_of_storage._M_data = src.get()._M_end_of_storage._M_data;
        src.get()._M_buffers._M_dynamic_buf = 0;
      }
    }
#else
    : _M_start(src.get()._M_start), _M_finish(src.get()._M_finish), _M_end_of_storage(src.get()._M_end_of_storage)
    {src.get()._M_start = 0;}
#endif  

  ~_String_base() { _M_deallocate_block(); }

  void _M_reset(_Tp *__start, _Tp *__finish, _Tp *__end_of_storage) {
#ifdef _STLP_USE_SHORT_STRING_OPTIM
    _M_buffers._M_dynamic_buf = __start;
#else
    _M_start = __start;
#endif
    _M_finish = __finish;
    _M_end_of_storage._M_data = __end_of_storage;
  }

  void _M_destroy_back () {
#ifdef _STLP_USE_SHORT_STRING_OPTIM
    if (!_M_using_static_buf())
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
      _STLP_STD::_Destroy(_M_finish);
  }

  void _M_destroy_range(size_t __from_off = 0, size_t __to_off = 1) {
#ifdef _STLP_USE_SHORT_STRING_OPTIM
    if (!_M_using_static_buf())
		  _STLP_STD::_Destroy_Range(_M_buffers._M_dynamic_buf + __from_off, _M_finish + __to_off);
#else
		_STLP_STD::_Destroy_Range(_M_start + __from_off, _M_finish + __to_off);
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
  }

  void _M_destroy_ptr_range(_Tp *__f, _Tp *__l) {
#ifdef _STLP_USE_SHORT_STRING_OPTIM
    if (!_M_using_static_buf())
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
		  _STLP_STD::_Destroy_Range(__f, __l);
  }

  void _M_Swap(_String_base<_Tp, _Alloc> &__s) {
#ifdef _STLP_USE_SHORT_STRING_OPTIM
    if (_M_using_static_buf())
      _STLP_STD::swap(_M_buffers, __s._M_buffers);
    else
      _STLP_STD::swap(_M_buffers._M_dynamic_buf, __s._M_buffers._M_dynamic_buf);
#else
    _STLP_STD::swap(_M_start, __s._M_start);
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
    _STLP_STD::swap(_M_finish, __s._M_finish);
    _STLP_STD::swap(_M_end_of_storage, __s._M_end_of_storage);
  }

  void _M_throw_length_error() const;
  void _M_throw_out_of_range() const;
};

# if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS _String_base<char, allocator<char> >;
#  if defined (_STLP_HAS_WCHAR_T)
_STLP_EXPORT_TEMPLATE_CLASS _String_base<wchar_t, allocator<wchar_t> >;
#  endif
# endif /* _STLP_USE_TEMPLATE_EXPORT */

_STLP_END_NAMESPACE

#endif /* _STLP_STRING_BASE_H */
