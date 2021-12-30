#ifndef __TINYSTL_STRING_H
#define __TINYSTL_STRING_H

#include "tinystl_algo.h"
#include "tinystl_alloc.h"
#include "tinystl_types.h"
#include "tinystl_uninitialized.h"

// for debug
#include <ostream>
#include <stdio.h>

// for strcmp, strlen 
#include <string.h>
// for the list init
#include <initializer_list>
namespace tinystd {

struct __long_mode {
    size_t _M_cap;
    size_t _M_size;
    char *_M_data;
};

struct __short_mode {
    unsigned char _M_size;
    char _M_data[23];
};
//
// _M_l._M_cap & 1
// _M_s._M_szie & 1
//
union __string_data {
    __long_mode _M_l;
    __short_mode _M_s;
};

template <typename _Alloc> class basic_string;
template <typename _Alloc>
std::ostream &operator<<(std::ostream &__out, const basic_string<_Alloc> &__str);


#define __max_short_cap 22
template <typename _Alloc = __DEFAULT_ALLOCATOR>
class basic_string {
    friend std::ostream &operator<< <>(std::ostream &__out, const basic_string<> &__str);
public:
    typedef char value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t different_type;

    //
    // Init all to zero. 
    //
    basic_string() : _M_data{0} {}
    basic_string(size_type __n, char __c) : basic_string() { _M_fill_initialize(__n, __c); }
    basic_string(const char *__cstr) : basic_string() {
        int __len = strlen(__cstr);
        _M_copy_initialize(__cstr, __cstr + __len, __len);
    }
    basic_string(const char *__cstr, size_type __count) : basic_string() {
        _M_copy_initialize(__cstr, __cstr + __count, __count);
    }
    template <typename _InputIter, tinystd::void_t<decltype(*tinystd::__declval<_InputIter>())>* = nullptr>
    basic_string(_InputIter __lhs, _InputIter __rhs) : basic_string() {
        _M_copy_initialize(__lhs, __rhs);
    }
    basic_string(const basic_string& __other, size_type __pos, size_type __count) : basic_string() {
        _M_copy_initialize(__other.begin() + __pos, __other.begin() + __pos + __count);
    }
    basic_string(const basic_string& __other, size_type __pos) : basic_string() {
        _M_copy_initialize(__other.begin() + __pos, __other.end());
    }
    basic_string(std::initializer_list<char> __li) : basic_string() { 
        _M_copy_initialize(__li.begin(), __li.end());
    }
    ~basic_string() {
        if (_M_long_mode()) {
            __char_allocator::_S_deallocate(_M_data._M_l._M_data, _M_data._M_l._M_cap);
        }
    }

    basic_string &operator+(const basic_string &__other) {
        size_type __sz = size();
        _M_transform_shape(__other.size());
        tinystd::uninitialized_copy(__other.begin(), __other.end(), begin() + __sz);
        return *this;   
    }

    basic_string &operator+(char __c) { push_back(__c); return *this; }

    basic_string &operator=(const basic_string &__other) {
        clear();
        _M_set_size(__other.size());
        tinystd::uninitialized_copy(__other.begin(), __other.end(), begin());
        return *this;
    }

    basic_string &operator=(char __c) { *this = basic_string{__c}; return *this; }

    iterator begin() { return const_cast<iterator>(static_cast<const basic_string *>(this)->begin()); }
    iterator end() { return const_cast<iterator>(static_cast<const basic_string *>(this)->end()); }
    const_iterator begin() const {
        if (_M_long_mode()) return _M_data._M_l._M_data;
        else return _M_data._M_s._M_data;
    }
    const_iterator end() const {
        if (_M_long_mode()) return _M_data._M_l._M_data + _M_data._M_l._M_size;
        else return _M_data._M_s._M_data + (_M_data._M_s._M_size >> 1);
    }
    size_type length() const { return size(); }
    size_type size() const {
        if (_M_long_mode()) { 
            return _M_data._M_l._M_size; 
        } else { 
            return _M_data._M_s._M_size >> 1; 
        }
    }
    size_type capacity() const {
        if (_M_long_mode()) {
            return _M_data._M_l._M_cap; 
        } else {
            return __max_short_cap;
        }
    }
    bool empty() const { return size() == 0; }

    void push_back(char __c) {
        int __idx = size();
        _M_transform_shape(1);
        if (_M_long_mode()) {
            _M_data._M_l._M_data[__idx++] = __c;
        } else {
            _M_data._M_s._M_data[__idx++] = __c;
        }
    }

    void clear() 
        { memset(begin(), 0, size()); }

    void assign(size_type __cnt, char __c) 
        { _M_fill_assign(__cnt, __c); }

    void assign(const basic_string &__other) 
        { _M_copy_assign(__other.begin(), __other.end(), __other.size()); }    
    
