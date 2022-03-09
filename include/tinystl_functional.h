#ifndef __TINYSTL_FUNCTIONAL_H
#define __TINYSTL_FUNCTIONAL_H

#include "tinystl_types.h"
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

//
// STL hash function 
// 
inline tinystd::size_t __stl_hash_string(const char *__s) {
    unsigned long __h = 0;
    for (; *__s; ++__s) {
        __h = 5 * __h + *__s;
    }
    return tinystd::size_t(__h);
}

template <typename _Key> struct hash {};

template <> 
struct hash<char *> {
    tinystd::size_t operator()(const char *__s) const 
        { return tinystd::__stl_hash_string(__s); }
};

template <> 
struct hash<const char *> {
    tinystd::size_t operator()(const char *__s) const 
        { return tinystd::__stl_hash_string(__s); }
};

template <>
struct hash<char> {
    tinystd::size_t operator()(char __x) const 
        { return __x; }
};

template <>
struct hash<unsigned char> {
    tinystd::size_t operator()(unsigned char __x) const 
        { return __x; }
};

template <>
struct hash<short> {
    tinystd::size_t operator()(short __x) const 
        { return __x; }
};

template <>
struct hash<unsigned short> {
    tinystd::size_t operator()(unsigned short __x) const 
        { return __x; }
};

template <>
struct hash<int> {
    tinystd::size_t operator()(int __x) const 
        { return __x; }
};

template <>
struct hash<unsigned int> {
    tinystd::size_t operator()(unsigned int __x) const 
        { return __x; }
};
template <>
struct hash<long> {
    tinystd::size_t operator()(long __x) const 
        { return __x; }
};
template <>
struct hash<unsigned long> {
    tinystd::size_t operator()(unsigned long __x) const 
        { return __x; }
};

}

#endif // __TINYSTL_FUNCTIONAL_H
