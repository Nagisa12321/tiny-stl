#ifndef TINYSTL_TREE_H
#define TINYSTL_TREE_H
#include "tinystl_constructor.h"
#include "tinystl_iterator_base.h"
#include "tinystl_alloc.h"
#include "tinystl_types.h"
#include "tinystl_pair.h"

namespace tinystd {

struct __avl_tree_node_base {
    typedef __avl_tree_node_base *__base_ptr;
    __base_ptr _M_left;
    __base_ptr _M_right;
    __base_ptr _M_parent;

    static __base_ptr _S_minimum(__base_ptr __node) {
        while (__node->_M_left) 
            { __node = __node->_M_left; }
        return __node;
    }

    static __base_ptr _S_maximum(__base_ptr __node) {
        while (__node->_M_right)
            { __node = __node->_M_right; }
        return __node;
    }
};

template <typename _Tp>
struct __avl_tree_node : public __avl_tree_node_base {
    typedef __avl_tree_node<_Tp> *__ptr;

    _Tp _M_value;
};

struct __avl_tree_base_iterator {
    typedef __avl_tree_node_base::__base_ptr __base_ptr;

    // iterator traits will use this...
    typedef __bidirectional_iter iterator_category;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;

    typedef __avl_tree_base_iterator __self;

    __avl_tree_base_iterator(__base_ptr __node)
        : _M_node(__node) {}

    __base_ptr _M_node;

    void _M_increment() {
        if (_M_node->_M_right) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left) 
                { _M_node = _M_node->_M_left; }
        } else {
            __base_ptr __parent = _M_node->_M_parent;
            while (__parent && _M_node == __parent->_M_right) 
                { _M_node = __parent; __parent = __parent->_M_parent; }
            
            // let the [_M_node == null] means
            // it is the end iterator!
            if (__parent) _M_node = __parent;
            else _M_node = 0;
        }
    }

    void _M_decrement() {
        if (_M_node->_M_left) {
            _M_node = _M_node->_M_left;
            while (_M_node->_M_right)
                { _M_node = _M_node->_M_right; }
        } else {
            __base_ptr __parent = _M_node->_M_parent;
            while (__parent && _M_node == __parent->_M_left) 
                { _M_node = __parent; __parent = __parent->_M_parent; }

            // now [__parent == NULL] is undefined behaviors
            // so I don't want to deal with it...
            _M_node = __parent;
        }
    }
};

template <typename _Tp, typename _Ref, typename _Ptr>
struct __avl_tree_iterator : public __avl_tree_base_iterator {
    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;

    typedef __avl_tree_iterator<_Tp, _Ref, _Ptr> __self;
    typedef typename __avl_tree_node<_Tp>::__ptr __ptr;

    __avl_tree_iterator()
        : __avl_tree_base_iterator(0) {}
    __avl_tree_iterator(__ptr __node)
        : __avl_tree_base_iterator(__node) {}

    reference operator*() const
        { return __ptr(_M_node)->_M_value; }

    pointer operator->() const
        { return &(operator*()); }

    __self &operator++()
        { _M_increment(); return *this; }

    __self operator++(int) {
        __self __tmp = *this;
        _M_increment();
        return __tmp;
    }

    __self &operator--() 
        { _M_decrement(); return *this; }

    __self operator--(int) {
        __self __tmp = *this;
        _M_decrement();
        return __tmp;
    }
};

template <typename _Key, 
          typename _Value, 
          typename _KeyOfValue, 
          typename _Compare, 
          typename _Alloc = __DEFAULT_ALLOCATOR>
class __avl_tree {
protected:
    typedef __avl_tree_node_base::__base_ptr __base_ptr;
    typedef typename __avl_tree_node<_Value>::__ptr __ptr;
    typedef simple_alloc<__avl_tree_node<_Value>, _Alloc> __avl_tree_node_allocator;

public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

protected:
    __ptr _M_get_node() 
        { return __avl_tree_node_allocator::_S_allocate(); }

    void _M_put_node(__ptr __node) 
        { __avl_tree_node_allocator::_S_deallocate(__node); }


    __ptr _M_create_node(const value_type &__val) {
        __ptr __name = _M_get_node();
        // TODO: add try and catch here
        tinystd::construct(&__name._M_value, __val);
        return __name;
    }

    void _M_destory_node(__ptr __node) {
        tinystd::destory(__node);
        _M_put_node(__node);
    }

protected:
    size_type _M_node_count;
    __ptr _M_root;
    _Compare _M_comp;

public:
    typedef __avl_tree_iterator<value_type, reference, pointer> iterator;

    iterator begin() 
        { return iterator((__ptr) __avl_tree_node_base::_S_minimum(_M_root)); }
    iterator end()
        { return iterator((__ptr) __avl_tree_node_base::_S_maximum(_M_root)); }
    bool empty() const 
        { return _M_node_count == 0; }
    size_type size() const
        { return _M_node_count; }
    size_type max_size() const
        { return size_type(-1); }
    
    // insert the val, then return a pair
    // return the iterator insert
    // return end means it contains the value... 
    iterator insert_unique(const value_type &__val) {
        tinystd::pair<iterator, bool> __res = find(__val);
        if (__res.second) 
            { return iterator(0); }
        else {
            iterator __parent = __res.first;
            __ptr __new_node = _M_create_node(__val);
            if (!__parent._M_node) {
                // means that before this insert. 
                // the tree is empty... 
                _M_root = __new_node;
            } else {
                // insert a new node in the right place. 
                if (_M_comp(__val, __parent._M_node->_M_value)) {
                    __parent._M_node->_M_left = __new_node;
                } else {
                    __parent._M_node->_M_right = __new_node;
                }
            }

            ++_M_node_count;
        }
    }

    iterator insert_equal(const value_type &__val) {
        
    }

    // find that the tree is contains __val 
    // if find the val return it's iterator and true
    // but if not find, return the parent iterator and false... 
    tinystd::pair<iterator, bool> find(const value_type &__val) {
        __ptr __cur = _M_root, __parent = 0;
        while (__cur && __cur->_M_value != __val) {
            __parent = __cur;
            if (_M_comp(__cur->_M_value, __val))
                { __cur = __cur->_M_left; }
            else
                { __cur = __cur->_M_right; }
        }
        if (__cur) 
            { return { iterator(__cur), true }; }
        else 
            { return { iterator(__parent), false }; }
    }
};

}

#endif // TINYSTL_TREE_H