    void assign(const basic_string &__other, size_type __pos, size_type __cnt) 
        { _M_copy_assign(__other.begin() + __pos, __other.begin() + __pos + __cnt, __other.size()); }

    void assign(const char *__cstr, size_type __pos) 
        { _M_copy_assign(__cstr, __cstr + __pos, __pos); }

    template <typename _InputIter>
    void assign(_InputIter __lhs, _InputIter __rhs)
        { _M_copy_assign(__lhs, __rhs); }

    void __debug_message() const {
        printf("string: 0x%lx, 0x%lx, 0x%lx\n", (long) _M_data._M_l._M_data, 
                                           (long) _M_data._M_l._M_size, 
                                           (long) _M_data._M_l._M_cap);
    }

protected:
    typedef simple_alloc<char, _Alloc> __char_allocator;
    __string_data _M_data;

    bool _M_long_mode() const { return _M_data._M_s._M_size & 1; }

    void _M_size_init(size_type __len) {
        if (__len < __max_short_cap) {
            _M_data._M_s._M_size = __len << 1;
        } else {
            _M_data._M_l._M_size = __len;
            size_type __cap = __len + 1;
            if (!(__cap & 1)) ++__cap;
            _M_data._M_l._M_cap = __cap;
            _M_data._M_l._M_data = __char_allocator::_S_allocate(__cap);

            // 
            // Set the memory to zero!
            //
            memset(_M_data._M_l._M_data, 0, __cap);
        }
    }

    void _M_fill_initialize(size_type __n, char __c) {
        _M_size_init(__n);
        tinystd::uninitialized_fill_n(begin(), __n, __c);
    }

    template <typename _InputIter>
    void _M_copy_initialize(_InputIter __lhs, _InputIter __rhs) {
        size_type __len = tinystd::distance(__lhs, __rhs);
        _M_copy_initialize(__lhs, __rhs, __len);
    }

    template <typename _InputIter>
    void _M_copy_initialize(_InputIter __lhs, _InputIter __rhs, size_type __len) {
        _M_size_init(__len);
        tinystd::uninitialized_copy(__lhs, __rhs, begin());
    }

    void _M_fill_assign(size_type __n, char __c) {
        memset(begin(), 0, size());
        size_type __sz = size();
        if (__n > __sz) {
            _M_transform_shape(__n - __sz);
        }
        
        tinystd::uninitialized_fill_n(begin(), __n, __c);
    } 

    template <typename _InputIter>
    void _M_copy_assign(_InputIter __lhs, _InputIter __rhs, size_type __len) {
        memset(begin(), 0, size());
        size_type __sz = size();
        if (__len > __sz) {
            _M_transform_shape(__len - __sz);
        }
        tinystd::uninitialized_copy(__lhs, __rhs, begin());
    }

    template <typename _InputIter>
    void _M_copy_assign(_InputIter __lhs, _InputIter __rhs) {
        size_type __len = tinystd::distance(__lhs, __rhs);
        _M_copy_initialize(__lhs, __rhs, __len);
    }

    void _M_transform_shape(size_type __n) {
        size_type __sz = size();
        size_type __cap = capacity();   
                         /* \0 */
        if (__sz + __n >= __cap - 1) {
            size_type __new_sz = tinystd::max(__sz + __n, __cap << 1);

            // the new mode is long mode 
            // so the new capa is a Odd number
            if (__new_sz % 2 == 0) ++__new_sz; 
            char *__new_space = __char_allocator::_S_allocate(__new_sz);
            // set the memory to zero. 
            memset(__new_space, 0, __cap << 1);
            // copy to the new space
            tinystd::uninitialized_copy(begin(), end(), __new_space);
            // delete the old space, if it exist. 
            if (_M_long_mode()) {
                __char_allocator::_S_deallocate(_M_data._M_l._M_data, __cap);
            }

            // setup the _M_data. 
            _M_data._M_l._M_cap = __new_sz;
            _M_data._M_l._M_data = __new_space;
        } 
        _M_set_size(__sz + __n);
    }

    void _M_set_size(size_type __sz) {
        if (_M_long_mode()) {
            _M_data._M_l._M_size = __sz;
        } else {
            _M_data._M_s._M_size = __sz << 1;
        }
    }
};
#undef __max_short_cap

template <typename _Alloc>
std::ostream &operator<<(std::ostream &__out, const basic_string<_Alloc> &__str) {
    if (__str._M_long_mode()) {
        __out << __str._M_data._M_l._M_data;
    } else {
        __out << __str._M_data._M_s._M_data;
    }
    return __out;
}

// 
// There is the real string... 
//
typedef basic_string<> string;

} // namespace tinystd
#endif // __TINYSTL_STRING_H
