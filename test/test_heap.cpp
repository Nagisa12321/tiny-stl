#include <iostream>
#include <vector>
#include "tinystl_heap.h"
#include "tinystl_vector.h"
#include "tinystl_iterator.h"
#include <cassert>

void __test_pop_heap();
void __test_sort_heap();

int main() {
    srand(time(0));
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test pop_heap(), ", __test_pop_heap },
        { "test sort_heap(). ", __test_sort_heap },
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