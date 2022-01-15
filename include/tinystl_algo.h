#ifndef __TINYSTL_ALGO_H
#define __TINYSTL_ALGO_H

#include "tinystl_iterator_base.h"
namespace tinystd {

template <typename _InputIter, typename _Tp>
_InputIter find(_InputIter __lhs, _InputIter __rhs, const _Tp &__val) {
    while (__lhs != __rhs && *__lhs != __val) ++__lhs;
    return __lhs;
}

template <typename _InputIter, typename _UnaryPredicate>
_InputIter find_if(_InputIter __lhs, _InputIter __rhs, _UnaryPredicate __conn) {
    while (__lhs != __rhs && !__conn(*__lhs)) ++__lhs;
    return __lhs;
}

template <typename _InputIter, typename _Function>
void for_each(_InputIter __lhs, _InputIter __rhs, _Function __func) {
    while (__lhs != __rhs)
        __func(*__lhs++);
}

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

} // namespace tinystd

#endif // __TINYSTL_ALGO_H
