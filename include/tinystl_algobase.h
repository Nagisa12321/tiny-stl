#ifndef TINYSTL_ALGOBASE_H
#define TINYSTL_ALGOBASE_H
#include "tinystl_functional.h"
#include "tinystl_iterator_base.h"
#include "tinystl_move.h"
#include "tinystl_pair.h"

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

template <typename _ForwardIter1, typename _ForwardIter2>
void iter_swap(_ForwardIter1 __lhs, _ForwardIter2 __rhs) {
    typename __iterator_traits<_ForwardIter1>::value_type __tmp = *__lhs;
    *__lhs = *__rhs;
    *__rhs = __tmp;
}

template <class _InputIter1, class _InputIter2, typename _Compare>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _Compare __cmp) {
    for (; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) {
        if (__cmp(*__first1, *__first2)) return true;
        if (__cmp(*__first2, *__first1)) return false;
    } 
    return (__first1 != __last1) && (__first2 != __last2);
}

template <class _InputIt1, class _InputIt2>
bool lexicographical_compare(_InputIt1 __first1, _InputIt1 __last1,
                             _InputIt2 __first2, _InputIt2 __last2) {
    return tinystd::lexicographical_compare(__first1, __last1, __first2, __last2, 
        tinystd::less<typename __iterator_traits<_InputIt1>::value_type>());
}

template <typename _Tp, typename _Compare>
inline const _Tp &max(const _Tp &__lhs, const _Tp &__rhs, _Compare __cmp) {
    return __cmp(__lhs, __rhs) ? __rhs : __lhs;
}

template <typename _Tp, typename _Compare>
inline const _Tp &min(const _Tp &__lhs, const _Tp &__rhs, _Compare __cmp) {
    return __cmp(__lhs, __rhs) ? __lhs : __rhs;
}

template <typename _Tp>
inline const _Tp &max(const _Tp &__lhs, const _Tp &__rhs) {
    return __lhs < __rhs ? __rhs : __lhs;
}

template <typename _Tp>
_Tp min(const _Tp &__lhs, const _Tp &__rhs) {
    return __lhs < __rhs ? __lhs : __rhs;
}

template <typename _InputIter1, typename _InputIter2, typename _BinaryPredicate> 
tinystd::pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1, _InputIter1 __last1,
                                                 _InputIter2 __first2, _BinaryPredicate __comm) 
{
    while (__first1 != __last1 && __comm(*__first1, *__first2)) 
        { ++__first1; ++__first2; }
    return { __first1, __first2 };
}

template <typename _InputIter1, typename _InputIter2> 
tinystd::pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1, _InputIter1 __last1,
                                                 _InputIter2 __first2) 
    { return tinystd::mismatch(__first1, __last1, __first2, 
        tinystd::equal_to<typename __iterator_traits<_InputIter1>::value_type>()); }


}

#endif // TINYSTL_ALGOBASE_H
