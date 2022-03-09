#ifndef _TINYSTL_HASHTABLE_
#define _TINYSTL_HASHTABLE_

#include "tinystl_alloc.h"
#include "tinystl_constructor.h"
#include "tinystl_functional.h"
#include "tinystl_pair.h"
#include "tinystl_types.h"
#include "tinystl_alloc.h"
#include "tinystl_vector.h"
namespace tinystd {

template <typename _Key, typename _Tp, typename _HashFunc, typename _Equal, typename _Alloc> 
class unordered_map;

template <typename _Value> 
struct __hashtable_node {
    __hashtable_node *_M_next;
    _Value _M_val;
};

template <typename _Key, typename _Tp, typename _HashFunc, typename _Equal, typename _Alloc> 
struct __hashtable_iterator;

template <typename _Key, typename _Tp, typename _HashFunc, typename _Equal, typename _Alloc> 
struct __hashtable_iterator {
    typedef __hashtable_node<tinystd::pair<const _Key, _Tp>> __node;
    typedef unordered_map<_Key, _Tp, _HashFunc, _Equal, _Alloc> __hashtable;
    typedef __hashtable_iterator<_Key, _Tp, _HashFunc, _Equal, _Alloc> __self;

    // 
    // iterator traits
    // 
    typedef tinystd::__forward_iter iterator_category;
    typedef tinystd::pair<const _Key, _Tp> value_type;
    typedef tinystd::ptrdiff_t difference_type;
    typedef tinystd::pair<const _Key, _Tp> *pointer;
    typedef tinystd::pair<const _Key, _Tp> &reference;

    __node *_M_cur;
    __hashtable *_M_table;
    typename __hashtable::size_type _M_index;

    __hashtable_iterator(__node *__n, __hashtable *__ht) 
        : _M_cur(__n)
        , _M_table(__ht) {}

    __hashtable_iterator() 
        : _M_cur(nullptr) 
        , _M_table(nullptr) {}

    reference operator*() const 
        { return _M_cur->_M_val; }

    pointer operator->() const 
        { return &_M_cur->_M_val; }

    __self &operator++();

    __self operator++(int) {
        __self __tmp = *this; 
        ++*this;
        return __tmp;
    }

    bool operator==(const __self &__it) const 
        { return _M_cur == __it._M_cur; }

    bool operator!=(const __self &__it) const 
        { return _M_cur != __it._M_cur; }
};

const tinystd::size_t __default_buckets_size = 16;

template <typename _Key, typename _Tp, typename _HashFunc = tinystd::hash<_Key>, 
          typename _Equal = tinystd::equal_to<_Key>, typename _Alloc = __DEFAULT_ALLOCATOR>
class unordered_map {
    friend class __hashtable_iterator<_Key, _Tp, _HashFunc, _Equal, _Alloc>;
public:
    typedef _HashFunc hasher;
    typedef _Equal key_equal;
    typedef tinystd::pair<_Key, _Tp> value_type;
    typedef _Tp mapped_type;
    typedef _Key key_typed;

    typedef size_t size_type;
private:
    hasher _M_hash;
    key_equal _M_equals;

    typedef __hashtable_node<tinystd::pair<_Key, _Tp>> __node;
    typedef simple_alloc<__node, _Alloc> __node_allocator;

    tinystd::vector<__node *, _Alloc> _M_buckets;
    size_type _M_num_elements;

public:
    unordered_map()
        : _M_hash(hasher())
        , _M_equals(key_equal())
        , _M_buckets(__default_buckets_size)
        , _M_num_elements(0)
    {}
    unordered_map(size_type __bucket_count)
        : _M_hash(hasher())
        , _M_equals(key_equal())
        , _M_buckets(__bucket_count)
        , _M_num_elements(0)
    {}
    template <typename _InputIter>
    unordered_map(_InputIter __first, _InputIter __last, 
                  tinystd::size_t __bucket_count = __default_buckets_size, 
                  const hasher &__hasher = hasher(), 
                  const key_equal &__key_eq = key_equal())
        : _M_hash(__hasher)
        , _M_equals(__key_eq)
        , _M_buckets(__bucket_count)
        , _M_num_elements(0)
    {    
        _M_copy_initialize(__first, __last);
    }

    size_type bucket_count() const
        { return _M_buckets.size(); }

private:
    __node *_M_new_node(const value_type &__obj) {
        // 
        // TODO: add try and catch here 
        // 
        __node *__n = __node_allocator::allocate();
        __n->_M_next = nullptr;
        tinystd::construct(&__n->_M_val, __obj);
        return __n;
    }

    void _M_delete_node(__node *__n) {
        tinystd::destory(__n->_M_val);
        __node_allocator::deallocate(__n);
    }

    template <typename _InputIter>
    void _M_copy_initialize(_InputIter __first, _InputIter __last) {

    }

};

template <typename _Key, typename _Tp, typename _HashFunc, typename _Equal, typename _Alloc> 
typename __hashtable_iterator<_Key, _Tp, _HashFunc, _Equal, _Alloc>::__self
&__hashtable_iterator<_Key, _Tp, _HashFunc, _Equal, _Alloc>::operator++() {
    _M_cur = _M_cur->_M_next;
    if (!_M_cur) {
        ++_M_index;
        if (_M_index != _M_table->_M_buckets.size()) 
            { _M_cur = _M_table->_M_buckets[_M_index]; }
    }
}




} // tinystd

#endif // _TINYSTL_HASHTABLE_
