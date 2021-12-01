#include <cstdlib>
#include <new>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "tinystl_constructor.h"

using namespace std;

void test_destory_class();
void test_destory_value();
void test_destory_chars();
void test_call_value_destructor();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "destory class", test_destory_class },
        { "destory value", test_destory_value },
        { "destory chars", test_destory_chars },
        { "call calue destructor", test_call_value_destructor },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << ">> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

class Foo {
public:
    Foo() : __pi(new int(1024)) { cout << "Foo()" << endl; }
    ~Foo() { cout << "~Foo()" << endl; delete __pi; }
private:
    int *__pi;
};

void test_destory_class() {
    Foo *__foos = (Foo *) malloc(10 * sizeof(Foo));
    for (int __i = 0; __i < 10; ++__i) {
        new(__foos + __i) Foo();
    }
    tinystd::destory(__foos, __foos + 10);
    free(__foos);
}

void test_destory_value() {
    int *__ints = (int *) malloc(10 * sizeof(int));
    for (int __i = 0; __i < 10; ++__i) {
        new(__ints + __i) int();
    }
    tinystd::destory(__ints, __ints + 10);
    free(__ints);
}


void test_destory_chars() {

}

template<typename _InputIter> 
void __my_destory(_InputIter __lhs, _InputIter __rhs) {
    while (__lhs != __rhs) 
        tinystd::destory(&*__lhs++);
}

void test_call_value_destructor() {
    /* so call the ~int() is ok but do nothing */
    int* __pi = (int *) malloc(10 * sizeof(int));
    __my_destory(__pi, __pi + 10);
    free(__pi);
}