#ifndef TINYSTL_CONSTRUCTOR_H
#define TINYSTL_CONSTRUCTOR_H
#include "tinystl_iterator_base.h"
#include "tinystl_move.h"
#include "tinystl_type_traits.h"
namespace tinystd {

template <typename _Tp, typename... _Args> inline void construct(_Tp *__p, _Args &&...__args);
template <typename _Tp> inline void destory(_Tp *__p);
template <typename _ForwardIter> inline void destory(_ForwardIter __lhs, _ForwardIter __rhs);
template <typename _ForwardIter, typename _Tp> inline void __destory(_ForwardIter __lhs, _ForwardIter __rhs, _Tp *);
template <typename _ForwardIter> inline void __destory_aux(_ForwardIter __lhs, _ForwardIter __rhs, __false_type);
template <typename _ForwardIter> inline void __destory_aux(_ForwardIter __lhs, _ForwardIter __rhs, __true_type);

template <typename _Tp, typename... _Args>
inline void construct(_Tp *__p, _Args &&...__args) { new (__p) _Tp(tinystd::forward(__args)...); }

template <typename _Tp>
inline void destory(_Tp *__p) { __p->~_Tp();  }

template <typename _ForwardIter>
inline void destory(_ForwardIter __lhs, _ForwardIter __rhs) {
    __destory(__lhs, __rhs, __value_type(__lhs)); 
}

template <typename _ForwardIter, typename _Tp>
inline void __destory(_ForwardIter __lhs, _ForwardIter __rhs, _Tp *) {
    typedef typename __type_traits<_Tp>::has_trivial_destructor __has_destructor;
    __destory_aux(__lhs, __rhs, __has_destructor());
}

template <typename _ForwardIter>
inline void __destory_aux(_ForwardIter __lhs, _ForwardIter __rhs, __false_type) {
    while (__lhs != __rhs)
        destory(&(*(__lhs++)));   /* crazy option! */
}

template <typename _ForwardIter>
inline void __destory_aux(_ForwardIter __lhs, _ForwardIter __rhs, __true_type) {}

}

#endif // TINYSTL_CONSTRUCTOR_H
