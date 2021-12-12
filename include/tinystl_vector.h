#ifndef __TINYSTL_VECTOR_H
#define __TINYSTL_VECTOR_H
#include "tinystl_iterator_base.h"
#include "tinystl_algo.h"
#include "tinystl_alloc.h"
#include "tinystl_constructor.h"
#include "tinystl_move.h"
#include "tinystl_types.h"
#include "tinystl_uninitialized.h"

// use c++ init list. 
#include <initializer_list>
#define __DEFAULT_ALLOCATOR __malloc_alloc_template<0>
namespace tinystd {

template <typename _Tp, typename _Alloc = __DEFAULT_ALLOCATOR>
class vector {
public:
    typedef _Tp                     value_type;
    typedef value_type*             pointer;
    typedef const value_type*       const_pointer;
    typedef value_type*             iterator;
    typedef const value_type*       const_iterator;
    typedef value_type&             reference;
    typedef const value_type&       const_reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t               different_type;

protected:
    typedef simple_alloc<value_type, _Alloc> __data_allocator;
    iterator _M_start;
    iterator _M_finish;
    iterator _M_end_of_storage;

    template <typename _InputIter>
    void _M_copy_initialize(_InputIter __lhs, _InputIter __rhs) {
        size_type __sz = tinystd::distance(__lhs, __rhs);
        _M_start = __data_allocator::_S_allocate(__sz);
        _M_finish = _M_end_of_storage = _M_start + __sz;
        tinystd::uninitialized_copy(__lhs, __rhs, _M_start);
    }

    void _M_fill_initialize(size_type __sz, const value_type &__val) {
        _M_start = _M_allocate_and_fill(__sz, __val);
        _M_finish = _M_end_of_storage = _M_start + __sz;
    }

    iterator _M_allocate_and_fill(size_type __sz, const value_type &__val) {
        iterator __res = __data_allocator::_S_allocate(__sz);
        tinystd::uninitialized_fill_n(__res, __sz, __val);
        return __res;
    }

    void _M_deallocate() {
        if (_M_start) 
            __data_allocator::_S_deallocate(_M_start, _M_end_of_storage - _M_start);
    }

    /**
     * @brief insert a _Tp in the pos.  
     * 
     * @param __pos 
     * @param __val 
     */
    void _M_insert_aux(iterator __pos, const _Tp &__val) {
        if (_M_finish != _M_end_of_storage) {
            construct(_M_finish, *(_M_finish - 1));
            ++_M_finish;
            tinystd::copy_backward(__pos, _M_finish - 2, _M_finish - 1);
            *__pos = __val;
        } else {
            size_type __old_sz = size();
            size_type __new_sz = __old_sz ? (__old_sz << 1) : 1;
            iterator __new_start = __data_allocator::_S_allocate(__new_sz), __new_finish;
            try {
                __new_finish = tinystd::uninitialized_copy(_M_start, _M_finish, __new_start);
                construct(__new_finish++, __val);
                __new_finish = tinystd::uninitialized_copy(__pos, _M_finish, __new_finish);
            } catch(...) {
                // roll back... 
                destory(__new_start, __new_finish);
                __data_allocator::_S_deallocate(__new_start, __new_sz);
            }
            destory(begin(), end());
            _M_deallocate();

            _M_start = __new_start;
            _M_finish = __new_finish;
            _M_end_of_storage = _M_start + __new_sz;
        }
    }


public:
    vector() 
        : _M_start(0),
          _M_finish(0),
          _M_end_of_storage(0) {}
    vector(size_type __sz, const value_type &__val) { _M_fill_initialize(__sz, __val); }
    vector(int __sz, const value_type &__val) { _M_fill_initialize(__sz, __val); }
    vector(long __sz, const value_type &__val) { _M_fill_initialize(__sz, __val); }
    explicit vector(size_type __sz) { _M_fill_initialize(__sz, value_type()); }
    vector(std::initializer_list<_Tp> __li) { _M_copy_initialize(__li.begin(), __li.end()); }
    template <typename _InputIter> 
    vector(_InputIter __begin, _InputIter __end) { _M_copy_initialize(__begin, __end); }
    vector(const vector &__vec) {
        iterator __new_start;
        size_t __capa = __vec._M_end_of_storage - __vec._M_start;
        size_t __sz = __vec.size();
        try {
            if (__vec.size()) {
                __new_start = __data_allocator::_S_allocate(__capa);
                tinystd::uninitialized_copy(__vec._M_start, __vec._M_finish, __new_start);
            }
        } catch (...) {
            destory(__new_start, __new_start + __capa);
            __data_allocator::_S_deallocate(__new_start, __capa);
        }

        _M_start = __new_start;
        _M_finish = __new_start + __sz;
        _M_end_of_storage = __new_start + __capa;
    }
    ~vector() {
        destory(_M_start, _M_finish);
        _M_deallocate();
    }

