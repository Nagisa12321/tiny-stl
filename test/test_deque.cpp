#include "tinystl_deque.h"
#include "tinystl_list.h"
#include "tinystl_vector.h"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

void __test_std_deque();
void __test_init();
void __test_push();
void __test_pop();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        // { "test std::deque", __test_std_deque },
        // { "test init", __test_init }, 
        // { "test push_back and push_front", __test_push }, 
        { "test pop_back and pop_front", __test_pop }, 
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

template <typename _InputIter>
void __print_container(_InputIter __lhs, _InputIter __rhs) {
    std::cout << "[";
    while (__lhs < __rhs) 
        std::cout << *__lhs++ << ", ";
    std::cout << "]" << std::endl;
}


void __test_std_deque() {
    std::deque<int> __ideq(20, 9);
    std::cout << "size=" << __ideq.size() << std::endl;

    for (int i = 0; i < __ideq.size(); ++i)
        __ideq[i] = i;

    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;

    for (int i = 0; i < 3; ++i)
        __ideq.push_back(i);

    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;
    std::cout << "size=" << __ideq.size() << std::endl;

    __ideq.push_back(3);
    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;
    std::cout << "size=" << __ideq.size() << std::endl;

    __ideq.push_front(99);
    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;
    std::cout << "size=" << __ideq.size() << std::endl;

    __ideq.push_front(98);
    __ideq.push_front(97);
    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;
    std::cout << "size=" << __ideq.size() << std::endl;

    std::deque<int>::iterator it;
    it = std::find(__ideq.begin(), __ideq.end(), 99);
    std::cout << *it << std::endl;
    std::cout << *(it._M_cur) << std::endl;
}

void __test_init() {
    {
        std::cout << "test1: deque(size_type __n, const _Tp &__value)" << std::endl;
        tinystd::deque<int> __ideq(256, 2048);
        for (int i : __ideq)
            std::cout << i << ' ';
        std::cout << std::endl;
    }
}

void __test_push() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::deque<int> __ideq;
        std::cout << "__ideq.size()=" << __ideq.size() << std::endl;
        for (int i = 0; i < 10; ++i)
            __ideq.push_back(2048);
        std::cout << "__ideq.size()=" << __ideq.size() << std::endl;
        for (int i : __ideq)
            std::cout << i << ' ';
        std::cout << std::endl;
        for (int i = 0; i < 256; ++i)
            __ideq.push_back(i);
        std::cout << "__ideq.size()=" << __ideq.size() << std::endl;
        for (int i : __ideq)
            std::cout << i << ' ';
        std::cout << std::endl;
    }
    std::cout << "test2: " << std::endl; 
    {
        tinystd::deque<std::string> __sdeq;
        __sdeq.push_back("hello");
        __sdeq.push_front("hi");
        __print_container(__sdeq.begin(), __sdeq.end());
    }
    std::cout << "test3: " << std::endl;
    {
        tinystd::deque<std::string> __sdeq;
        for (int i = 0; i < 500; ++i) {
            __sdeq.push_back(std::to_string(i));
            __sdeq.push_front(std::to_string(i));
        }
        __print_container(__sdeq.begin(), __sdeq.end());
    }
}
/**
 * @brief 
 * test1: pop_front: 
 * ABCDE
 * BCDEF
 * CDEFG
 * DEFGH
 * test2: pop_back: 
 * []
 * [5, 3, 4, ]
 * [5, 3, ]
 * test3: pop too many
 * 32768
 * 0
 */
void __test_pop() {
    // std::cout << "test1: pop_front: " << std::endl;
    // {
    //     auto print = [](const auto &con) {
    //         for (auto const &item : con)
    //             std::cout << item;
    //         std::cout << '\n';
    //     };

    //     tinystd::deque<char> letters{'A', 'B', 'C', 'D'};

    //     for (char c{letters.back()}; c != 'H';) {
    //         letters.push_back(++c);
    //         print(letters);
    //         letters.pop_front();
    //     }
    // }
    // std::cout << "test2: pop_back: " << std::endl;
    // {
    //     tinystd::deque<int> numbers;
    
    //     __print_container(numbers.begin(), numbers.end()); 
    
    //     numbers.push_back(5);
    //     numbers.push_back(3);
    //     numbers.push_back(4);
    
    //     __print_container(numbers.begin(), numbers.end()); 
    
    //     numbers.pop_back();
    
    //     __print_container(numbers.begin(), numbers.end()); 
    // }
    std::cout << "test3: pop too many" << std::endl;
    {
        int __k = 1024;
        tinystd::deque<int> numbers;
        for (int i = 0; i < __k; ++i)
            numbers.push_back(i);
        int a = __k >> 1;
        for (int i = 0; i < a; ++i)
            numbers.pop_back();
        std::cout << numbers.size() << std::endl;
        for (int i = 0; i < __k - a; ++i)
            numbers.pop_front();
        std::cout << numbers.size() << std::endl;
    }
}