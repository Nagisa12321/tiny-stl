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
} // namespace tinystd

#endif // __TINYSTL_ALGO_H
