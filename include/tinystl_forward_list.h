#ifndef __TINYSTL_FORWARD_LIST_H
#define __TINYSTL_FORWARD_LIST_H

#include "tinystl_constructor.h"
#include "tinystl_iterator_base.h"
#include "tinystl_types.h"
#include "tinystl_alloc.h"
namespace tinystd {

struct __slist_node_base 
    { __slist_node_base *_M_next; };

template <typename _Tp>
struct __slist_node : public __slist_node_base 
    { _Tp _M_data; };

/**
 * @brief insert __new after __prev, 
 *      and then return __new;
 */
inline __slist_node_base *__slist_make_link(__slist_node_base * __prev,
                                    __slist_node_base *__new) {
    __new->_M_next = __prev->_M_next;
    __prev->_M_next = __new;
    return __new;
}

inline size_t __slist_size(__slist_node_base *__head) {
    size_t __res = 0;
    while (__head) {
        __head = __head->_M_next;
        ++__res;
    }
    return __res;
}

struct __slist_iterator_base {
    typedef __forward_iter iterator_category;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;

    __slist_node_base *_M_node;

    __slist_iterator_base(__slist_node_base *__node) : _M_node(__node) {}

    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const __slist_iterator_base &__it) const { return __it._M_node == _M_node; }
    bool operator!=(const __slist_iterator_base &__it) const { return __it._M_node != _M_node; }
};

template <typename _Tp, typename _Ref, typename _Ptr>
struct __slist_iterator : public __slist_iterator_base {
    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;

    typedef __slist_iterator<_Tp, _Ref, _Ptr> __self;

    __slist_iterator(__slist_node<_Tp> *__node) : __slist_iterator_base(__node) {}
    __slist_iterator() : __slist_iterator_base(0) {}
    __slist_iterator(const __slist_iterator &__it) : __slist_iterator_base(__it._M_node) {}

    reference operator*() const { return ((__slist_node<_Tp>)_M_node)._M_data; }
    pointer operator->() const { return &(operator*()); }

    __self &operator++() { _M_incr(); return *this; }
    __self operator++(int) { __self __res = *this; _M_incr(); return __res; }
};

template <typename _Tp, typename _Alloc = __DEFAULT_ALLOCATOR>
class forward_list {
public:
    typedef _Tp value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef __slist_iterator<value_type, reference, pointer> iterator;
    typedef __slist_iterator<value_type, const_reference, const_pointer> const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t different_type;

    forward_list() { _M_head._M_next = nullptr; }
    ~forward_list() { clear(); }

    iterator begin() { return iterator((__list_node *)_M_head._M_next); }
    iterator end() { return iterator(nullptr); }
    size_t size() const { return __slist_size(_M_head._M_next); }
    bool empty() const { return !_M_head._M_next; }
    reference front() { return ((__list_node *) _M_head._M_next)->_M_data; }
    const_reference front() const { return ((__list_node *) _M_head._M_next)->_M_data; }
    void push_front(const _Tp &__data) { __slist_make_link(&_M_head, _M_create_node(__data)); }
    void pop_front() {
        __list_node *__tmp = (__list_node *) _M_head._M_next;
        _M_head._M_next = __tmp->_M_next;
        _M_destory_node(__tmp);
    }
    void swap(forward_list &__other) {
        __list_node_base *__tmp = __other._M_head._M_next;
        __other._M_head._M_next = _M_head._M_next;
        _M_head._M_next = __tmp;
    }
    
    void clear() {
        while (!empty()) { pop_front(); }
    }
protected:
    typedef __slist_node_base __list_node_base;
    typedef __slist_node<_Tp> __list_node;
    typedef __slist_iterator_base __iterator_base;
    typedef tinystd::simple_alloc<__list_node, _Alloc> __node_allocator;

    __list_node_base _M_head;

    __list_node *_M_create_node(const _Tp &__data) {
        __list_node *__res = __node_allocator::_S_allocate();
        __res->_M_data = __data;
        return __res;
    }

    void _M_destory_node(__list_node *__node) {
        tinystd::destory(&__node->_M_data);
        __node_allocator::_S_deallocate(__node);
    }
};

}

#endif // __TINYSTL_FORWARD_LIST_H
