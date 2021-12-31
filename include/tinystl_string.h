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

template <typename _CharT>
struct __long_mode {
    size_t _M_cap;
    size_t _M_size;
    _CharT *_M_data;
};

template <typename _CharT>
struct __short_mode {
    unsigned char _M_size;
    _CharT _M_data[23];
};
//
// _M_l._M_cap & 1
// _M_s._M_szie & 1
//
template <typename _CharT>
union __string_data {
    __long_mode<_CharT> _M_l;
    __short_mode<_CharT> _M_s;
};

template <typename _CharT, typename _Alloc> class basic_string;
template <typename _CharT, typename _Alloc>
std::ostream &operator<<(std::ostream &__out, const basic_string<_CharT, _Alloc> &__str);

#define __max_short_cap 22
template <typename _CharT, typename _Alloc = __DEFAULT_ALLOCATOR>
class basic_string {
    friend std::ostream &operator<< <>(std::ostream &__out, const basic_string<_CharT> &__str);
public:
    typedef _CharT value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t different_type;
    
    static size_type npos;

    //
    // Init all to zero. 
    //
    basic_string() : _M_data{0} {}
    basic_string(size_type __n, _CharT __c) : basic_string() { _M_fill_initialize(__n, __c); }
    basic_string(const _CharT *__cstr) : basic_string() {
        int __len = strlen(__cstr);
        _M_copy_initialize(__cstr, __cstr + __len, __len);
    }
    basic_string(const _CharT *__cstr, size_type __count) : basic_string() {
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
    basic_string(std::initializer_list<_CharT> __li) : basic_string() { 
        _M_copy_initialize(__li.begin(), __li.end());
    }
    ~basic_string() {
        if (_M_long_mode()) {
            __char_allocator::_S_deallocate(_M_data._M_l._M_data, _M_data._M_l._M_cap);
        }
    }
    
    reference operator[](size_type __idx) 
        { return begin()[__idx]; }

    const_reference operator[](size_type __idx) const
        { return begin()[__idx]; }

    basic_string &operator+=(const basic_string &__other) 
        { _M_copy_append(__other.begin(), __other.end(), __other.size()); return *this; }

    basic_string &operator+=(_CharT __c) { push_back(__c); return *this; }
    
    basic_string operator+(const basic_string &__other) {
        basic_string __tmp = *this;
        __tmp += __other;
        return __tmp;
    }

    basic_string operator+(_CharT __c) {
        basic_string __tmp = *this;
        __tmp += __c;
        return __tmp;
    }

    basic_string &operator=(const basic_string &__other) {
        clear();
        _M_set_size(__other.size());
        tinystd::uninitialized_copy(__other.begin(), __other.end(), begin());
        return *this;
    }

    basic_string &operator=(_CharT __c) { *this = basic_string{__c}; return *this; }

    const_iterator cbegin() { return static_cast<const basic_string *>(this)->begin(); }
    const_iterator cend() { return static_cast<const basic_string *>(this)->end(); }
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

    void push_back(_CharT __c) {
        int __idx = size();
        _M_transform_shape(1);
        if (_M_long_mode()) {
            _M_data._M_l._M_data[__idx++] = __c;
        } else {
            _M_data._M_s._M_data[__idx++] = __c;
        }
    }

    void pop_back() 
        { _M_pop_back(1); }

    void clear() 
        { memset(begin(), 0, size()); _M_set_size(0); }

    void assign(size_type __cnt, _CharT __c) 
        { _M_fill_assign(__cnt, __c); }

    void assign(const basic_string &__other) 
        { _M_copy_assign(__other.begin(), __other.end(), __other.size()); }    
    
    void assign(const basic_string &__other, size_type __pos, size_type __cnt) 
        { _M_copy_assign(__other.begin() + __pos, __other.begin() + __pos + __cnt, __other.size()); }

    void assign(const _CharT *__cstr, size_type __pos) 
        { _M_copy_assign(__cstr, __cstr + __pos, __pos); }

    template <typename _InputIter>
    void assign(_InputIter __lhs, _InputIter __rhs)
        { _M_copy_assign(__lhs, __rhs); }

    const_reference at(size_type __idx) const {
        if (__idx >= size()) {
            throw std::out_of_range("out of bound!");
        }
        return begin()[__idx];
    }
    
    reference at(size_type __idx) 
        { return const_cast<reference>((static_cast<const basic_string *>(this))->at(__idx)); }

    reference front() 
        { return *begin(); }

    const_reference front() const
        { return *begin(); }

    reference back() 
        { return *(end() - 1); }

    const_reference back() const
        { return *(end() - 1); }

    void __debug_message() const {
        printf("string: 0x%lx, 0x%lx, 0x%lx\n", (long) _M_data._M_l._M_data, 
                                           (long) _M_data._M_l._M_size, 
                                           (long) _M_data._M_l._M_cap);
    }

    //
    // Difference between data and c_str
    // c_str should return a char * that end of \0
    // data is to return the begin().... 
    // 
    // in c++11 they are same 
    //
    const _CharT *data() const
        { return begin(); }
    
    const _CharT *c_str() const
        { return begin(); } 

    void reserve(size_type __new_cap) {
        size_type __cap = capacity();
        if (__new_cap <= __cap)
            return; 

        if (__new_cap <= __max_short_cap) 
            return;

        if (__new_cap % 2 == 0)        
            ++__new_cap;
        
        size_type __sz = size();
        _CharT *__old_space = 0;
        if (_M_long_mode())
            __old_space = _M_data._M_l._M_data;
        _CharT *__new_space = __char_allocator::_S_allocate(__new_cap);
        //
        // Set to zero
        //
        memset(__new_space, 0, __new_cap);
        tinystd::uninitialized_copy(begin(), end(), __new_space);

        _M_data._M_l._M_cap = __new_cap;
        _M_data._M_l._M_size = __sz;
        _M_data._M_l._M_data = __new_space;

        if (__old_space)
            __char_allocator::_S_deallocate(__old_space, __cap);
    }

    void shrink_to_fit() {
        if (_M_long_mode()) {
            size_type __sz = size();
            if (__sz < __max_short_cap) {
                //
                // Copy bcak to the short mode. 
                // 
                _CharT *__old_data = _M_data._M_l._M_data;
                size_type __cap = capacity();

                // Set the memory to zero. 
                memset(_M_data._M_s._M_data, 0, __max_short_cap);
                tinystd::uninitialized_copy(__old_data, __old_data + __sz, _M_data._M_s._M_data);
                
                // Don't use _M_set_size
                // because the size now is a long mode flag
                _M_data._M_s._M_size = __sz << 1;

                // free the old data... 
                __char_allocator::_S_deallocate(__old_data, __cap);
            } else {
                //
                // make a short capa... 
                // but it is still long mode... 
                // 
                _CharT *__old_data = _M_data._M_l._M_data;
                size_type __cap = capacity();
                size_type __new_cap = __sz + 1;
                if (__new_cap % 2 == 0) ++__new_cap;

                // Allocate a new space, it's size is __sz + 1... 
                _CharT *__new_space = __char_allocator::_S_allocate(__new_cap);
                memset(__new_space, 0, __new_cap);

                // copy to the new space
                tinystd::uninitialized_copy(__old_data, __old_data + __sz, __new_space);
                
                _M_data._M_l._M_cap = __new_cap;
                _M_data._M_l._M_data = __new_space;

                // free the old data... 
                __char_allocator::_S_deallocate(__old_data, __cap);
               
            }
        }
    }

    basic_string &insert(size_type __pos, size_type __cnt, _CharT __c)
        { _M_fill_insert(__pos, __cnt, __c); return *this; }

    basic_string &insert(size_type __pos, const basic_string &__str)
        { _M_copy_insert(__pos, __str.begin(), __str.end(), __str.size()); return *this; }

    basic_string &insert(size_type __pos, const _CharT *__cstr) {
        size_t __len = strlen(__cstr);
        _M_copy_insert(__pos, __cstr, __cstr + __len, __len);
        return *this;
    }

    basic_string &insert(size_type __pos, const basic_string &__str, size_type __idx, size_type __cnt)
        { _M_copy_insert(__pos, __str.begin() + __idx, __str.begin() + __idx + __cnt, __cnt); return *this; }

    iterator insert(const_iterator __pos, _CharT __c) { 
        size_type __off = __pos - begin();
        _M_copy_insert(__off, &__c, &__c + 1, 1); 
        return begin() + __off;
    }

    iterator insert(const_iterator __pos, size_type __cnt, _CharT __c) { 
        size_type __off = __pos - begin();
        _M_fill_insert(__off, __cnt, __c); 
        return begin() + __off;
    }

    template <typename _InputIter>
    iterator insert(const_iterator __pos, _InputIter __lhs, _InputIter __rhs) { 
        size_type __off = __pos - begin();
        _M_copy_insert(__off, __lhs, __rhs); 
        return begin() + __off; 
    }

    size_type find_first_of(_CharT __c) {
        iterator __pos = tinystd::find(begin(), end(), __c);
        if (__pos == end()) return -1;
        return __pos - begin();
    }

    size_type find_last_of(_CharT __c) {
        for (size_type __off = size(); __off >= 0; --__off) {
            if (operator[](__off) == __c)
                return __off;
        }
        return -1;
    }
    
    iterator erase(iterator __pos) { 
        size_type __off = __pos - begin(); 
        _M_copy_replace(__off, 1, begin(), begin()); 
        return begin() + __off;
    }

    iterator erase(iterator __lhs, iterator __rhs) { 
        size_type __off = __lhs - begin();
        _M_copy_replace(__off, __rhs - __lhs, begin(), begin()); 
        return begin() + __off;
    }

    basic_string &erase(size_type __pos = 0, size_type __cnt = -1) {
        size_type __d = tinystd::min(__cnt, size() - __pos);
        _M_copy_replace(__pos, __d, begin(), begin()); 
        return *this;
    }

    size_type find(_CharT __c) 
        { return find_first_of(__c); }

    size_type find(const basic_string &__sub, size_type __pos, size_type __count) {
        if (__pos + __count >= size()) return -1;
        iterator __it = tinystd::find_if(begin() + __pos, end() - __count, [&](const _CharT &__c) {
            return basic_string(&__c, &__c + __count) == __sub;
        }); 
        if (__it == end() - __count) return -1;
        return __it - begin();
    }

    int compare(const basic_string &__other) 
        { return _M_compare(*this, __other); }

    int compare(size_type __pos, size_type __cnt, const basic_string &__other) 
        { return _M_compare(tinystd::basic_string(*this, __pos, __cnt), __other); }

    int compare(size_type __pos, size_type __cnt, 
            const basic_string &__other, size_type __opos, size_type __ocnt = -1)
        { 
            size_type __d = tinystd::min(__ocnt, __other.size() - __opos);
            return _M_compare(tinystd::basic_string(*this, __pos, __cnt), 
                tinystd::basic_string(__other, __opos, __d)); 
        }
    
    basic_string &replace(size_type __pos, size_type __count, const basic_string &__other) {
        _M_copy_replace(__pos, __count, __other.begin(), __other.end()); 
        return *this;
    }

    basic_string &replace(const_iterator __lhs, const_iterator __rhs, size_type __ocnt, _CharT __c) {
        _M_fill_replace(__lhs - begin(), __rhs - __lhs, __ocnt, __c);
        return *this;
    }

    basic_string &replace(size_type __pos, size_type __count, 
        const basic_string &__other, size_type __opos, size_type __ocnt = -1) {
        size_type __d = tinystd::min(__ocnt, __other.size() - __opos);
        basic_string __tmp(__other, __ocnt, __d); 
        _M_copy_replace(__pos, __count, __tmp.begin(), __tmp.end(), __d);
        return *this;
    }

    basic_string &replace(size_type __pos, size_type __count, 
        const _CharT *__cstr, size_type __ocnt) {
        _M_copy_replace(__pos, __count, __cstr, __cstr + __ocnt, __ocnt);
        return *this;
    }

    basic_string &append(size_type __n, _CharT __c)
        { _M_fill_append(__n, __c); return *this; }

    basic_string &append(const basic_string &__other)
        { _M_copy_append(__other.begin(), __other.end(), __other.size()); return *this; }

    basic_string &append(const basic_string &__other, size_type __pos, size_type __count) { 
        basic_string __tmp(__other, __pos, __count);
        _M_copy_append(__tmp.begin(), __tmp.end(), __tmp.size()); 
        return *this; 
    }

    basic_string &append(const _CharT *__s, size_type __count)
        { _M_copy_append(__s, __s + __count, __count); return *this; }

    template <typename _InputIter>
    basic_string &append(_InputIter __lhs, _InputIter __rhs)
        { _M_copy_append(__lhs, __rhs); return *this; }

    basic_string substr(size_type __pos = 0, size_type __cnt = -1) 
        { 
            size_type __sz = size();
            if (__pos >= __sz)
                throw std::out_of_range("pos is eq or bigger than the size of string. ");
            return basic_string(*this, __pos, tinystd::min(__sz - __pos, __cnt)); 
        }

    size_type copy(_CharT* __dest, size_type __count, size_type __pos = 0) const {
        size_type __sz = size();
        if (__pos >= __sz) 
            throw std::out_of_range("pos is eq or bigger than the size of the string. ");
        size_type __d = tinystd::min(__sz - __pos, __count);
        tinystd::uninitialized_copy(begin() + __pos, 
                begin() + __pos + __d, __dest);
        return __d;
    }

    void resize(size_type __new_sz, _CharT __c = _CharT()) {
        if (__new_sz > max_size())
            throw std::length_error("bigger than max size. ");
        size_type __sz = size();
        if (__new_sz < __sz)
            { _M_pop_back(__sz - __new_sz); }
        else if (__new_sz > __sz)
            { _M_fill_insert(__sz, __new_sz - __sz, __c); }
    }

    size_type max_size() const { return size_type(-1) >> 1; }

    void swap(basic_string &__other) {
        __string_data __tmp = _M_data;
        _M_data = __other._M_data;
        __other._M_data = __tmp;
    }
protected:
    typedef simple_alloc<_CharT, _Alloc> __char_allocator;
    __string_data<_CharT> _M_data;

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

    void _M_fill_initialize(size_type __n, _CharT __c) {
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

    void _M_fill_assign(size_type __n, _CharT __c) {
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
            _CharT *__new_space = __char_allocator::_S_allocate(__new_sz);
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

    void _M_fill_insert(size_type __pos, size_type __n, _CharT __c) {
        size_type __old_sz = size();
        _M_transform_shape(__n);
        tinystd::copy_backward(begin() + __pos, begin() + __old_sz, end());
        tinystd::uninitialized_fill_n(begin() + __pos, __n, __c);
    }

    template <typename _InputIter>
    void _M_copy_insert(size_type __pos, _InputIter __lhs, _InputIter __rhs) {
        size_type __len = tinystd::distance(__lhs, __rhs);
        _M_copy_insert(__pos, __lhs, __rhs, __len); 
    }

    template <typename _InputIter>
    void _M_copy_insert(size_type __pos, _InputIter __lhs, _InputIter __rhs, size_type __len) {
        size_type __old_sz = size();
        _M_transform_shape(__len);
        tinystd::copy_backward(begin() + __pos, begin() + __old_sz, end());
        tinystd::uninitialized_copy(__lhs, __rhs, begin() + __pos);
    }

    void _M_set_size(size_type __sz) {
        if (_M_long_mode()) {
            _M_data._M_l._M_size = __sz;
        } else {
            _M_data._M_s._M_size = __sz << 1;
        }
    }

    void _M_pop_back(size_type __sz) {
        _M_set_size(size() - __sz);
        _CharT *__new_end = end();
        //
        // Set the memory to zero...
        // 
        memset(__new_end, 0x0, __sz);
    }

    int _M_compare(const basic_string<_CharT, _Alloc> &__lhs, const basic_string<_CharT, _Alloc> &__rhs) {
        typename basic_string<_CharT, _Alloc>::size_type __off;
        for (__off = 0; __off < __rhs.size() && __off < __lhs.size(); ++__off) {
            if (__lhs[__off] < __rhs[__off])
                return -1;
            else if (__lhs[__off] > __rhs[__off])
                return 1;
        }
        if (__off == __lhs.size() && __off == __rhs.size())
            return 0;
        else if (__off == __lhs.size())
            return -1;
        else
            return 1;
    }

    void _M_fill_replace(size_type __pos, size_type __cnt, size_type __n, _CharT __c) {
        _M_fix_size(__pos, __cnt, __n);
        tinystd::uninitialized_fill_n(begin() + __pos, __cnt, __c);
    }

    template <typename _InputIter>
    void _M_copy_replace(size_type __pos, size_type __cnt, _InputIter __lhs, _InputIter __rhs) 
        { _M_copy_replace(__pos, __cnt, __lhs, __rhs, tinystd::distance(__lhs, __rhs)); }

    template <typename _InputIter>
    void _M_copy_replace(size_type __pos, size_type __cnt, _InputIter __lhs, _InputIter __rhs, size_type __d) {
        _M_fix_size(__pos, __cnt, __d);
        tinystd::uninitialized_copy(__lhs, __rhs, begin() + __pos);
    }

    void _M_fix_size(size_type __pos, size_type __cnt, size_type __rpsz) {
        if (__rpsz < __cnt) {
            tinystd::uninitialized_copy(begin() + __pos + __cnt, end(), begin() + __pos + __rpsz); 
            _M_pop_back(__cnt - __rpsz); 
        } else if (__rpsz > __cnt) {
            size_type __old_sz = size(); 
            _M_transform_shape(__rpsz - __cnt);
            tinystd::copy_backward(begin() + __pos, begin() + __old_sz, end());
        }
    }

    void _M_fill_append(size_type __n, _CharT __c) {
        size_type __old_sz = size();
        _M_transform_shape(__n);
        tinystd::uninitialized_fill_n(begin() + __old_sz, __n, __c);
    }

    template <typename _InputIter>
    void _M_copy_append(_InputIter __lhs, _InputIter __rhs) 
        { _M_copy_append(__lhs, __rhs, tinystd::distance(__lhs, __rhs)); }

    template <typename _InputIter>
    void _M_copy_append(_InputIter __lhs, _InputIter __rhs, size_type __d) {
        size_type __old_sz = size();
        _M_transform_shape(__d);
        tinystd::uninitialized_copy(__lhs, __rhs, begin() + __old_sz); 
    }
};
#undef __max_short_cap

template <typename _CharT, typename _Alloc>
typename basic_string<_CharT, _Alloc>::size_type basic_string<_CharT, _Alloc>::npos = -1;

template <typename _CharT, typename _Alloc>
std::ostream &operator<<(std::ostream &__out, const basic_string<_CharT, _Alloc> &__str) {
    if (__str._M_long_mode()) {
        __out << __str._M_data._M_l._M_data;
    } else {
        __out << __str._M_data._M_s._M_data;
    }
    return __out;
}

template <typename _CharT, typename _Alloc>
bool operator==(const basic_string<_CharT, _Alloc> &__lhs, const basic_string<_CharT, _Alloc> &__rhs) {
    return __lhs.size() == __rhs.size() && 
        tinystd::equal(__lhs.begin(), __lhs.end(), __rhs.begin());
}

//
// There is the real string...
//
typedef basic_string<char> string;

} // namespace tinystd
#endif // __TINYSTL_STRING_H
