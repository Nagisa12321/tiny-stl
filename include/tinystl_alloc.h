#ifndef __TINYSTL_ALLOC_H
#define __TINYSTL_ALLOC_H
#include <stdlib.h>     // for malloc(), free(), exit()
#include <limits.h>     // for UINT_MAX
#include <string.h>
#include <stdio.h>
#include "tinystl_constructor.h"
#include "tinystl_types.h"
#define __THROW_BAD_ALLOC   fprintf(stderr, "out of memory"); exit(1)
#define __DEFAULT_ALLOCATOR __malloc_alloc_template<0>

namespace tinystd {

template <typename _Tp> inline _Tp *allocate(size_t __sz);
template <typename _Tp> inline void deallocate(_Tp *__p, size_t __sz);


/**
 * @tparam _Alloc 
 *      - __malloc_alloc_template
 *      - __defalut_alloc_template
 */
template <typename _Tp, class _Alloc> 
class simple_alloc {
public:
    static _Tp *_S_allocate(size_t __n) {
        return __n == 0 ? 0 :
            (_Tp*) _Alloc::_S_allocate(__n * sizeof(_Tp));
    }
    static _Tp *_S_allocate(void) {    /* only allocate 1 */
        return (_Tp *) _Alloc::_S_allocate(sizeof(_Tp));
    }
    static void _S_deallocate(_Tp *__p, size_t __sz) {
        if (__sz != 0) 
            _Alloc::_S_deallocate(__p, __sz * sizeof(_Tp));
    }
    static void _S_deallocate(_Tp *__p) {
        _Alloc::_S_deallocate(__p, sizeof(_Tp));
    }
};

template <int _Inst>
class __malloc_alloc_template {
public:
    static void *_S_allocate(size_t __sz) {
        void *__res = malloc(__sz);
        if (!__res) return _S_oom_malloc(__sz);
        return __res;
    }

    static void _S_deallocate(void *__p, size_t __sz) { 
        /* memset(__p, 0x1, __sz); */
        free(__p); 
    }

    static void *_S_reallocate(void *__p, size_t /*__old_sz*/, size_t __new_sz) {
        void *__res = realloc(__p, __new_sz);
        if (!__res) return _S_oom_realloc(__p, __new_sz);
        return __res;
    }

    static void _S_set_malloc_handler(void (*__handler)()) {
        __malloc_alloc_oom_handler = __handler;
    }

    static void __debug_run_handler() { __malloc_alloc_oom_handler(); }
private:
    static void (*__malloc_alloc_oom_handler)();
    static void *_S_oom_malloc(size_t __sz) {
        void (*__my_alloc_handler)();
        void *__res;
        for (;;) {
            __my_alloc_handler = __my_alloc_handler;
            if (!__my_alloc_handler) { __THROW_BAD_ALLOC; }
            __my_alloc_handler();   // exec handler
            __res = malloc(__sz);
            if (__res) return __res;
        }
    }
    static void *_S_oom_realloc(void *__p, size_t __new_sz) {
        void (*__my_alloc_handler)();
        void *__res;
        for (;;) {
            __my_alloc_handler = __my_alloc_handler;
            if (!__my_alloc_handler) { __THROW_BAD_ALLOC; }
            __my_alloc_handler();   // exec handler
            __res = realloc(__p, __new_sz);
            if (__res) return __res;
        }
    }
};

template <int _Inst>
void (*__malloc_alloc_template<_Inst>::__malloc_alloc_oom_handler)() = 0;

enum { __ALIGN = 8, };                              // cast blocks
enum { __MAX_BYTES = 128, };                        // bytes of blocks
enum { __NFREELISTS = __MAX_BYTES / __ALIGN, };     // numbers of blocks

template <int _Inst>
class __default_alloc_template {
public:
    static void *_S_allocate(size_t __sz) {
        __obj *__my_free_list;
        __obj *__res;
        if (__sz > (size_t) __MAX_BYTES) { 
            return __malloc_alloc_template<0>::_S_allocate(__sz); 
        }
        __my_free_list = _S_free_list + _S_freelist_index(__sz);
        __res = __my_free_list->__free_list_link; 
        if (!__res) { return _S_refill(_S_round_up(__sz)); }
        __my_free_list->__free_list_link = __res->__free_list_link;
        return __res;
    }
    static void *_S_reallocate(void *__p, size_t __old_sz, size_t __new_sz) {
        return 0;
    }
    static void _S_deallocate(void *__p, size_t __n) {
        __obj *__my_free_list;
        __obj *__old_head, *__new_head;
        /* memset(__p, 0x1, __n); */

        if (__n > (size_t) __MAX_BYTES) { 
            return __malloc_alloc_template<0>::_S_deallocate(__p, __n); 
        }
        __my_free_list = _S_free_list + _S_freelist_index(__n);
        __old_head = __my_free_list->__free_list_link;
        __new_head = (__obj *) __p;
        __new_head->__free_list_link = __old_head;
        __my_free_list->__free_list_link = __new_head;
    }

    static void *__debug_refill(size_t __sz, char *__chunk, int __nobjs, void *__last_address_fill) {
        __obj  *__new_alloc, *__next_obj, *__current_obj;
        __new_alloc = (__obj *) __chunk;

        for (int __i = 1; __i < __nobjs; ++__i) { /* link the rest chunk */
            __current_obj = (__obj *) ((char *) __new_alloc + __sz * __i);
            if (__i == __nobjs - 1) {
                __current_obj->__free_list_link = (__obj *)__last_address_fill;
            } else {
                __current_obj->__free_list_link = (__obj *) ((char *) __current_obj + __sz);
            }
        }
        return __new_alloc;
    }

