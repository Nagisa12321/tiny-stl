#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <string.h>
#include <list>
#include <queue>
#include <forward_list>
#include <list>
#include "tinystl_algo.h"

/*
v contains 3
v does not contain 5
*/
void __test_find();
void __test_advance();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases {
        { "find", __test_find },
        { "advance", __test_advance },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

void __test_find() {
    {
        std::cout << "test 1: " << std::endl;
        std::vector<int> v{1, 2, 3, 4};
        int n1 = 3;
        int n2 = 5;
    
        auto result1 = tinystd::find(begin(v), end(v), n1);
        auto result2 = tinystd::find(begin(v), end(v), n2);
    
        (result1 != std::end(v))
            ? std::cout << "v contains " << n1 << '\n'
            : std::cout << "v does not contain " << n1 << '\n';
    
        (result2 != std::end(v))
            ? std::cout << "v contains " << n2 << '\n'
            : std::cout << "v does not contain " << n2 << '\n';
    }

    {
        std::cout << "test 2: " << std::endl;
        const int __array_size = 7;
        int __ia[__array_size] = { 0, 1, 2, 3, 4, 5, 6, };
        std::vector<int> __ivec(__ia, __ia + __array_size);
        std::list<int> __ilst(__ia, __ia + __array_size);
        std::deque<int> __ideq(__ia, __ia + __array_size);

        std::vector<int>::iterator __it1 = tinystd::find(__ivec.begin(), __ivec.end(), 4);
        if (__it1 == __ivec.end()) { std::cout << "4 not found" << std::endl; }
        else { std::cout << "4 found. " << *__it1 << std::endl; }

        std::list<int>::iterator __it2 = tinystd::find(__ilst.begin(), __ilst.end(), 6);
        if (__it2 == __ilst.end()) { std::cout << "6 not found" << std::endl; }
        else { std::cout << "6 found. " << *__it2 << std::endl; }

        std::deque<int>::iterator __it3 = tinystd::find(__ideq.begin(), __ideq.end(), 8);
        if (__it3 == __ideq.end()) { std::cout << "8 not found" << std::endl; }
        else { std::cout << "8 found. " << *__it3 << std::endl; }
        
    }
}

void __test_advance() {
    {
        std::cout << "test 1: "<< std::endl;
        int __ia[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,  };
        // std::forward_list<int> __iflst(__ia, __ia + 10);
        // std::list<int> __ilst(__ia, __ia + 10);
        int *__int_iter = __ia;
        // auto __forward_list_iter = __iflst.begin();
        // auto __list_iter = __ilst.begin();

        tinystd::advance(__int_iter, 5);
        // tinystd::advance(__forward_list_iter, 5);
        // tinystd::advance(__list_iter, 5);
        std::cout << "*__int_iter=" << *__int_iter << std::endl;
        // std::cout << "*__forward_list_iter=" << *__forward_list_iter << std::endl;
        // std::cout << "*__list_iter=" << *__list_iter << std::endl;

    }
}