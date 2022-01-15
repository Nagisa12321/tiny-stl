#ifndef TINYSTL_TREE_H
#define TINYSTL_TREE_H
#include "tinystl_iterator_base.h"

namespace tinystd {

struct __avl_tree_node_base {
    typedef __avl_tree_node_base *__base_ptr;
    __base_ptr _M_left;
    __base_ptr _M_right;
    __base_ptr _M_parent;

    static __base_ptr _M_minimum(__base_ptr __node) {
        while (__node->_M_left) 
            { __node = __node->_M_left; }
        return __node;
    }

    static __base_ptr _M_maximum(__base_ptr __node) {
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

}

#endif // TINYSTL_TREE_H