    static void __debug_print_mem_pool() {
        printf("Memory Pool: \n");
        for (int __offset = 0; __offset < __NFREELISTS; ++__offset) {
            printf("%d bytes: ", (__offset + 1) * 8);
            __obj *__cur = _S_free_list + __offset;
            if (!__cur) continue;
            while (__cur->__free_list_link) {
                printf("%p, ", __cur->__free_list_link);
                __cur = __cur->__free_list_link;
            }
            printf("\n");
        }
    }

    static void __debug_count_free(size_t *__free_list, size_t *__memory_pool) {
        *__free_list = *__memory_pool = 0;
        for (int __i = 0; __i < __NFREELISTS; ++__i) {
            __obj *__cur = _S_free_list + __i;
            if (!__cur) continue;
            while (__cur->__free_list_link) {
                *__free_list += (__i + 1) * 8;
                __cur = __cur->__free_list_link;
            }
        }
        *__memory_pool = _S_end_free - _S_start_free;
    }

    static char *__debug_chunk_alloc(size_t __sz, int __nobjs) {
        return _S_chunk_alloc(__sz, __nobjs);
    }

private:
    struct __obj {
        struct __obj *__free_list_link = 0;
    };

    static __obj _S_free_list[__NFREELISTS];
    static char *_S_start_free;
    static char *_S_end_free;
    static size_t _S_heap_size;

    static size_t _S_round_up(size_t __sz) {
        return (__sz + __ALIGN - 1) & ~(__ALIGN - 1);
    }
    static size_t _S_freelist_index(size_t __bytes) {
        return (__bytes + __ALIGN - 1) / __ALIGN - 1;
    }
    static void *_S_refill(size_t __sz) {
        __obj *__my_free_list, *__new_alloc, *__next_obj, *__current_obj;
        int __nobjs = 20;
        char *__chunk = _S_chunk_alloc(__sz, __nobjs);
        if (__nobjs == 1) return __chunk;
        __my_free_list = _S_free_list + _S_freelist_index(__sz);
        __new_alloc = (__obj *) __chunk;
        __my_free_list->__free_list_link = (__obj *) (__chunk + __sz);

        for (int __i = 1; __i < __nobjs; ++__i) { /* link the rest chunk */
            __current_obj = (__obj *) ((char *) __new_alloc + __sz * __i);
            if (__i == __nobjs - 1) {
                __current_obj->__free_list_link = 0;
            } else {
                __current_obj->__free_list_link = (__obj *) ((char *) __current_obj + __sz);
            }
        }
        return __new_alloc;
    } 
    static char *_S_chunk_alloc(size_t __sz, int &__nobjs) {
        char *__result;
        size_t __total_bytes = __sz * __nobjs;
        size_t __bytes_left = _S_end_free - _S_start_free;
        if (__bytes_left >= __total_bytes) {
            __result = _S_start_free;
            _S_start_free += __total_bytes;
            return __result;
        } else if (__bytes_left >= __sz) {
            __nobjs = __bytes_left / __sz;
            __result = _S_start_free;
            _S_start_free += __nobjs * __sz;
            return __result;
        } else {
            printf("_S_start_free=%p, _S_end_free=%p, __sz=%ld, __bytes_left=%ld\n", _S_start_free, _S_end_free, __sz, __bytes_left);
            if (__bytes_left > 0) {         /* also a crazy opteration! */
                __obj *__my_free_list = _S_free_list + _S_freelist_index(__bytes_left);
                __obj *__tmp = __my_free_list->__free_list_link;
                __my_free_list->__free_list_link = (__obj *) _S_start_free;
                ((__obj *) _S_start_free)->__free_list_link = __tmp;
            }
            size_t __bytes_to_get = 2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
            _S_start_free = (char *) malloc(__bytes_to_get);
            if (!_S_start_free) {
                // search the unused freelist!
                __obj *__my_free_list;
                for (int __i = __sz; __i <= __MAX_BYTES; __i += __ALIGN) {
                    __my_free_list = _S_free_list + _S_freelist_index(__i);
                    __obj *__first_free = __my_free_list->__free_list_link;
                    if (__first_free) {
                        _S_start_free = (char *) __first_free;
                        _S_end_free = _S_start_free + __i;
                        __my_free_list->__free_list_link =
                            __first_free->__free_list_link; 
                        /* call myself */
                        return _S_chunk_alloc(__sz, __nobjs);
                    }
                }
                // there is no more memory!
                _S_start_free = (char *) __malloc_alloc_template<_Inst>::_S_allocate(__bytes_to_get);
            }
            _S_end_free = _S_start_free + __bytes_to_get;
            _S_heap_size += __bytes_to_get;

            return _S_chunk_alloc(__sz, __nobjs);
        }
    }
};

template <int _Inst> 
typename __default_alloc_template<_Inst>::__obj 
__default_alloc_template<_Inst>::_S_free_list[__NFREELISTS];
template <int _Inst> char *__default_alloc_template<_Inst>::_S_start_free = 0;
template <int _Inst> char *__default_alloc_template<_Inst>::_S_end_free = 0;
template <int _Inst> size_t __default_alloc_template<_Inst>::_S_heap_size = 0;


template <typename _Tp> 
inline _Tp *allocate(size_t __sz) {
    return __sz == 0 ? 0 : 
        (_Tp *) malloc(__sz * sizeof(_Tp));
}

template <typename _Tp> 
inline void deallocate(_Tp *__p, size_t __sz) { free(__p); }

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

    void construct(pointer __p, const _Tp &__a) { tinystd::construct(__p, __a); }
    void destory(pointer __p) { tinystd::destory(__p); } /* destructor, but no free. */
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
