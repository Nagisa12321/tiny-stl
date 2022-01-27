#ifndef __TINYSTL_FUNCTIONAL_H
#define __TINYSTL_FUNCTIONAL_H

namespace tinystd {

template <typename _Tp>
struct less {
    bool operator()(const _Tp &__lhs, const _Tp &__rhs) const 
        { return __lhs < __rhs; }
};

template <typename _Tp>
struct greater {
    bool operator()(const _Tp &__lhs, const _Tp &__rhs) const 
        { return __lhs > __rhs; }
};

template <typename _Tp>
struct equal_to {
    bool operator()(const _Tp &__lhs, const _Tp &__rhs) const 
        { return __lhs == __rhs; }
};

template <typename _Tp>
struct plus {
    _Tp operator()(const _Tp &__lhs, const _Tp &__rhs) const 
        { return __lhs + __rhs; }
};

template <typename _Tp>
struct minus {
    _Tp operator()(const _Tp &__lhs, const _Tp &__rhs) const 
        { return __lhs - __rhs; }
}; 

template <typename _Tp>
struct multiplies {
    _Tp operator()(const _Tp &__lhs, const _Tp &__rhs) const
        { return __lhs * __rhs; }
};

// return the data itself...
template <typename _Tp>
struct identity {
    const _Tp &operator()(const _Tp &__data) const
        { return __data; }
};

template <typename _Pair>
struct __select1st {
    const typename _Pair::first_type &operator()(const _Pair &__pair) 
        { return __pair.first; }
};

}

#endif // __TINYSTL_FUNCTIONAL_H
