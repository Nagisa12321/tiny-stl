#ifndef __TINYSTD_ITERATOR_BASE_H
#define __TINYSTD_ITERATOR_BASE_H
#include "tinystl_types.h"

namespace tinystd {

struct __input_iter {};
struct __output_iter {};
struct __forward_iter : public __input_iter {};
struct __bidirectional_iter : public __forward_iter {};
struct __random_access_iter : public __bidirectional_iter {};

template <typename _Iterator>
struct __iterator_traits {
    typedef typename _Iterator::iterator_category   iterator_category;
    typedef typename _Iterator::value_type          value_type;
    typedef typename _Iterator::difference_type     difference_type;
    typedef typename _Iterator::pointer             pointer;
    typedef typename _Iterator::reference           reference;
};

template <typename _Tp>
struct __iterator_traits<_Tp*> {
    typedef __random_access_iter    iterator_category;
    typedef _Tp                     value_type; 
    typedef ptrdiff_t               difference_type;
    typedef _Tp*                    pointer;
    typedef _Tp&                    reference;

};

template <typename _Tp>
struct __iterator_traits<const _Tp*> {
    typedef __random_access_iter    iterator_category;
    typedef _Tp                     value_type; 
    typedef ptrdiff_t               difference_type;
    typedef const _Tp*              pointer;
    typedef const _Tp&              reference;
};

/* the iterator's base class ...  */
template <typename _Category,
          typename _Tp,
          typename _Distance = ptrdiff_t,
          typename _Pointer = _Tp*,
          typename _Reference = _Tp&>
struct __iterator_base {
    typedef _Category           iterator_category;
    typedef _Tp                 value_type; 
    typedef _Distance           difference_type;
    typedef const _Pointer      pointer;
    typedef const _Reference    reference;
};

template <typename _Iterator>
inline constexpr typename __iterator_traits<_Iterator>::iterator_category __iterator_category(const _Iterator &) {
    typedef typename __iterator_traits<_Iterator>::iterator_category __category;
    return __category();
}

template <typename _Iterator>
inline constexpr typename __iterator_traits<_Iterator>::difference_type *__distance_type(const _Iterator &) {
    return static_cast<typename __iterator_traits<_Iterator>::difference_type *>(0);
}

template <typename _Iterator>
inline constexpr typename __iterator_traits<_Iterator>::value_type *__value_type(const _Iterator &) {
    return static_cast<typename __iterator_traits<_Iterator>::value_type *>(0);
}

}
#endif // __TINYSTD_ITERATOR_BASE_H 