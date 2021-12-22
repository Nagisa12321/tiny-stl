#ifndef __TINYSTL_UNINITIALIZED_H
#define __TINYSTL_UNINITIALIZED_H
#include "tinystl_iterator_base.h"
#include "tinystl_algo.h"
#include "tinystl_constructor.h"
#include "tinystl_type_traits.h"
#include <string.h> // for memmove

namespace tinystd {

template <typename _InputIter, typename _ForwardIter>
inline _ForwardIter uninitialized_copy(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res);
template <typename _InputIter, typename _ForwardIter, typename _Tp>
inline _ForwardIter __uninitialized_copy(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res, _Tp *);
template <typename _InputIter, typename _ForwardIter>
inline _ForwardIter __uninitialized_copy_aux(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res, __true_type);
template <typename _InputIter, typename _ForwardIter>
inline _ForwardIter __uninitialized_copy_aux(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res, __false_type);
inline wchar_t* uninitialized_copy(const wchar_t *__lhs, const wchar_t *__rhs, wchar_t *__res);
inline char* uninitialized_copy(const char *__lhs, const char *__rhs, char *__res);
template <typename _ForwardIter, typename _Tp>
inline void uninitialized_fill(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val);
template <typename _ForwardIter, typename _Tp, typename _Tp1>
inline void __uninitialized_fill(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val, _Tp1 *);
template <typename _ForwardIter, typename _Tp>
inline void __uninitialized_fill_aux(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val, __true_type);
template <typename _ForwardIter, typename _Tp>
inline void __uninitialized_fill_aux(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val, __false_type);
template <typename _ForwardIter, typename _Size, typename _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __lhs, _Size __sz, const _Tp &__val);
template <typename _ForwardIter, typename _Size, typename _Tp, typename _Tp1>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __lhs, _Size __sz, const _Tp &__val, _Tp1 *);
template <typename _ForwardIter, typename _Size, typename _Tp>
inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter __lhs, _Size __sz, const _Tp &__val, __true_type);
template <typename _ForwardIter, typename _Size, typename _Tp>
inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter __lhs, _Size __sz, const _Tp &__val, __false_type);

template <typename _InputIter, typename _ForwardIter>
inline _ForwardIter uninitialized_copy(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res) {
    return __uninitialized_copy(__lhs, __rhs, __res, __value_type(__res));  
}

template <typename _InputIter, typename _ForwardIter, typename _Tp>
inline _ForwardIter __uninitialized_copy(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res, _Tp *) {
    typedef typename __type_traits<_Tp>::is_POD_type __is_pod_type;
    return __uninitialized_copy_aux(__lhs, __rhs, __res, __is_pod_type());
}

template <typename _InputIter, typename _ForwardIter>
inline _ForwardIter __uninitialized_copy_aux(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res, __true_type) {
    return tinystd::copy(__lhs, __rhs, __res);
}

template <typename _InputIter, typename _ForwardIter>
inline _ForwardIter __uninitialized_copy_aux(_InputIter __lhs, _InputIter __rhs, _ForwardIter __res, __false_type) {
    while (__lhs != __rhs) { construct(&(*__res++), *__lhs++); }
    return __res;
}

inline wchar_t* uninitialized_copy(const wchar_t *__lhs, const wchar_t *__rhs, wchar_t *__res) {
    memmove(__res, __lhs, (__rhs - __lhs) * sizeof(wchar_t));
    return __res + (__rhs - __lhs);
}

inline char* uninitialized_copy(const char *__lhs, const char *__rhs, char *__res) {
    memmove(__res, __lhs, __rhs - __lhs);
    return __res + (__rhs - __lhs);
}

template <typename _ForwardIter, typename _Tp>
inline void uninitialized_fill(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val) {
    __uninitialized_fill(__lhs, __rhs, __val, __value_type(__lhs)); 
}

template <typename _ForwardIter, typename _Tp, typename _Tp1>
inline void __uninitialized_fill(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val, _Tp1 *) {
    typedef typename __type_traits<_Tp1>::is_POD_type __is_pod_type;
    __uninitialized_fill_aux(__lhs, __rhs, __val, __is_pod_type());
}

template <typename _ForwardIter, typename _Tp>
inline void __uninitialized_fill_aux(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val, __false_type) {
    while (__lhs != __rhs) { construct(&(*__lhs++), __val); }
}

template <typename _ForwardIter, typename _Tp>
inline void __uninitialized_fill_aux(_ForwardIter __lhs, _ForwardIter __rhs, const _Tp &__val, __true_type) {
    tinystd::fill(__lhs, __rhs, __val);
}

template <typename _ForwardIter, typename _Size, typename _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __lhs, _Size __sz, const _Tp &__val) {
    return __uninitialized_fill_n(__lhs, __sz, __val, __value_type(__lhs));
}

template <typename _ForwardIter, typename _Size, typename _Tp, typename _Tp1>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __lhs, _Size __sz, const _Tp &__val, _Tp1 *) {
    typedef typename __type_traits<_Tp1>::is_POD_type __is_pod_type;
    return __uninitialized_fill_n_aux(__lhs, __sz, __val, __is_pod_type());
}

template <typename _ForwardIter, typename _Size, typename _Tp>
inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter __lhs, _Size __sz, const _Tp &__val, __true_type) {
    return tinystd::fill_n(__lhs, __sz, __val);
}

template <typename _ForwardIter, typename _Size, typename _Tp>
inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter __lhs, _Size __sz, const _Tp &__val, __false_type) {
    for (; __sz > 0; --__sz) { construct(&(*__lhs++), __val); }
    return __lhs;
}

}
#endif // __TINYSTL_UNINITIALIZED_H
