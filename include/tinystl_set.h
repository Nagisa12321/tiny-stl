#ifndef __TINYSTL_SET
#define __TINYSTL_SET
    
#include "tinystl_functional.h"
#include "tinystl_alloc.h"
#include "tinystl_tree.h"

// for the initializer_list constructor...
#include <initializer_list>

namespace tinystd {

template <typename _Key, 
          typename _Compare = tinystd::less<_Key>, 
          typename _Alloc = __DEFAULT_ALLOCATOR>
class set {
public:
    typedef _Key key_type;
    typedef _Key value_type;
    typedef _Compare key_compare;
    typedef _Compare value_compare;

private:
    typedef __avl_tree<_Key, _Key, tinystd::identity<_Key>, _Compare> __rep_type;

    __rep_type _M_tree;

public:
    typedef typename __rep_type::const_pointer const_pointer;
    typedef typename __rep_type::const_pointer pointer;
    typedef typename __rep_type::const_reference reference;
    typedef typename __rep_type::const_reference const_reference;
    typedef typename __rep_type::const_iterator iterator;
    typedef typename __rep_type::size_type size_type;
    typedef typename __rep_type::difference_type difference_type;

    set() 
        : _M_tree(_Compare()) {}
    set(const _Compare &__comp) 
        : _M_tree(__comp) {}
    template <typename _InputIter>
    set(_InputIter __first, _InputIter __last)
        : _M_tree(_Compare()) {
        _M_tree.insert_unique(__first, __last);
    }
    template <typename _InputIter>
    set(_InputIter __first, _InputIter __last, const _Compare &__comp)
        : _M_tree(__comp) {
        _M_tree.insert_unique(__first, __last);
    }
    set(const set &__other)
        : _M_tree(__other._M_tree) {}
    set(std::initializer_list<_Key> __li)
        : _M_tree() {
        _M_tree.insert_unique(__li.begin(), __li.end());
    }

    set &operator=(const set &__other) 
        { _M_tree = __other._M_tree; return *this; }

    iterator begin() const
        { return _M_tree.begin(); }
    iterator end() const
        { return _M_tree.end(); }
    iterator cbegin() const
        { return _M_tree.begin(); }
    iterator cend() const
        { return _M_tree.end(); }
    size_type size() const
        { return _M_tree.size(); }
    bool empty() const
        { return _M_tree.empty(); }
    size_type max_size() const
        { return size_type(-1); }
    void clear() 
        { _M_tree.clear(); }

    iterator insert(const key_type &__key) 
        { return iterator((__avl_tree_node<_Key> *) _M_tree.insert_unique(__key)._M_node); }
    iterator find(const key_type &__key)
        { return iterator((__avl_tree_node<_Key> *) _M_tree.find(__key).first._M_node); }
    
};

}

#endif // __TINYSTL_SET