    reference operator[](size_type __sz) { return *(begin() + __sz); }
    const_reference operator[](size_type __sz) const { return *(begin() + __sz); }
    vector &operator=(const vector &__vec) {
        if (&__vec == this) return *this;
        iterator __new_start;
        size_t __capa = __vec._M_end_of_storage - __vec._M_start;
        size_t __sz = __vec.size();
        try {
            if (__vec.size()) {
                __new_start = __data_allocator::_S_allocate(__capa);
                tinystd::uninitialized_copy(__vec._M_start, __vec._M_finish, __new_start);
            }
        } catch (...) {
            destory(__new_start, __new_start + __capa);
            __data_allocator::_S_deallocate(__new_start, __capa);
        }

        destory(_M_start, _M_finish);
        _M_deallocate();

        _M_start = __new_start;
        _M_finish = __new_start + __sz;
        _M_end_of_storage = __new_start + __capa;

        return *this;
    }

    iterator begin() { return _M_start; }
    const_iterator begin() const { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator end() const { return _M_finish; }
    size_type size() const { return _M_finish - _M_start; }
    size_type capacity() const { return _M_end_of_storage - _M_start; }
    bool empty() const { return begin() == end(); }
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *end(); }
    const_reference back() const { return *end(); }
    void push_back(const _Tp &__val) {
        if (_M_finish != _M_end_of_storage) {
            construct(_M_finish++, __val);
        } else {
            _M_insert_aux(end(), __val);
        }
    }

    void push_back(_Tp &&__val) {
        emplace_back(tinystd::move(__val));
    }

    template <typename... _Args>  
    reference &emplace_back(_Args &&...__args) {
        if (_M_finish != _M_end_of_storage) {
            construct(_M_finish++, _Tp(tinystd::forward(__args))...);
        } else {
            _M_insert_aux(end(), _Tp(tinystd::forward(__args))...);
        }
        return back();
    }
    /**
     * @return return the same pos,  
     *         but if the pos's next is end, return end;
     */
    iterator erase(iterator __pos) {
        if (__pos + 1 != end())
            tinystd::copy(__pos + 1, _M_finish, __pos);
        --_M_finish;
        destory(_M_finish);
        return __pos;
    }
    iterator erase(iterator __begin, iterator __end) {
        iterator __last = tinystd::copy(__end, _M_finish, __begin);
        destory(__last, _M_finish);
        _M_finish -= __end - __begin;
        return __begin;
    }
    iterator insert(iterator __pos, size_type __sz, const _Tp &__val) {
        if (__sz > 0) {
            if (_M_end_of_storage - _M_finish >= __sz) {
                int __element_after = _M_finish - __pos;
                iterator __old_finish = _M_finish;
                if (__element_after > __sz) {
                    tinystd::uninitialized_copy(_M_finish - __sz, _M_finish, _M_finish);
                    _M_finish += __sz;
                    tinystd::copy_backward(__pos, __old_finish - __sz, __old_finish);
                    tinystd::fill(__pos, __sz, __val);
                } else {
                    tinystd::uninitialized_fill_n(_M_finish, __sz - __element_after, __val);
                    _M_finish += __sz - __element_after;
                    tinystd::uninitialized_copy(__pos, __old_finish, _M_finish);
                    _M_finish += __element_after;
                    tinystd::fill(__pos, __old_finish, __val);
                }
            } else {
                const size_type __old_sz = size();
                const size_type __new_sz = __old_sz + tinystd::max(__old_sz, __sz);
                iterator __new_start = __data_allocator::_S_allocate(__new_sz);
                iterator __new_finish = __new_start;
                try {
                    __new_finish = tinystd::uninitialized_copy(_M_start, __pos, __new_start);
                    __new_finish = tinystd::uninitialized_fill_n(__new_finish, __sz, __val);
                    __new_finish = tinystd::uninitialized_copy(__pos, _M_finish, __new_finish);
                } catch (...) {
                    // roll back... 
                    destory(__new_start, __new_finish);
                    __data_allocator::_S_deallocate(__new_start, __new_sz);
                }

                destory(_M_start, _M_finish);
                _M_deallocate();

                _M_start = __new_start;
                _M_finish = __new_finish;
                _M_end_of_storage = __new_start + __new_sz;
            }
        }
    }
    void resize(size_type __new_sz, const _Tp &__val) {
        if (__new_sz < size()) { erase(begin() + __new_sz, end()); }
        else { insert(end(), __new_sz - size(), __val); }
    }
    void resize(size_type __new_sz) { resize(__new_sz, _Tp()); }
    void pop_back() { destory(--_M_finish); }
    void clear() { erase(begin(), end()); }
};


}

#endif // __TINYSTL_VECTOR_H
