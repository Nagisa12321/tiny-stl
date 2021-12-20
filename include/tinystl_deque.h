#ifndef __TINYSTL_DEQUE_H
#define __TINYSTL_DEQUE_H
#include "tinystl_algo.h"
#include "tinystl_alloc.h"
#include "tinystl_constructor.h"
#include "tinystl_iterator_base.h"
#include "tinystl_types.h"
#include "tinystl_uninitialized.h"

// use c++ init list. 
#include <initializer_list>

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

    __deque_iterator() = default;
    __deque_iterator(_Tp *__cur, _Tp *__first, _Tp *__last, __map_pointer __map)
        : _M_cur(__cur), _M_first(__first), _M_last(__last), _M_map(__map) 
            { }

// 
// Overloaded operators
// 
    // operator __deque_iterator<_Tp, const _Ref, const _Ptr, _BufSize>() { 
    //     __deque_iterator<_Tp, const _Ref, const _Ptr, _BufSize> __res;
    //     __res._M_cur = _M_cur;
    //     __res._M_first = _M_first;
    //     __res._M_last = _M_last;
    //     __res._M_map = _M_map;
    // }
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
    __self &operator-=(difference_type __n) {
        if (__n <= _M_cur - _M_first) {
            _M_cur -= __n;
        } else {
            __n -= _M_cur - _M_first;
            size_t __div = (__n - 1) / _S_buffer_size();
            size_t __mod = (__n - 1) % _S_buffer_size();
            _M_set_node(_M_map - __div - 1);
            _M_cur = _M_last - __mod - 1;
        }
        return *this;
    }
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

    deque() 
        { _M_create_map_and_nodes(0); }
    deque(const deque &__d) : deque(__d.begin(), __d.end()) {}
    deque(size_type __n) : deque(__n, _Tp()) {} 
    deque(size_type __n, const _Tp &__value)
        { _M_fill_initialize(__n, __value); }
    deque(std::initializer_list<_Tp> __li)
        { _M_copy_initialize(__li.begin(), __li.end()); }
    template <typename _InputIter, tinystd::void_t<decltype(*tinystd::__declval<_InputIter>())>* = nullptr>
    deque(_InputIter __begin, _InputIter __end) 
        { _M_copy_initialize(__begin, __end); }

    ~deque()
    { 
        // clear the data that the map maintain... 
        _M_clear_data();

        // free the map itself
        __map_allocator::_S_deallocate(_M_map, _M_map_size);
    }

    reference operator[](size_type __idx) { return _M_start[__idx]; }

    iterator begin() { return _M_start; }
    const_iterator begin() const { return _M_to_const_iterator(_M_start); }
    iterator end() { return _M_finish; }
    const_iterator end() const { return _M_to_const_iterator(_M_finish); }
    reference front() { return *_M_start; }
    reference back() { return *(_M_finish - 1); }
    size_type size() const { return tinystd::distance(_M_start, _M_finish); }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return _M_start == _M_finish; }
    void push_back(const _Tp &__val) {
        if (_M_finish._M_cur != _M_finish._M_last - 1) 
            { tinystd::construct(_M_finish._M_cur++, __val); }
        else 
            { _M_push_back_aux(__val); }
    }
    void push_front(const _Tp &__val) {
        if (_M_start._M_cur != _M_start._M_first) 
            {  tinystd::construct(--_M_start._M_cur, __val); }
        else 
            { _M_push_front_aux(__val); }
    }
    void pop_back() {
        if (_M_finish._M_cur != _M_finish._M_first) 
            { 
                tinystd::destory(--_M_finish._M_cur); 
            }
        else
            { _M_pop_back_aux(); }
    }
    void pop_front() {
        if (_M_start._M_cur != _M_start._M_last - 1) 
            { tinystd::destory(_M_start._M_cur++); }
        else
            { _M_pop_front_aux(); }
    }
    void clear() {
        _M_clear_data();
        // TODO: try not to free the maps!

        // make a new _M_start
        *_M_start._M_map = _M_allocate_buffer();
        _M_start._M_set_node(_M_start._M_map);
        _M_start._M_cur = _M_start._M_first;

        // finish = start so the size is zero
        _M_finish = _M_start;
    }
    /**
     * @brief Removes the element at pos.
     * All iterators and references are invalidated, 
     * unless the erased elements are at the end or the beginning of the container, 
     * in which case only the iterators and references to the erased elements are invalidated.
     * @return iterator 
     * Iterator following the last removed element.
     * If pos refers to the last element, then the end() iterator is returned.
     * If last==end() prior to removal, then the updated end() iterator is returned.
     * If [first, last) is an empty range, then last is returned.
     * 
     */    
    iterator erase(iterator __pos) {
        size_type __distance = tinystd::distance(_M_start, __pos);
        iterator __next = __pos;
        // !!!Don't do destory because copy will do it!
        // tinystd::destory(__pos._M_cur);
        if (__distance < (size() >> 1)) {
            tinystd::copy_backward(_M_start, __pos, __pos + 1);
            pop_front();
        } else {
            tinystd::copy(__pos + 1, _M_finish, __pos);
            pop_back();
        }
        return _M_start + __distance;
    }
    iterator erase(iterator __lhs, iterator __rhs) {
        size_type __left = tinystd::distance(_M_start, __lhs);
        size_type __right = tinystd::distance(__rhs, _M_finish);
        size_type __width = tinystd::distance(__lhs, __rhs);
        if (__left < __right) {
            tinystd::copy_backward(_M_start, __lhs, __rhs);
            for (int __w = 0; __w < __width; ++__w)
                { pop_front(); }
        } else {
            tinystd::copy(__rhs, _M_finish, __lhs);
            for (int __w = 0; __w < __width; ++__w)
                { pop_back(); }
        }
        return _M_start + __left;
    }

    /**
     * @brief inserts value before pos
     * 
     * @return Iterator pointing to the inserted value 
     */
    iterator insert(iterator __pos, const _Tp &__data) {
        size_type __d = tinystd::distance(_M_start, __pos);
        if (__d <= (size() >> 1)) {
            // Push a random shit at the front... 
            // Just for the convenience of applying for space :)
            push_front(__data);
            tinystd::copy(_M_start + 1, __pos, _M_start);
        } else {
            push_back(__data);
            tinystd::copy_backward(__pos, _M_finish, _M_finish + 1);
        }
        // the real insert. :)
        *(_M_start + __d) = __data;
        return _M_start + __d;
    }
    iterator insert(iterator __pos, size_type __n, const _Tp &__data) {
        if (!__n)
            { return __pos; }
        size_type __d = tinystd::distance(_M_start, __pos);
        if (__d <= (size() >> 1)) {
            // push some random shit in the front place 
            // and then copy
            for (size_type __i = 0; __i < __n; ++__i) 
                { push_front(__data); }
            copy(_M_start + __n, __pos, _M_start);
        } else { 
            iterator __tmp = _M_finish;
            for (size_type __i = 0; __i < __n; ++__i)
                { push_back(__data); }
            copy_backward(__pos, __tmp, _M_finish);
        }
        // the real insert. :)
        tinystd::fill_n(_M_start + __d, __n, __data);
        return _M_start + __d;
    }
    template <typename _InputIter, tinystd::void_t<decltype(*tinystd::__declval<_InputIter>())>* = nullptr>
    iterator insert(iterator __pos, _InputIter __start, _InputIter __finish) {
        if (__start == __finish)
            { return __pos; }
        size_type __d = tinystd::distance(_M_start, __pos);
        size_type __n = tinystd::distance(__start, __finish);
        if (__d <= (size() >> 1)) {
            // push some random shit in the front place 
            // and then copy
            for (size_type __i = 0; __i < __n; ++__i) 
                { push_front(*__start); }
            copy(_M_start + __n, __pos, _M_start);
        } else { 
            iterator __tmp = _M_finish;
            for (size_type __i = 0; __i < __n; ++__i)
                { push_back(*__start); }
            copy_backward(__pos, __tmp, _M_finish);
        }
        // the real insert. :)
        tinystd::copy(__start, __finish, _M_start + __d);
        return _M_start + __d;
    }
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
        try {
            tinystd::uninitialized_fill(_M_start, _M_finish, __value);
        } catch (...) {
            // TODO: add code here (roll back ... ^^)
        }
    }

    template <typename _InputIter>
    void _M_copy_initialize(_InputIter __start, _InputIter __finish) {
        size_type __n = tinystd::distance(__start, __finish);
        _M_create_map_and_nodes(__n);
        try {
            tinystd::uninitialized_copy(__start, __finish, _M_start);
        } catch (...) {
            // TODO: add code here (roll back ... ^^)
        }
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
        // TODO: checkme: the other space is undefined. 
        _M_map = __map_allocator::_S_allocate(_M_map_size);

        __map_pointer __new_start = _M_map + ((_M_map_size - __num_nodes) >> 1);
        __map_pointer __new_finish = __new_start + __num_nodes - 1;


        __map_pointer __cur;
        try {
            for (__cur = __new_start; __cur <= __new_finish; ++__cur) 
                *__cur = _M_allocate_buffer();
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

    void _M_push_back_aux(const _Tp &__val) {
        // make a new map. 
        _M_reserve_map_at_back();
        *(_M_finish._M_map + 1) = _M_allocate_buffer(); 
        try {
            // construct the last val;
            tinystd::construct(_M_finish._M_cur, __val);
            _M_finish._M_set_node(_M_finish._M_map + 1);
            _M_finish._M_cur = _M_finish._M_first;
        } catch (...) {
            // roll back. 
            // If not everyone is successfully create. 
            // then clear every one. 
            // TODO: add code here. 
        }
    }
    void _M_push_front_aux(const _Tp &__val) {
        // make a new map. 
        _M_reserve_map_at_front();
        *(_M_start._M_map - 1) = _M_allocate_buffer();
        try {
            // construct the last val;
            _M_start._M_set_node(_M_start._M_map - 1);
            _M_start._M_cur = _M_start._M_last - 1;
            tinystd::construct(_M_start._M_cur, __val);
        } catch (...) {
            // roll back. 
            // If not everyone is successfully create. 
            // then clear every one. 
            // TODO: add code here. 
        }
    }

    void _M_pop_back_aux() {
        _M_free_buffer(_M_finish._M_first);
        _M_finish._M_set_node(_M_finish._M_map - 1);
        _M_finish._M_cur = _M_finish._M_last - 1;
        tinystd::destory(_M_finish._M_cur);
    }

    void _M_pop_front_aux() {
        tinystd::destory(_M_start._M_cur);
        _M_free_buffer(_M_start._M_first);
        _M_start._M_set_node(_M_start._M_map + 1);
        _M_start._M_cur = _M_start._M_first;
    }


    /**
     * @brief 
     * +---+---+---+---+
     * |   |   |   |   |
     * +---+---+---+---+
     *             ^
     *             |
     *           finish.map 
     */
    void _M_reserve_map_at_back() {
        if (_M_finish._M_map == _M_map + _M_map_size - 1) {
            _M_reallocate_map();
        }
    }

    void _M_reserve_map_at_front() {
        if (_M_start._M_map == _M_map) {
            _M_reallocate_map();
        }
    }

    void _M_reallocate_map() {
        // TODO: add roll back

        size_type __new_map_size = _M_map_size << 1;
        size_type __num_nodes = _M_finish._M_map - _M_start._M_map;
        __map_pointer __new_map = __map_allocator::_S_allocate(__new_map_size);
        __map_pointer __node_start = __new_map + ((__new_map_size - __num_nodes) >> 1);
        
        // _M_map is just stored pointers of (Tp *) so just use [copy];
        // TODO: checkme: the other space is undefined. 
        tinystd::copy(_M_start._M_map, _M_finish._M_map + 1, __node_start);

        // free the old _M_map
        __map_allocator::_S_deallocate(_M_map, _M_map_size);

        // setup member
        _M_map = __new_map;
        _M_map_size = __new_map_size;

        // !!!There is no need to set _M_start._M_cur and _M_finish._M_cur
        // because they are point to the correct place!
        _M_start._M_set_node(__node_start);
        _M_finish._M_set_node(__node_start + __num_nodes);
    }

    _Tp *_M_allocate_buffer() {
        return __data_allocator::_S_allocate(_S_buffer_size());
    }

    void _M_clear_data() {
        // destroy every _Tp    (_Tp::~_Tp())
        tinystd::destory(_M_start, _M_finish);

        // free every buffer
        __map_pointer __begin = _M_start._M_map;
        __map_pointer __end = _M_finish._M_map + 1;
        while (__begin != __end) 
            { _M_free_buffer(*__begin++); }
    }

    void _M_free_buffer(_Tp *__buffer) {
        return __data_allocator::_S_deallocate(__buffer, _S_buffer_size());
    }

    const_iterator _M_to_const_iterator(iterator __it) const {
        return const_iterator(__it._M_cur, __it._M_first, __it._M_last, __it._M_map);
    }
};

}

#endif // __TINYSTL_DEQUE_H
