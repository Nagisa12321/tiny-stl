#ifndef __TINYSTL_HEAP_H
#define __TINYSTL_HEAP_H
#include "tinystl_iterator_base.h"
#include "tinystl_functional.h"

namespace tinystd {

template <typename _RandomAccessIter, typename _BinaryPredicate>
inline void __heapify_down(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __pos, _BinaryPredicate __comm) {
    if (__last <= __pos) return;

    _RandomAccessIter __c1 = __first + 2 * (__pos - __first) + 1;
    _RandomAccessIter __c2 = __first + 2 * (__pos - __first) + 2;
    _RandomAccessIter __max_iter = __pos;

    if (__c1 < __last && __comm(*__max_iter, *__c1)) __max_iter = __c1;
    if (__c2 < __last && __comm(*__max_iter, *__c2)) __max_iter = __c2;

    if (__max_iter != __pos) {
        // TODO: iterator swap? 
        auto __tmp = *__max_iter;
        *__max_iter = *__pos;
        *__pos = __tmp;

        __heapify_down(__first, __last, __max_iter, __comm);
    }
}

template <typename _RandomAccessIter, typename _BinaryPredicate>
inline void __heapify_up(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __pos, _BinaryPredicate __comm) {
    if (__pos < __first) return;

    _RandomAccessIter __c1 = __first + 2 * (__pos - __first) + 1;
    _RandomAccessIter __c2 = __first + 2 * (__pos - __first) + 2;
    _RandomAccessIter __max_iter = __pos;

    if (__c1 < __last && __comm(*__max_iter, *__c1)) __max_iter = __c1;
    if (__c2 < __last && __comm(*__max_iter, *__c2)) __max_iter = __c2;

    if (__max_iter != __pos) {
        // TODO: iterator swap? 
        auto __tmp = *__max_iter;
        *__max_iter = *__pos;
        *__pos = __tmp;

        __heapify_up(__first, __last, __first + (__pos - __first - 1) / 2, __comm);
    }
}

//
// Inserts the element at the position last-1 into the max heap defined by 
// the range [first, last-1). The first version of the function uses operator< 
// to compare the elements, the second uses the given comparison function comp.
// 
template <typename _RandomAccessIter, typename _BinaryPredicate>
inline void push_heap(_RandomAccessIter __first, _RandomAccessIter __last, _BinaryPredicate __comm)
    { __heapify_up(__first, __last, __first + (__last - __first - 2) / 2, __comm); }

template <typename _RandomAccessIter>
inline void push_heap(_RandomAccessIter __first, _RandomAccessIter __last) {
    tinystd::less<typename tinystd::__iterator_traits<_RandomAccessIter>::value_type> __default_comm; 
    push_heap(__first, __last, __default_comm);
}

// Swaps the value in the position first and the value in the position last-1 
// and makes the subrange [first, last-1) into a heap. This has the effect of 
// removing the first element from the heap defined by the range [first, last).
template <typename _RandomAccessIter, typename _BinaryPredicate>
inline void pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _BinaryPredicate __comm) {
    typename tinystd::__iterator_traits<_RandomAccessIter>::value_type __tmp = *__first;
    *__first = *(__last - 1);
    *(__last - 1) = __tmp;
    __heapify_down(__first, __last - 1, __first, __comm);
}

template <typename _RandomAccessIter>
inline void pop_heap(_RandomAccessIter __first, _RandomAccessIter __last) {
    tinystd::less<typename tinystd::__iterator_traits<_RandomAccessIter>::value_type> __default_comm; 
    pop_heap(__first, __last, __default_comm);
}

template <typename _RandomAccessIter, typename _BinaryPredicate>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _BinaryPredicate __comm) {
    _RandomAccessIter __mid = __first + (__last - __first) / 2 - 1;
    do { __heapify_down(__first, __last, __mid--, __comm); }
    while (__mid >= __first);
}

template <typename _RandomAccessIter>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last) {
    tinystd::less<typename tinystd::__iterator_traits<_RandomAccessIter>::value_type> __default_comm; 
    make_heap(__first, __last, __default_comm);
}

template <typename _RandomAccessIter, typename _BinaryPredicate>
inline void sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _BinaryPredicate __comm) {
    while (__first != __last)
        { pop_heap(__first, __last--, __comm); }
}

template <typename _RandomAccessIter>
inline void sort_heap(_RandomAccessIter __first, _RandomAccessIter __last) {
    tinystd::less<typename tinystd::__iterator_traits<_RandomAccessIter>::value_type> __default_comm; 
    sort_heap(__first, __last, __default_comm);
}

}

#endif // __TINYSTL_HEAP_H
