#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ios>
#include <memory>
#include <ostream>
#include <type_traits>
#include <vector> 
#include <iostream>
#include "tinystl_alloc.h"

void test_alloc();
void test_malloc_alloc_template();
void test_refill();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test alloc", test_alloc },
        { "test __malloc_alloc_template", test_malloc_alloc_template },
        { "test the refill of chunk", test_refill },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
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

static void __func() { printf("hello world!\n"); }

void test_malloc_alloc_template() {
    // tinystd::__malloc_alloc_template<0>::__debug_run_handler();  // 
    tinystd::__malloc_alloc_template<0>::_S_set_malloc_handler(__func);
    tinystd::__malloc_alloc_template<0>::__debug_run_handler();
}

void test_refill() {
    void *__mem = malloc(160);
    memset(__mem, 0, 160);
    int __last_addr = 0;
    std::cout << "last address: " << &__last_addr << std::endl;
    std::cout << "__mem: " << __mem << std::endl;
    tinystd::__default_alloc_template<0>::__debug_refill(32, (char *)__mem, 5, &__last_addr);
    for (int __i = 0; __i < 160 / 8; ++__i) {
        std::cout << ((long *)__mem + __i) << " 0x" << std::hex << *((long *)__mem + __i) << std::endl;
    }
    free(__mem);
}
