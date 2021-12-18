#ifndef __TINYSTL_SPEED_H
#define __TINYSTL_SPEED_H
#include <iostream>
#include <sys/time.h>

struct __timer {
    typedef long __time_type;
    static void __start() { gettimeofday(&__ts, nullptr); }
    static void __end() { gettimeofday(&__te, nullptr); }
    static long __get_ms() {
        return (__te.tv_sec - __ts.tv_sec) * 1000 
            + (__te.tv_usec - __ts.tv_usec) / 1000;
    }
    static struct timeval __ts, __te;
};
struct timeval __timer::__ts;
struct timeval __timer::__te;

template <typename _InputIter>
void __print_container(_InputIter __lhs, _InputIter __rhs) {
    std::cout << "[";
    while (__lhs != __rhs) 
        { std::cout << *__lhs++ << ", "; }
    std::cout << "]" << std::endl;
}

#endif // __TINYSTL_SPEED_H
