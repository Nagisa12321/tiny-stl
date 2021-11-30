#include <cstdlib>
#include <ostream>
#include <type_traits>
#include <vector> 
#include <iostream>
#include "tinystl_alloc.h"

void test_alloc();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test alloc", test_alloc },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << ">> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

void test_alloc() {
    std::cout << "test1: " << std::endl;
    int *pi = (int *) malloc(sizeof(int));
    new (pi) int(1024);
    std::cout << "*pi=" << *pi << std::endl;
    free(pi);

    std::cout << "test2: " << std::endl;
    int *__pi;
    tinystd::allocator<int> __alloc;
    __pi = __alloc.allocate(10);
    for (int __i = 0; __i < 10; ++__i) {
        __alloc.construct(__pi + __i, __i);
    }
    for (int __i = 0; __i < 10; ++__i) {
        std::cout << __pi[__i] << " ";
    }
    std::cout << std::endl;
    // for (int __i = 0; __i < 10; ++__i) {
    //     __alloc.destory(__pi);
    // }
    __alloc.deallocate(__pi, 10);

    std::cout << "test3: " << std::endl;
    tinystd::allocator<void> __valloc;

}