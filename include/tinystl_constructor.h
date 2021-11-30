#ifndef TINYSTL_CONSTRUCTOR_H
#define TINYSTL_CONSTRUCTOR_H
#include <stdio.h>          // for debug
#include "type_traits.h"

namespace tinystd {

template <typename _Tp1, typename _Tp2>
inline void construct(_Tp1 *__p, const _Tp2 &__rhs) { new (__p) _Tp1(__rhs); }

template <typename _Tp>
inline void destory(_Tp *__p) { __p->~_Tp(); 
    printf("destory %p\n", __p);
}

template <typename _ForwardIter>
inline void destory(_ForwardIter __lhs, _ForwardIter __rhs) {
    __destory(__lhs, __rhs, &(*__lhs));
}

template <typename _ForwardIter, typename _Tp>
inline void __destory(_ForwardIter __lhs, _ForwardIter __rhs, _Tp *) {
    typedef typename __type_traits<_Tp>::has_trivial_destructor __has_destructor;
    __destory_aux(__lhs, __rhs, __has_destructor());
}

template <typename _ForwardIter>
inline void __destory_aux(_ForwardIter __lhs, _ForwardIter __rhs, __false_type) {
    printf("do not have trivial_destructor, so will call it\n");
    while (__lhs != __rhs)
        destory(&*(__lhs++));   /* crazy option! */
}

template <typename _ForwardIter>
inline void __destory_aux(_ForwardIter __lhs, _ForwardIter __rhs, __true_type) {}

}

#endif // TINYSTL_CONSTRUCTOR_H
