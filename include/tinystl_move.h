#ifndef __TINYSTL_MOVE_H
#define __TINYSTL_MOVE_H
#include "tinystl_type_traits.h"

namespace tinystd {

template <typename _Tp>
constexpr typename tinystd::remove_reference<_Tp>::type &&
move(_Tp &&__t) noexcept {
    return static_cast<typename tinystd::remove_reference<_Tp>::type &&>(__t);
}

template <typename _Tp>
constexpr _Tp &&forward(_Tp &&__t) noexcept {
    return static_cast<_Tp &&>(__t);
}

// template <typename _Tp> 
// void swap(_Tp &__lhs, _Tp &__rhs) {
//     _Tp __tmp = __lhs;
//     __lhs = __rhs;
//     __rhs = __tmp;
// }

} // namespace tinystd

#endif // __TINYSTL_MOVE_H
