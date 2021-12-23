#ifndef __TINYSTL_FORWARD_LIST_H
#define __TINYSTL_FORWARD_LIST_H

#include "tinystl_constructor.h"
#include "tinystl_iterator_base.h"
#include "tinystl_types.h"
#include "tinystl_alloc.h"

// use std iniializer_list 
#include <initializer_list>
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

    reference operator*() const { return ((__slist_node<_Tp> *)_M_node)->_M_data; }
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
    forward_list(const std::initializer_list<_Tp> &__li) : forward_list() 
        { _M_copy_initialize(__li.begin(), __li.end()); }
    template <typename _InputIter, tinystd::void_t<decltype(*tinystd::__declval<_InputIter>())>* = nullptr>
    forward_list(_InputIter __lhs, _InputIter __rhs) : forward_list() 
        { _M_copy_initialize(__lhs, __rhs); }
    forward_list(size_type __n, const _Tp &__data = _Tp()) : forward_list() 
        { _M_fill_initialize(__n, __data); }
    forward_list(const forward_list<_Tp> &__li) : forward_list() 
        { _M_copy_initialize(__li.begin(), __li.end()); }
    
    ~forward_list() { clear(); }

    forward_list &operator=(const forward_list &__li) {
        clear();
        _M_copy_initialize(__li.begin(), __li.end());
        return *this;
    }

    forward_list &operator=(forward_list &&__li) {
        _M_head._M_next = __li._M_head._M_next;
        __li._M_head._M_next = nullptr;
        return *this;
    }

    iterator before_begin() { return iterator((__list_node *)_M_head); }
    const_iterator before_begin() const { return const_iterator((__list_node *)_M_head); }
    iterator begin() { return iterator((__list_node *)_M_head._M_next); }
    const_iterator begin() const { return const_iterator((__list_node *)_M_head._M_next); }
    iterator end() { return iterator(nullptr); }
    const_iterator end() const { return const_iterator(nullptr); }
    size_type size() const { return __slist_size(_M_head._M_next); }
    bool empty() const { return !_M_head._M_next; }
    size_type max_size() const { return size_type(-1); }
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
        while (!empty()) 
            { pop_front(); }
    }
    void assign(size_type __count, const _Tp &__value) {
        clear();
        _M_fill_initialize(__count, __value);
    }
    template <class _InputIter, tinystd::void_t<decltype(*tinystd::__declval<_InputIter>())>* = nullptr>
    void assign(_InputIter __first, _InputIter __last) {
        clear();
        _M_copy_initialize(__first, __last);
    }
    void assign(std::initializer_list<_Tp> __ilist) {
        clear();
        _M_copy_initialize(__ilist.begin(), __ilist.end());
    }
    /**
     * @brief 
     * 
     * @param __pos 
     * @param __data 
     * @return iterator 
     * 1) Iterator to the inserted element.
     * 2) Iterator to the last element inserted, or pos if count==0.
     * 3) Iterator to the last element inserted, or pos if first==last.
     * 4) Iterator to the last element inserted, or pos if ilist is empty.
     */
    iterator insert_after(iterator __pos, const _Tp &__data) {
        __slist_make_link(__pos._M_node, _M_create_node(__data));
        return ++__pos;
    }
    iterator insert_after(iterator __pos, size_type __n, const _Tp &__data) {
        if (!__n) return __pos;
        __list_node *__cur = (__list_node *) __pos._M_node;
        __list_node *__tmp = (__list_node *) __cur->_M_next;
        for (; __n > 0; --__n) {
            __slist_make_link(__cur, _M_create_node(__data));
            __cur = (__list_node *) __cur->_M_next;
        }
        __cur->_M_next = __tmp;
        return iterator(__cur);
    }
    template <typename _InputIter, tinystd::void_t<decltype(*tinystd::__declval<_InputIter>())>* = nullptr>
    iterator insert_after(iterator __pos, _InputIter __lhs, _InputIter __rhs) {
        if (__lhs == __rhs) return __pos;
        __list_node *__cur = (__list_node *) __pos._M_node;
        __list_node *__tmp = (__list_node *) __cur->_M_next;
        while (__lhs != __rhs) {
            __slist_make_link(__cur, _M_create_node(*__lhs++));
            __cur = (__list_node *) __cur->_M_next;
        }
        __cur->_M_next = __tmp;
        return iterator(__cur);
    }
    iterator insert_after(iterator __pos, const std::initializer_list<_Tp> &__ili) {
        return insert_after(__pos, __ili.begin(), __ili.end());
    }
    /**
     * @brief 
     * 
     * @return iterator 
     * Iterator to the element following the erased one,
     * or end() if no such element exists.
     */
    iterator erase_after(iterator __pos) {
        __list_node *__tmp = (__list_node *) __pos._M_node->_M_next;
        __pos._M_node->_M_next = __pos._M_node->_M_next->_M_next;
        _M_destory_node(__tmp);
    }

    iterator erase_after(iterator __lhs, iterator __rhs) {
        
    }


protected:
    typedef __slist_node_base __list_node_base;
    typedef __slist_node<_Tp> __list_node;
    typedef __slist_iterator_base __iterator_base;
    typedef tinystd::simple_alloc<__list_node, _Alloc> __node_allocator;

    __list_node_base _M_head;

    __list_node *_M_create_node(const _Tp &__data) {
        __list_node *__res = __node_allocator::_S_allocate();
        // !!!Use constuctor here~
        tinystd::construct(&__res->_M_data, __data);
        return __res;
    }

    void _M_destory_node(__list_node *__node) {
        tinystd::destory(&__node->_M_data);
        __node_allocator::_S_deallocate(__node);
    }

    template <typename _InputIter>
    void _M_copy_initialize(_InputIter __lhs, _InputIter __rhs) {
        __list_node *__cur = (__list_node *) (&_M_head);
        while (__lhs != __rhs) {
            __slist_make_link(__cur, _M_create_node(*__lhs++));
            __cur = (__list_node *) __cur->_M_next;
        }
    }

    void _M_fill_initialize(size_type __n, const _Tp &__data) {
        for (; __n > 0; --__n) {
            push_front(__data);
        }
    }
};

}

#endif // __TINYSTL_FORWARD_LIST_H
