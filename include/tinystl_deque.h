#ifndef __TINYSTL_DEQUE_H
#define __TINYSTL_DEQUE_H
#include "tinystl_algo.h"
#include "tinystl_alloc.h"
#include "tinystl_iterator_base.h"
#include "tinystl_types.h"
#include "tinystl_uninitialized.h"
#include <cstdio>

namespace tinystd {

inline size_t __deque_buf_size(size_t __size) {
    return __size < 512 ? size_t(512 / __size) : size_t(1);
}

template <typename _Tp, typename _Ref, typename _Ptr, size_t _BufSize>
struct __deque_iterator {
//
// iterator_traits
// 
    typedef __random_access_iter iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef _Ptr pointer;
    typedef _Ref reference;

    typedef size_t size_type;
    typedef _Tp **__map_pointer;
    typedef __deque_iterator __self;

    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }
//
// Keep the link with the container
// 
    _Tp *_M_cur;     // the pointer to current
    _Tp *_M_first;   // the pointer to the begin of the buffer
    _Tp *_M_last;    // the pointer to the end of the buffer
    __map_pointer _M_map;


// 
// Overloaded operators
// 
    reference operator*() const { return *_M_cur; }
    pointer operator->() const { return _M_cur; }
    difference_type operator-(const __self &__x) {
        return (_M_cur - _M_first) +
               (__x._M_last - __x._M_cur) +
               _S_buffer_size() * (_M_map - __x._M_map - 1);
    }
    __self &operator++() {
        ++_M_cur;
        if (_M_cur == _M_last) {
            _M_set_node(_M_map + 1);
            _M_cur = _M_first;
        }
        return *this;
    }
    __self operator++(int) {
        __self __tmp = *this;
        ++*this;
        return __tmp;
    }
    __self &operator--() {
        if (_M_cur == _M_first) {
            _M_set_node(_M_map - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
        return *this;
    }
    __self operator--(int) {
        __self __tmp = *this;
        --*this;
        return __tmp;
    }
    __self &operator+=(difference_type __n) {
        if (__n < _M_last - _M_cur) {
            _M_cur += __n;
        } else {
            __n -= _M_last - _M_cur;
            size_t __div = __n / _S_buffer_size();
            size_t __mod = __n % _S_buffer_size();
            _M_set_node(_M_map + __div + 1);
            _M_cur = _M_first + __mod;
        }
        return *this;
    }
    __self operator+(difference_type __n) {
        __self __tmp = *this;
        return __tmp += __n;
    }
    __self &operator-=(difference_type __n) { return *this += -__n; }
    __self operator-(difference_type __n) { __self __tmp = *this; return __tmp -= __n; }
    reference operator[](size_t __idx) { return *(*this + __idx); }
    bool operator==(const __self &__it) { return _M_cur == __it._M_cur; }
    bool operator!=(const __self &__it) { return !(*this == __it); }
    bool operator<(const __self &__it) { return _M_map == __it._M_map ? _M_cur < __it._M_cur : _M_map < __it._M_map; }
    
    void _M_set_node(__map_pointer __new_map) {
        _M_map = __new_map;
        _M_first = *__new_map;
        _M_last = _M_first + _S_buffer_size();;

    }   
};

template <typename _Tp, typename _Alloc = __DEFAULT_ALLOCATOR, size_t _BufSize = 0>
class deque {
public:
//
//  Basic Types
//
    typedef _Tp value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef __deque_iterator<_Tp, reference, pointer, _BufSize> iterator;
    typedef __deque_iterator<_Tp, const_reference, const_pointer, _BufSize> const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t different_type;

    deque(size_type __n, const _Tp &__value)
        : _M_start(), _M_finish(), _M_map(0), _M_map_size(0)
    { _M_fill_initialize(__n, __value); }

    reference operator[](size_type __idx) { return _M_start[__idx]; }

    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    reference front() { return *_M_start; }
    reference back() { return *(_M_finish - 1); }
    size_type size() const { return tinystd::distance(_M_start, _M_finish); }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return _M_start == _M_finish; }

protected:
    typedef pointer *__map_pointer;
//
// Memory allocator
//
    typedef tinystd::simple_alloc<_Tp, _Alloc> __data_allocator;
    typedef tinystd::simple_alloc<_Tp *, _Alloc> __map_allocator;

    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }

    __map_pointer _M_map;   // **_Tp 
    size_type _M_map_size;  
    iterator _M_start;
    iterator _M_finish;

    void _M_fill_initialize(size_type __n, const _Tp &__value) {
        _M_create_map_and_nodes(__n);
        tinystd::uninitialized_fill(_M_start, _M_finish, __value);
    }

    /**
     * @brief Generate and process the structure of the deque
     * 
     * @param __nums numbers of the init elements. 
     */
    void _M_create_map_and_nodes(size_type __num_elements) {
        size_type __buffer_sz = _S_buffer_size();
        size_type __num_nodes = __num_elements / __buffer_sz + 1;

                                // FIXME: min is eight ? 
        _M_map_size = tinystd::max(size_type(8), __num_nodes + 2);

        // allocate the map 
        _M_map = __map_allocator::_S_allocate(_M_map_size);

        __map_pointer __new_start = _M_map + ((_M_map_size - __num_nodes) >> 1);
        __map_pointer __new_finish = __new_start + __num_nodes - 1;

        __map_pointer __cur;
        try {
            for (__cur = __new_start; __cur <= __new_finish; ++__cur) 
                                // FIXME: add to a function ? (allocate_node())
                *__cur = __data_allocator::_S_allocate(__buffer_sz);
        } catch (...) {
            // roll back. 
            // If not everyone is successfully create. 
            // then clear every one. 
            // TODO: add code here. 
        }

        _M_start._M_set_node(__new_start);
        _M_finish._M_set_node(__new_finish);
        _M_start._M_cur = _M_start._M_first;
        _M_finish._M_cur = _M_finish._M_first + __num_elements % __buffer_sz;
    }
};

}

#endif // __TINYSTL_DEQUE_H
