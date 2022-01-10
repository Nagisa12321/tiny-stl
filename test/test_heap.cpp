#include <iostream>
#include <vector>
#include "tinystl_heap.h"
#include "tinystl_vector.h"
#include "tinystl_iterator.h"
#include <cassert>

void __test_pop_heap();
void __test_sort_heap();
void __test_push_heap();
void __test_heap();

int main() {
    srand(time(0));
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test pop_heap(), ", __test_pop_heap },
        { "test sort_heap(). ", __test_sort_heap },
        { "test push_heap(). ", __test_push_heap },
        { "test heap.... ", __test_heap },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

// v: 9 5 4 1 1 3
// after pop_heap: 5 3 4 1 1 9
// largest element: 9
// heap without largest: 5 3 4 1 1
void __test_pop_heap() {
    tinystd::vector<int> v{3, 1, 4, 1, 5, 9};

    tinystd::make_heap(v.begin(), v.end());

    std::cout << "v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    tinystd::pop_heap(v.begin(), v.end()); // moves the largest to the end

    std::cout << "after pop_heap: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    int largest = v.back();
    v.pop_back(); // actually removes the largest element
    std::cout << "largest element: " << largest << '\n';

    std::cout << "heap without largest: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
}

template <typename _Iterator>
bool __is_sorted(_Iterator __first, _Iterator __last) {
    if (__first == __last) return true;
    while (tinystd::next(__first) != __last) {
        if (*__first > *tinystd::next(__first))
            return false;
        tinystd::advance(__first, 1);
    }
    return true;
}

template <typename _Iterator> 
bool __is_heap(_Iterator __first, _Iterator __last) {
    _Iterator __pos = __first;
    while (__pos != __last) {
        _Iterator __c1 = __first + 2 * (__pos - __first) + 1;
        _Iterator __c2 = __first + 2 * (__pos - __first) + 2;
        if (__c1 < __last && *__c1 > *__pos) return false;
        if (__c2 < __last && *__c2 > *__pos) return false;
        ++__pos;
    }
    return true;
}

// heap:   9 4 5 1 1 3 
// sorted: 1 1 3 4 5 9
void __test_sort_heap() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::vector<int> v = {3, 1, 4, 1, 5, 9};

        tinystd::make_heap(v.begin(), v.end());    std::cout << "heap:\t";

        for (const auto &i : v) {
            std::cout << i << ' ';

        }

        tinystd::sort_heap(v.begin(), v.end());    std::cout << "\nsorted:\t";

        for (const auto &i : v) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "test2: sort many. " << std::endl;
    {
        tinystd::vector<int> ivec; 
        for (int i = 0; i < 0xfffff; ++i) {
            ivec.push_back(rand() % 0xfffff);
        }
        tinystd::make_heap(ivec.begin(), ivec.end());
        tinystd::sort_heap(ivec.begin(), ivec.end());

        assert(__is_sorted(ivec.begin(), ivec.end()));

        std::cout << "...ok!" << std::endl;
    }
}

// v: 9 5 4 1 1 3 
// before push_heap: 9 5 4 1 1 3 6 
// after push_heap:  9 5 6 1 1 3 4
void __test_push_heap() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::vector<int> v{3, 1, 4, 1, 5, 9};

        tinystd::make_heap(v.begin(), v.end());

        std::cout << "v: ";
        for (auto i : v) std::cout << i << ' ';
        std::cout << '\n';

        v.push_back(6);

        std::cout << "before push_heap: ";
        for (auto i : v) std::cout << i << ' ';
        std::cout << '\n';

        tinystd::push_heap(v.begin(), v.end());

        std::cout << "after push_heap:  ";
        for (auto i : v) std::cout << i << ' ';
        std::cout << '\n';
    }
    std::cout << "test2: " << std::endl;
    {
        tinystd::vector<int> ivec;
        for (int i = 0; i < 0xffff; ++i) {
            ivec.push_back(rand() % 0xffff);
            tinystd::push_heap(ivec.begin(), ivec.end());
        }
        assert(__is_heap(ivec.begin(), ivec.end()));

        std::cout << "...ok!" << std::endl;
    }
}

// 9 5 8 3 4 0 2 1 3 
// 9 7 8 3 5 0 2 1 3 4 
// 9
// 8 7 4 3 5 0 2 1 3 
// 0 1 2 3 3 4 5 7 8
void __test_heap() {
    std::cout << "test1: " << std::endl;
    {
        int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5 };
        tinystd::vector<int> ivec(ia, ia + 9);

        tinystd::make_heap(ivec.begin(), ivec.end());
        for (int i = 0; i < ivec.size(); ++i) 
            std::cout << ivec[i] << ' ';
        std::cout << std::endl;

        ivec.push_back(7);
        tinystd::push_heap(ivec.begin(), ivec.end());
        for (int i = 0; i < ivec.size(); ++i) 
            std::cout << ivec[i] << ' ';
        std::cout << std::endl;

        tinystd::pop_heap(ivec.begin(), ivec.end());
        std::cout << ivec.back() << std::endl;
        ivec.pop_back();

        for (int i = 0; i < ivec.size(); ++i) 
            std::cout << ivec[i] << ' ';
        std::cout << std::endl;

        tinystd::sort_heap(ivec.begin(), ivec.end());
        for (int i = 0; i < ivec.size(); ++i) 
            std::cout << ivec[i] << ' ';
        std::cout << std::endl;
    }
    std::cout << "test2: " << std::endl;
    {
        int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5, };
        tinystd::make_heap(ia, ia + 9);
        tinystd::sort_heap(ia, ia + 9);

        for (int i = 0; i < 9; ++i) 
            std::cout << ia[i] << ' ';
        std::cout << std::endl;
    }
}