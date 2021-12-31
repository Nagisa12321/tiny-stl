#ifndef __TINYSTL_ALGO_H
#define __TINYSTL_ALGO_H

#include "tinystl_iterator_base.h"
namespace tinystd {

template <typename _InputIter, typename _Tp>
_InputIter find(_InputIter __lhs, _InputIter __rhs, const _Tp &__val) {
    while (__lhs != __rhs && *__lhs != __val) ++__lhs;
    return __lhs;
}

template <typename _InputIter, typename _Function>
void for_each(_InputIter __lhs, _InputIter __rhs, _Function __func) {
    while (__lhs != __rhs)
        __func(*__lhs++);
}

template <typename _ForwardIter, typename _Size, typename _Tp>
_ForwardIter fill_n(_ForwardIter __lhs, _Size __sz, const _Tp &__val) {
    // TODO: fix code here
    for (; __sz > 0; --__sz, ++__lhs) { *__lhs = __val; }
    return __lhs;
}


template <typename _InputIter, typename _ForwardIter>
_ForwardIter copy(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res) {
    // TODO: fix code here (chapter 6)
    for (; __lhs != __rhs; ++__lhs, ++__res) { *__res = *__lhs; } 
    return __res;
}

/**
 * @brief Copies the elements from the range, 
 * defined by [first, last), to another range ending at d_last. 
 * The elements are copied in reverse order (the last element is copied first), 
 * but their relative order is preserved.
 */
template <typename _InputIter, typename _ForwardIter>
_ForwardIter copy_backward(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res) {
    // TODO: fix code here
    for (; __lhs != __rhs; ) { *(--__res) = *(--__rhs); }
    return __rhs;
}

template <typename _Tp>
_Tp max(const _Tp &__lhs, const _Tp &__rhs) {
    return __lhs > __rhs ? __lhs : __rhs;
}

template <typename _Tp>
_Tp min(const _Tp &__lhs, const _Tp &__rhs) {
    return __lhs < __rhs ? __lhs : __rhs;
}

template <typename _ForwardIter, typename _Tp>
void fill(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val) {
    // TODO: fix code here
    for (; __lhs != __rhs; ++__lhs) 
        { *__lhs = __val; }
}

/////////////////////
////// advance //////
/////////////////////
template <typename _InputIter, typename _Distance>
void advance(_InputIter &__in, _Distance __d);
template <typename _RandomAccessIter, typename _Distance>
void __advance(_RandomAccessIter &__in, _Distance __d, __random_access_iter);
template <typename _BidirectionalIter, typename _Distance>
void __advance(_BidirectionalIter &__in, _Distance __d, __bidirectional_iter);
template <typename _InputIter, typename _Distance>
void __advance(_InputIter &__in, _Distance __d, __input_iter);

template <typename _InputIter, typename _Distance>
void advance(_InputIter &__in, _Distance __d) {
    // TODO: fix, why g++ will chose the c++ library here?
    __advance(__in, __d, __iterator_category(__in));
}

template <typename _RandomAccessIter, typename _Distance>
void __advance(_RandomAccessIter &__in, _Distance __d, __random_access_iter) {
    __in += __d;
}

template <typename _BidirectionalIter, typename _Distance>
void __advance(_BidirectionalIter &__in, _Distance __d, __bidirectional_iter) {
    if (__d > 0) { while (__d--) ++__in; }
    else { while (__d++) --__in; }
}

template <typename _InputIter, typename _Distance>
void __advance(_InputIter &__in, _Distance __d, __input_iter) {
    while (__d--) ++__in;
}


//////////////////////
////// distance //////
//////////////////////
template <typename _InputIter>
typename __iterator_traits<_InputIter>::difference_type 
    distance(_InputIter __lhs, _InputIter __rhs);
template <typename _InputIter>
typename __iterator_traits<_InputIter>::difference_type 
    __distance(_InputIter __lhs, _InputIter __rhs, __input_iter);
template <typename _RandomAccessIter>
typename __iterator_traits<_RandomAccessIter>::difference_type 
    __distance(_RandomAccessIter __lhs, _RandomAccessIter __rhs, __random_access_iter);

template <typename _InputIter>
typename __iterator_traits<_InputIter>::difference_type 
            distance(_InputIter __lhs, _InputIter __rhs) {
    return __distance(__lhs, __rhs, tinystd::__iterator_category(__lhs));
}

template <typename _InputIter>
typename __iterator_traits<_InputIter>::difference_type 
            __distance(_InputIter __lhs, _InputIter __rhs, __input_iter) {
    typedef typename __iterator_traits<_InputIter>::difference_type __distance;
    __distance __res = 0;
    while (__lhs != __rhs) { ++__res; ++__lhs; }
    return __res;
}

template <typename _RandomAccessIter>
typename __iterator_traits<_RandomAccessIter>::difference_type 
            __distance(_RandomAccessIter __lhs, _RandomAccessIter __rhs, __random_access_iter) {
    return __rhs - __lhs;
}

//
// equal operations .
// TODO: add more situations here
//
template <class _InputIt1, class _InputIt2>
bool equal(_InputIt1 __first1, _InputIt1 __last1, _InputIt2 __first2) {
    while (__first1 != __last1) {
        if (*__first1++ != *__first2++)
            return false;
    }
    return true;
}

template <class _InputIt1, class _InputIt2>
bool lexicographical_compare(_InputIt1 __first1, _InputIt1 __last1,
                             _InputIt2 __first2, _InputIt2 __last2) {
    for (; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) {
        if (*__first1 < *__first2) return true;
        else if (*__first2 < *__first1) return false;
    } 
    return (__first1 != __last1) && (__first2 != __last2);
}

} // namespace tinystd

#endif // __TINYSTL_ALGO_H
