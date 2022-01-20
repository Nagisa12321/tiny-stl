#ifndef __TINYSTL_TREE
#define __TINYSTL_TREE
#include "tinystl_algobase.h"
#include "tinystl_constructor.h"
#include "tinystl_iterator_base.h"
#include "tinystl_alloc.h"
#include "tinystl_type_traits.h"
#include "tinystl_types.h"
#include "tinystl_pair.h"
#include "tinystl_vector.h"

// for memory set. 
#include <string.h>

#ifdef __test_avl
    #include <stdio.h>
    #include <assert.h>
    #include "tinystl_queue.h"
#endif

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

// 
// Can only have the "value"
// but I can get the "key" from the value!
//
template <typename _Tp>
struct __avl_tree_node : public __avl_tree_node_base {
    typedef __avl_tree_node<_Tp> *__ptr;

    int _M_height;
    _Tp _M_value;

    void _M_height_flush() {
        _M_height = tinystd::max(
            _M_left ? ((__ptr) _M_left)->_M_height : 0x0, 
            _M_right ? ((__ptr) _M_right)->_M_height : 0x0
        ) + 1;
    }
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

    bool operator==(const __self &__other) const 
        { return _M_node == __other._M_node; }

    bool operator!=(const __self &__other) const 
        { return !(*this == __other); }
         
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
    typedef __avl_tree_iterator<value_type, reference, pointer> iterator;

protected:
    __ptr _M_get_node() 
        { return __avl_tree_node_allocator::_S_allocate(); }

    void _M_put_node(__ptr __node) 
        { __avl_tree_node_allocator::_S_deallocate(__node); }


    __ptr _M_create_node(const value_type &__val) {
        __ptr __node = _M_get_node();

        // set the memory to zero
        memset(__node, 0x0, sizeof(__avl_tree_node<_Value>));

        // TODO: add try and catch here
        tinystd::construct(&(__node->_M_value), __val);
        return __node;
    }

    void _M_destory_node(__ptr __node) {
        tinystd::destory(__node);
        _M_put_node(__node);
    }

    tinystd::pair<iterator, bool> _M_find(__ptr __cur, const value_type &__val) {
        __ptr __parent = 0x0;
        if (__cur)
            __parent = (__ptr) __cur->_M_parent;
        while (__cur && _S_key(__cur->_M_value) != _S_key(__val)) {
            __parent = __cur;
            if (_M_key_comp(_S_key(__val), _S_key(__cur->_M_value)))
                { __cur = (__ptr) __cur->_M_left; }
            else
                { __cur = (__ptr) __cur->_M_right; }
        }
        if (__cur) 
            { return { iterator(__cur), true }; }
        else 
            { return { iterator(__parent), false }; }
    }

    iterator _M_insert_node(__ptr __new_node, iterator __parent) {
        const value_type __val = __new_node->_M_value;
        if (!__parent._M_node) {
           // means that before this insert. 
           // the tree is empty... 
           _M_root = __new_node;
           _M_root->_M_height = 1;
       } else {
           // insert a new node in the right place. 
           if (_M_key_comp(_S_key(__val), 
               _S_key(((__ptr) __parent._M_node)->_M_value))) {
               __parent._M_node->_M_left = __new_node;
           } else {
               __parent._M_node->_M_right = __new_node;
           }
           // set the parent of the new node...
           __new_node->_M_parent = __parent._M_node;

           // update the height from __new_node to root; 
           __ptr __cur = __new_node;
           while (1) {
               __cur->_M_height_flush();
               if (__cur == _M_root) break;
               __cur = (__ptr) __cur->_M_parent;
           } 
       }

       ++_M_node_count;

#ifdef __test_avl
        printf(">>>>>>>>>>> insert %d\n", __val);
#endif 
        // rotate
        _M_rotate(__new_node);

#ifdef __test_avl
        _S_show_tree(_M_root);
        assert(_S_is_balanced(_M_root));
#endif 

        return iterator(__new_node);
    }

    void _M_rotate(__ptr __node) {
        if (!__node) return; 
        __ptr __parent = (__ptr) __node->_M_parent;
        __ptr __pivot = 0;
        // the four cases. 
        // (1) LL 
        if (_M_bf(__node) > 1 && _M_bf((__ptr) __node->_M_left) >= 0) { 
            __pivot = _M_rorate_right(__node); 
        // (2) RR
        } else if (_M_bf(__node) < -1 && _M_bf((__ptr) __node->_M_right) <= 0) {
            __pivot = _M_rorate_left(__node);
        // (3) LR
        } else if (_M_bf(__node) > 1 && _M_bf((__ptr) __node->_M_left) < 0) {
            __node->_M_left = _M_rorate_left((__ptr) __node->_M_left);
            __pivot = _M_rorate_right(__node);
        // (4) RL
        } else if (_M_bf(__node) < -1 && _M_bf((__ptr) __node->_M_right) > 0) {
            __node->_M_right = _M_rorate_right((__ptr) __node->_M_right);
            __pivot = _M_rorate_left(__node);
        }
    
        // replace the __node with __pivot
        if (__pivot) {
            if (__parent) {
                // update the height of parent... 
                __ptr __cur = __parent;
                while (__cur) {
                    __cur->_M_height_flush();
                    __cur = (__ptr) __cur->_M_parent;
                }
            } else {
                _M_root = __pivot;
            }
        }

        // rorate up...
        _M_rotate(__parent);
    }

