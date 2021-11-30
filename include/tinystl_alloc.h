#ifndef __TINYSTL_ALLOC_H
#define __TINYSTL_ALLOC_H
#include <stdlib.h>     // for malloc(), free()
#include <limits.h>     // for UINT_MAX
#include "tinystl_types.h"

namespace tinystd {

template <typename _Tp> 
inline _Tp *allocate(size_t __sz) {
    return __sz == 0 ? 0 : 
        (_Tp *) malloc(__sz * sizeof(_Tp));
}

template <typename _Tp> 
inline _Tp *deallocate(_Tp *__p, size_t __sz) { free(__p); }

template <typename _Tp>
class allocator {
public:
    typedef _Tp value_type;
    typedef _Tp *pointer;
    typedef const _Tp *const_pointer;
    typedef _Tp &reference;
    typedef const _Tp &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t name;

    allocator() noexcept {}
    allocator(const allocator &__alloc) noexcept {} 
    template <typename _Tp1> allocator(const allocator<_Tp1> &__alloc) noexcept {}
    ~allocator() noexcept {}

    pointer address(reference __ref) const { return &__ref; }
    const_pointer address(const_reference __cref) const { return &__cref; }

    pointer allocate(size_type __sz, const void * = 0) { return tinystd::allocate<_Tp>(__sz); }
    void deallocate(pointer __p, size_type __sz) { tinystd::deallocate(__p, __sz); }

    size_type max_size() const { return size_type(UINT_MAX / sizeof(_Tp)); }

    void construct(pointer __p, const _Tp &__a) { new(__p) _Tp(__a); }
    void destory(pointer __p) {  __p->~_Tp(); } /* destructor, but no free. */
};

template <> class allocator<void> {
    typedef void value_type;
    typedef void *pointer;
    typedef const void *const_pointer;
    typedef size_t size_type;
    typedef ptrdiff_t name;
};

} // namespace tinystd

#endif // __TINYSTL_ALLOC_H
