#ifndef __TINYSTL_VECTOR_H
#define __TINYSTL_VECTOR_H
#include "tinystl_algo.h"
#include "tinystl_alloc.h"
#include "tinystl_constructor.h"
#include "tinystl_types.h"
#include "tinystl_uninitialized.h"

#define __DEFAULT_ALLOCATOR __default_alloc_template<0>
namespace tinystd {

template <typename _Tp, typename _Alloc = __DEFAULT_ALLOCATOR>
class vector {
public:
    typedef _Tp             value_type;
    typedef value_type*     pointer;
    typedef value_type*     iterator;
    typedef value_type&     reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       different_type;

protected:
    typedef simple_alloc<value_type, _Alloc> __data_allocator;
    iterator _M_start;
    iterator _M_finish;
    iterator _M_end_of_storage;

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

    ~vector() {
        destory(_M_start, _M_finish);
        _M_deallocate();
    }

    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return _M_finish - _M_start; }
    size_type capacity() const { return _M_end_of_storage - _M_start; }
    bool empty() const { return begin() == end(); }
    reference operator[](size_type __sz) { return *(begin() + __sz); }
    reference front() { return *begin(); }
    reference back() { return *end(); }
    void push_back(const _Tp &__val) {
        if (_M_finish != _M_end_of_storage) {
            construct(_M_finish++, __val);
        } else {
            _M_insert_aux(end(), __val);
        }
    }
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
    iterator insert(iterator __begin, size_type __sz, const _Tp &__val) {

    }
    void resize(size_type __new_sz, const _Tp &__val) {
        if (__new_sz < size()) { erase(begin() + __new_sz, end()); }
        else { insert(end(), __new_sz - size(), __val); }
    }
    void resize(size_type __new_sz) { resize(__new_sz, _Tp()); }
    void pop_back() { destory(--_M_finish); }
};


}

#endif // __TINYSTL_VECTOR_H
