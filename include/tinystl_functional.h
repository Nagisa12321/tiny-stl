#ifndef __TINYSTL_FUNCTIONAL_H
#define __TINYSTL_FUNCTIONAL_H

namespace tinystd {

template <typename _Tp>
struct less {
    bool operator()(const _Tp &__lhs, const _Tp &__rhs) { return __lhs < __rhs; }
};

template <typename _Tp>
struct greater {
    bool operator()(const _Tp &__lhs, const _Tp &__rhs) { return __lhs > __rhs; }
};


}

#endif // __TINYSTL_FUNCTIONAL_H