    // return the "Pivot"
    __ptr _M_rorate_right(__ptr __node) {
        if (!__node) return 0x0;
        __ptr __left = (__ptr) __node->_M_left;
        __ptr __parent = (__ptr) __node->_M_parent; 
        
        if (__left) {
            __node->_M_left = __left->_M_right;
            if (__left->_M_right)
                __left->_M_right->_M_parent = __node;
            __left->_M_right = __node;
            __left->_M_parent = __parent;
        }

        __node->_M_parent = __left;

        if (__parent) {
            if (__node == __parent->_M_left) __parent->_M_left = __left;
            else __parent->_M_right = __left;
        }

        __node->_M_height_flush();
        if (__left)
            __left->_M_height_flush();

        return __left;
    }

    __ptr _M_rorate_left(__ptr __node) {
        if (!__node) return 0x0;
        __ptr __right = (__ptr) __node->_M_right;
        __ptr __parent = (__ptr) __node->_M_parent;

        if (__right) {
            __node->_M_right = __right->_M_left; 
            if (__right->_M_left)
                __right->_M_left->_M_parent = __node;
            __right->_M_left = __node;
            __right->_M_parent = __parent;
        }

        __node->_M_parent = __right;
        if (__parent) {
            if (__node == __parent->_M_left) __parent->_M_left = __right;
            else __parent->_M_right = __right;
        }

        __node->_M_height_flush();
        if (__right)
            __right->_M_height_flush();

        return __right;
    }

    int _M_bf(__ptr __node)
        { return __node ? ((__node->_M_left ? ((__ptr) __node->_M_left)->_M_height : 0) -
                 (__node->_M_right ? ((__ptr) __node->_M_right)->_M_height : 0)) : 0; }

protected:
    size_type _M_node_count;
    __ptr _M_root;
    _Compare _M_key_comp;

public:

    __avl_tree()
        : _M_node_count(0x0)
        , _M_root(0x0)
        , _M_key_comp() {}
    ~__avl_tree() 
        { clear(); }

    iterator begin() 
        { return iterator((__ptr) __avl_tree_node_base::_S_minimum(_M_root)); }
    iterator end()
        { return iterator(0); }
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
            return _M_insert_node(__new_node, __parent);
        }
    }

    iterator insert_equal(const value_type &__val) {
        __ptr __cur = _M_root;
        __ptr __parent = 0x0;
        while (__cur) {
            __parent = __cur;
            if (_M_key_comp(_S_key(__val), _S_key(__cur->_M_value)))
                { __cur = (__ptr) __cur->_M_left; }
            else
                { __cur = (__ptr) __cur->_M_right; }
        }
        __ptr __new_node = _M_create_node(__val);
        return _M_insert_node(__new_node, __parent);
    }

    // find that the tree is contains __val 
    // if find the val return it's iterator and true
    // but if not find, return the parent iterator and false... 
    tinystd::pair<iterator, bool> find(const value_type &__val) 
        { return _M_find(_M_root, __val); }

    void clear() { 
        _S_walk_tree(_M_root, [&](__ptr __node) { 
            _M_destory_node(__node); 
        });
    }

protected:
    static _Key _S_key(const value_type &__data) 
        { return _KeyOfValue()(__data); }
    
    template <typename _Function> 
    static void _S_walk_tree(const __ptr __node, const _Function &__fn) {
        if (!__node) return;

        _S_walk_tree((__ptr) __node->_M_left, __fn);
        _S_walk_tree((__ptr) __node->_M_right, __fn);
        __fn(__node);
    }

#ifdef __test_avl
    static int _S_height(__ptr __node) {
        if (__node == NULL) {
            return 0;
        } else {
            return tinystd::max(_S_height((__ptr) __node->_M_left), 
                _S_height((__ptr) __node->_M_right)) + 1;
        }
    }

    static bool _S_is_balanced(__ptr __node) {
        if (__node == NULL) {
            return true;
        } else {
            return tinystd::abs(_S_height((__ptr) __node->_M_left) - _S_height((__ptr) __node->_M_right)) <= 1 
                && _S_is_balanced((__ptr) __node->_M_left) && _S_is_balanced((__ptr) __node->_M_right);
        }
    }

    static void _S_show_tree(__ptr __node) {
        tinystd::queue<__ptr> __q;
        __q.push(__node);

        while (!__q.empty()) {
            size_t __sz = __q.size();

            for (int i = 0; i < __sz; ++i) {
                __ptr poll = __q.front();
                __q.pop();

                if (poll) {
                    printf("%d(%d), ", poll->_M_value, poll->_M_height);

                    __q.push((__ptr) poll->_M_left);
                    __q.push((__ptr) poll->_M_right);
                } else {
                    printf("NULL(0), ");
                }
            }
            
            printf("\n");
        }
    }
#endif

};

}

#endif // __TINYSTL_TREE_H
