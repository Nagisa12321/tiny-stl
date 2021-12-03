#ifndef __TINYSTL_ALGO_H
#define __TINYSTL_ALGO_H

namespace tinystd {

template <typename _InputIter, typename _Tp>
_InputIter find(_InputIter __lhs, _InputIter __rhs, const _Tp &__val) {
    while (__lhs != __rhs && *__lhs != __val) ++__lhs;
    return __lhs;
}

template <typename _InputIter, typename _Function>
void for_each(_InputIter __lhs, _InputIter __rhs, _Function __func) {
    while (__lhs != __rhs)
        __func(__lhs++);
}

template <typename _ForwardIter, typename _Size, typename _Tp>
_ForwardIter fill_n(_ForwardIter __lhs, _Size __sz, const _Tp &__val) {
    // TODO: fix code here
    for (; __sz > 0; --__sz, ++__lhs) { *__lhs = __val; }
    return __lhs;
}


template <typename _InputIter, typename _ForwardIter>
_ForwardIter copy(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res) {
    // TODO: fix code here
    for (; __lhs != __rhs; ++__lhs, ++__res) { *__res = *__lhs; } 
    return __res;
}

template <typename _ForwardIter, typename _Tp>
void fill(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val) {
    // TODO: fix code here
    for (; __lhs != __rhs; ++__lhs) 
        { *__lhs = __val; }
}

} // namespace tinystd

#endif // __TINYSTL_ALGO_H
