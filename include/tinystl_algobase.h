#ifndef TINYSTL_ALGOBASE_H
#define TINYSTL_ALGOBASE_H
#include "tinystl_functional.h"
#include "tinystl_iterator_base.h"

namespace tinystd {

template <typename _InputIter1, typename _InputIter2, typename _BinaryPredicate>
bool equal(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _BinaryPredicate __comm) {
    while (__first1 != __last1) {
        if (!__comm(*__first1++, *__first2++))
            return false;
    }
    return true;
}

template <class _InputIter1, class _InputIter2>
bool equal(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2) 
    { return tinystd::equal(__first1, __last1, __first2, 
        tinystd::equal_to<typename __iterator_traits<_InputIter1>::value_type>()); }

template <typename _ForwardIter, typename _Size, typename _Tp>
_ForwardIter fill_n(_ForwardIter __lhs, _Size __sz, const _Tp &__val) {
    for (; __sz > 0; --__sz, ++__lhs) 
        { *__lhs = __val; }
    return __lhs;
}

template <typename _ForwardIter, typename _Tp>
void fill(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val) {
    for (; __lhs != __rhs; ++__lhs) 
        { *__lhs = __val; }
}

}

#endif // TINYSTL_ALGOBASE_H
