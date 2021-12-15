#ifndef __TINYSTL_LIST_H
#define __TINYSTL_LIST_H
#include "tinystl_algo.h"
#include "tinystl_constructor.h"
#include "tinystl_iterator_base.h"
#include "tinystl_types.h"
#include "tinystl_alloc.h"

// use c++ init list. 
#include <cstddef>
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
    typedef __list_iterator<_Tp, const_reference, const_pointer> const_iterator;
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
    list(std::initializer_list<_Tp> __li) : list() { _M_copy_init(__li.begin(), __li.end()); }
    ~list() { clear(); _M_destory_node(_M_node); }

    list &operator=(const list &__li) { clear(); _M_copy_init(__li.begin(), __li.end()); return *this; }
    list &operator=(std::initializer_list<_Tp> __li) { clear(); _M_copy_init(__li.begin(), __li.end()); return *this; }

    iterator begin() { return iterator(_M_node->_M_next); }
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    const_iterator end() const { return const_iterator(_M_node); }
    reference front() { return _M_node->_M_next->_M_data; }
    reference back() { return _M_node->_M_prev->_M_data; }
    size_type size() const { return tinystd::distance(begin(), end()); }
    size_type empty() const { return _M_node->_M_next == _M_node; }
    void push_back(const _Tp &__data) { insert(_M_node, __data); }
    void push_front(const _Tp &__data) { insert(_M_node->_M_next, __data); }
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
        __list_node *__lhs = __pos._M_node->_M_prev;
        __list_node *__rhs = __pos._M_node;
        __new_node->_M_prev = __lhs;
        __new_node->_M_next = __rhs;
        __lhs->_M_next = __new_node;
        __rhs->_M_prev = __new_node;
        return iterator(__new_node);
    }
    template <typename _InputIter>
    iterator insert(iterator __pos, _InputIter __begin, _InputIter __end) {
        if (__begin == __end) return __pos; 
        iterator __ret = insert(__pos, *__begin);
        while (++__begin != __end) insert(__pos, *__begin);
        return __ret;
    }
    iterator insert(iterator __pos, size_t __cnt, const _Tp &__val) {
        if (!__cnt) return __pos; 
        iterator __ret = insert(__pos, __val);
        while (--__cnt) insert(__pos, __val);
        return __ret;
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
    iterator erase(iterator __first, iterator __last) {
        if (__first == __last) return __last;
        __first._M_node->_M_prev->_M_next = __last._M_node;
        __last._M_node->_M_prev = __first._M_node->_M_prev;
        while (__first != __last) {
            __list_node *__tmp = __first++._M_node;
            _M_destory_node(__tmp);
        }
        return __last;
    }
    /**
     * @brief Erases all elements from the container. After this call, size() returns zero.
     * Invalidates any references, pointers, or iterators referring to contained elements. 
     * Any past-the-end iterator remains valid.
     */
    void clear() {
        while (!empty()) { erase(begin()); }
    }
    /**
     * @brief Removes all elements satisfying specific criteria. 
     * The first version removes all elements that are equal to value, 
     * the second version removes all elements for which predicate p returns true.
     * @param __val value of the elements to remove
     */
    void remove(const _Tp &__val) {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __tmp = __first++;
            if (*__tmp == __val) erase(__tmp);
        }
    }
    template <typename _UnaryPredicate>
    void remove_if(_UnaryPredicate __condition) {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __tmp = __first++;
            if (__condition(*__tmp)) erase(__tmp);
        }
    }
    /**
     * @brief Removes all consecutive duplicate elements from the container. 
     * Only the first element in each group of equal elements is left.
     *  The behavior is undefined if the selected
     * comparator does not establish an equivalence relation.
     * 
     */
    void unique() {
        iterator __first = begin();
        iterator __last = end();
        iterator __next = begin();
        ++__next;
        while (__next != __last) {
            if (*__first == *__next) {
                __next = erase(__next);
            } else {
                ++__first;
                ++__next;
            }
        }
    }
    template <typename _BinaryPredicate>
    void unique(_BinaryPredicate __condition) {
        iterator __first = begin();
        iterator __last = end();
        iterator __next = begin();
        ++__next;
        while (__next != __last) {
            if (__condition(*__first, *__next)) {
                __next = erase(__next);
            } else {
                ++__first;
                ++__next;
            }
        }
    }
    /**
     * @brief Transfers all elements from other into *this. 
     * The elements are inserted before the element pointed to by pos.
     * The container other becomes empty after the operation. 
     * The behavior is undefined if other refers to the same object as *this.
     */
    void splice(iterator __pos, list &__x) {
        if (!__x.empty()) {
            _M_transfer(__pos, __x.begin(), __x.end());
        }
    }
    /**
     * @brief 
     * Transfers the element pointed to by it from other into *this. 
     * The element is inserted before the element pointed to by pos.
     */
    void splice(iterator __pos, list &, iterator __it) {
        iterator __tmp = __it;
        ++__tmp;
        if (__pos == __it || __pos == __tmp) return;
        _M_transfer(__pos, __it, __tmp);
    }
    /**
     * @brief  Transfers the elements in the range [first, last) from other into *this. 
     * The elements are inserted before the element pointed to by pos. 
     * The behavior is undefined if pos is an iterator in the range [first,last).
     */
    void splice(iterator __pos, list &, iterator __begin, iterator __end) {
        if (__begin != __end)
            _M_transfer(__pos, __begin, __end);
    }

    /**
     * @brief Merges two sorted lists into one. 
     * The lists should be sorted into ascending order.
     * No elements are copied. The container other 
     becomes empty after the operation. 
     */
    void merge(list &__x) {
        iterator __first1 = begin();
        iterator __first2 = __x.begin();
        iterator __end1 = end();
        iterator __end2 = __x.end();
        while (__first1 != __end1 && __first2 != __end2) {
            if (*__first1 < *__first2) {
                ++__first1;
            } else {
                iterator __next = __first2;
                _M_transfer(__first1, __first2, ++__next);
                __first2 = __next;
            }
        }
        if (__first2 != __end2) {
            _M_transfer(__end1, __first2, __end2);
        }
    }
    template <typename _Compare> 
    void merge(list &__x, _Compare __comp) {
        iterator __first1 = begin();
        iterator __first2 = __x.begin();
        iterator __end1 = end();
        iterator __end2 = __x.end();
        while (__first1 != __end1 && __first2 != __end2) {
            if (__comp(*__first1, *__first2)) {
                ++__first1;
            } else {
                iterator __next = __first2;
                _M_transfer(__first1, __first2, ++__next);
                __first2 = __next;
            }
        }
        if (__first2 != __end2) {
            _M_transfer(__end1, __first2, __end2);
        }
    }
    /**
     * @brief Reverses the order of the elements in the container. 
     * No references or iterators become invalidated.
     * 
     */
    void reverse() {
        if (empty() || _M_node->_M_next->_M_next == _M_node)
            return;
        iterator __first = begin();
        iterator __last = end();
        ++__first;
        while (__first != __last) {
            iterator __tmp = __first;
            ++__first;
            _M_transfer(begin(), __tmp, __first);
        }
    }
    /**
     * @brief Sorts the elements in ascending order. 
     * The order of equal elements is preserved. The first version uses operator< to compare the elements, 
     * the second version uses the given comparison function comp.
     */
    void sort() {
        size_t __sz = size();
        if (__sz <= 1) return;
        list __other;
        _M_cut(__other, __sz);  
        this->sort();
        __other.sort();
        merge(__other);
    }
    template <typename _Compare>
    void sort(_Compare __comp) {
        size_t __sz = size();
        if (__sz <= 1) return;
        list __other;
        _M_cut(__other, __sz);  
        this->sort(__comp);
        __other.sort(__comp);
        merge(__other, __comp);
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
// 
// Initialize option
//
    template <typename _Iterator>
    void _M_copy_init(_Iterator __lhs, _Iterator __rhs) {
        while (__lhs != __rhs) { push_back(*__lhs++); }
    }
    /**
     * @brief move the nodes between [__first, __last)
     *  to the place behind __pos.
     */
    void _M_transfer(iterator __pos, iterator __first, iterator __last) {
        if (__pos != __last) {
            __list_node *__back = __last._M_node->_M_prev;
            __first._M_node->_M_prev->_M_next = __last._M_node;
            __last._M_node->_M_prev = __first._M_node->_M_prev;
            __pos._M_node->_M_prev->_M_next = __first._M_node;
            __first._M_node->_M_prev = __pos._M_node->_M_prev;
            __pos._M_node->_M_prev = __back;
            __back->_M_next = __pos._M_node;
        }
    }

    void _M_cut(list &__other, size_t __sz) {
        // is the end of this li and the begin of other li
        iterator __begin = begin();
        iterator __back = end();
        --__back;
        iterator __half = __begin;
        tinystd::advance(__half, __sz >> 1);

        // cut to two list, sort the two lists, and then merge
        _M_transfer(__half, end(), __begin);

        __begin._M_node->_M_prev = _M_node;
        _M_node->_M_next = __begin._M_node;

        // make the other list
        __other._M_node->_M_next = __half._M_node;
        __half._M_node->_M_prev = __other._M_node;
        __other._M_node->_M_prev = __back._M_node;
        __back._M_node->_M_next = __other._M_node;
    }
};
} // namespace tinystd

#endif // __TINYSTL_LIST_H
