// uninitialized_copy example
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <ostream>
#include <vector>
#include <iostream>
#include <string>
#include "tinystl_algo.h"
#include "tinystl_uninitialized.h"

void test_uninitialized_copy();
void test_uninitialized_fill();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        {"test uninitialized copy", test_uninitialized_copy},
        {"test uninitialized fill", test_uninitialized_fill},
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

class __A {
public:
    __A() {
        printf("__A()\n");
    }
    __A(const __A &__a) {
        printf("__A(const __A &__a)\n");
    }
    ~__A() {
        printf("~__A()\n");
    }
    __A &operator=(const __A &__a) {
        printf("__A &operator=(const __A &__a)\n");
        return *this;
    }
};

void test_uninitialized_copy() {
    {
        std::cout << "test1: " << std::endl;
        std::string numbers[] = {"one", "two", "three"};

        // get block of uninitialized memory:
        std::pair<std::string *, std::ptrdiff_t> result = std::get_temporary_buffer<std::string>(3);

        if (result.second > 0) {
            tinystd::uninitialized_copy(numbers, numbers + result.second, result.first);

            for (int i = 0; i < result.second; i++)
                std::cout << result.first[i] << " ";
            std::cout << '\n';

            std::return_temporary_buffer(result.first);
        }
    }
    {
        std::cout << "test2: " << std::endl;
        __A __arr[5];
        __A *__pa = (__A *)malloc(5 * sizeof(__A));
        tinystd::uninitialized_copy(__arr, __arr + 5, __pa);
        for (int __i = 0; __i < 5; ++__i)
            (__pa + __i)->~__A();
        free(__pa);
    }
    {
        std::cout << "test3: " << std::endl;
        int __arr[5];
        int *__pa = (int *)malloc(5 * sizeof(int));
        tinystd::uninitialized_copy(__arr, __arr + 5, __pa);
        free(__pa);
    }
}

void test_uninitialized_fill() {
    {
        std::cout << "test1: " << std::endl;
        std::string *p;
        std::size_t sz;
        std::tie(p, sz) = std::get_temporary_buffer<std::string>(4);

        tinystd::uninitialized_fill(p, p + sz, "Example");

        for (std::string *i = p; i != p + sz; ++i) {
            std::cout << *i << '\n';
            i->~basic_string<char>();
        }
        std::return_temporary_buffer(p);
    }

    {
        std::cout << "test2: " << std::endl;
        // get block of uninitialized memory:
        std::string *__pstr = (std::string *) malloc(sizeof(std::string) * 3);
        for (int i = 0; i < 3; i++) new (__pstr + i) (std::string)();
        tinystd::uninitialized_fill(__pstr, __pstr + 3, "c++ rocks!");

        for (int i = 0; i < 3; i++)
            std::cout << __pstr[i] << '\n';

        free(__pstr);
    }
}
