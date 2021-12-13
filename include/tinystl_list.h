#ifndef __TINYSTL_LIST_H
#define __TINYSTL_LIST_H
#include "tinystl_algo.h"
#include "tinystl_constructor.h"
#include "tinystl_iterator_base.h"
#include "tinystl_types.h"
#include "tinystl_alloc.h"

// use c++ init list. 
#include <initializer_list>
namespace tinystd {
template <typename _Tp>
struct _List_node {
    _List_node *_M_prev;
    _List_node *_M_next;
    _Tp _M_data;
};

template <typename _Tp, typename _Ref, typename _Ptr>
struct __list_iterator {
    typedef __list_iterator<_Tp, _Tp &, _Tp *> iterator;
    typedef __list_iterator<_Tp, _Ref, _Ptr> self;
//
// iterator_traits
// 
    typedef __bidirectional_iter iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef _Ptr pointer;
    typedef _Ref reference;

    typedef _List_node<_Tp> *link_type;
    typedef size_t size_type;
//
// A node that point to the node of list.
//
    link_type _M_node;

//
// Constructors
//
    __list_iterator() : _M_node(nullptr) {}
    __list_iterator(link_type __x) : _M_node(__x) {}
    __list_iterator(const __list_iterator &__it) : _M_node(__it._M_node) {}
// 
// Overloaded operators
// 
    bool operator==(const __list_iterator &__it) const { return __it._M_node == _M_node; }
    bool operator!=(const __list_iterator &__it) const { return __it._M_node != _M_node; } 
    reference operator*() const { return _M_node->_M_data; }
    pointer operator->() const { return &_M_node->_M_data; }
    self &operator++() { _M_node = _M_node->_M_next; return *this; }
    self operator++(int) { self __tmp = *this; ++(*this); return __tmp; }
    self &operator--() { _M_node = _M_node->_M_prev; return *this; }
    self operator--(int) { self __tmp = *this; --(*this); return __tmp; }
};

template <typename _Tp, typename _Alloc = __DEFAULT_ALLOCATOR>
class list {
public:
    typedef _Tp value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef __list_iterator<_Tp, reference, pointer> iterator;
    typedef const __list_iterator<_Tp, reference, pointer> const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t different_type;

//
// Constructors
//
    list() : _M_node(_M_get_node()) {
        // point to myself
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    list(std::initializer_list<_Tp> __li) : list() {
        for (const _Tp &__val : __li) push_back(__val);
    }
    ~list() {

    }

    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    reference front() { return _M_node->_M_next->_M_data; }
    reference back() { return _M_node->_M_prev->_M_data; }
    size_type size() const { return tinystd::distance(begin(), end()); }
    size_type empty() const { return _M_node->_M_next == _M_node; }
    void push_back(const _Tp &__data) { insert(_M_node->_M_prev, __data); }
    void push_front(const _Tp &__data) { insert(_M_node, __data); }
    void pop_back() { iterator __tmp = end(); erase(--__tmp); }
    void pop_front() { erase(begin()); }
    
    /**
     * @brief Inserts elements at the specified location in the container.
     * 
     * @param __pos iterator before which the content will be inserted. pos may be the end() iterator 
     * @param __val element value to insert
     * @return  Iterator pointing to the inserted value 
     */
    iterator insert(iterator __pos, const _Tp &__val) {
        __list_node *__new_node = _M_create_node(__val);
        __list_node *__lhs = __pos._M_node;
        __list_node *__rhs = __pos._M_node->_M_next;
        __new_node->_M_prev = __lhs;
        __new_node->_M_next = __rhs;
        __lhs->_M_next = __new_node;
        __rhs->_M_prev = __new_node;
        return iterator(__new_node);
    }

    /**
     * @brief Removes the element at pos.
     * 
     * @param __pos iterator to the element to remove
     * @return Iterator following the last removed element. 
     */
    iterator erase(iterator __pos) {
        if (__pos == end()) return __pos;     // empty list
        __list_node *__delete = __pos._M_node;
        __list_node *__rhs = __delete->_M_next;
        __rhs->_M_prev = __delete->_M_prev;
        __delete->_M_prev->_M_next = __rhs;
        _M_destory_node(__delete);
        return iterator(__rhs);
    }
protected:
    typedef _List_node<_Tp> __list_node;
    typedef simple_alloc<__list_node, _Alloc> __data_allocator;

//
// The head and tail of the list
//
    __list_node *_M_node;
//
// Functions to deal with nodes. 
//
    __list_node *_M_get_node() { return __data_allocator::_S_allocate(); }
    void _M_free_node(__list_node *__node) { __data_allocator::_S_deallocate(__node); }
    __list_node *_M_create_node(const _Tp &__data) {
        __list_node *__new_node = _M_get_node();
        tinystd::construct(&__new_node->_M_data, __data);
        return __new_node;
    }
    void _M_destory_node(__list_node *__node) {
        destory(&__node->_M_data);
        _M_free_node(__node);
    }
};
} // namespace tinystd

#endif // __TINYSTL_LIST_H
