#include "tinystl_deque.h"
#include "tinystl_list.h"
#include "tinystl_vector.h"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <ostream>
#include <vector>
#include <algorithm>

void __test_iterator();
void __test_std_deque();
void __test_init();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test iterator", __test_iterator },
        { "test std::deque", __test_std_deque },
        { "test init", __test_init }, 
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

void __test_iterator() {
    {
        std::cout << "test1: opt+=, opt++, opt--" << std::endl;
        tinystd::__deque_iterator<int, int &, int *, 10> __it;
        int *__buffer1 = (int *) malloc(30 * sizeof(int));
        int *__buffer2 = __buffer1 + 10;
        int *__buffer3 = __buffer2 + 10;
        for (int i = 0; i < 10; ++i) {
            __buffer1[i] = i;
            __buffer2[i] = i + 10;
            __buffer3[i] = i + 20;
        }
        int **map = (int **) malloc(sizeof(int *));
        map[0] = __buffer1;
        map[1] = __buffer2;
        map[2] = __buffer3;

        // setup __it;
        __it._M_set_node(map);
        __it._M_cur = __it._M_first;
        std::cout << "now *__it is " << *__it << std::endl; 
        __it += 5;
        std::cout << "after +=5, *__it is " << *__it << std::endl;
        __it += 19;
        std::cout << "after +=19, *__it is " << *__it << std::endl;
        --__it;
        std::cout << "after --, *__it is " << *__it << std::endl;
        __it._M_set_node(map + 1);
        __it._M_cur = __it._M_first;
        std::cout << "set __it to 10, __it is " << *__it <<  std::endl;
        --__it;
        std::cout << "after --, *__it is " << *__it << std::endl;
        ++__it;
        std::cout << "after ++, *__it is " << *__it << std::endl;


        free(map);
        free(__buffer1);
    }
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