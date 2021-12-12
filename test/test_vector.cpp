#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>
#include "tinystl_alloc.h"
#include "tinystl_initializer_list.h"
#include "tinystl_vector.h"

void __test_vector();
void __test_constructor();
void __test_operator_equal();
void __test_memory_leak();
void __test_push_back();
void __test_emplace_back();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test vector", __test_vector },
        { "test constructor", __test_constructor },
        { "test operator equal", __test_operator_equal },
        // { "test memory leak", __test_memory_leak },
        { "test push back", __test_push_back },
        { "test emplace back", __test_emplace_back },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

/**
 * @brief
 * v = { 7, 5, 16, 8, 25, 13, };
 */
void __test_vector() {
    // Create a vector containing integers
    tinystd::vector<int> v = {7, 5, 16, 8};

    // Add two more integers to vector
    v.push_back(25);
    v.push_back(13);

    // Print out the vector
    std::cout << "v = { ";
    for (int n : v) {
        std::cout << n << ", ";
    }
    std::cout << "}; \n";
}

template <typename T>
std::ostream &operator<<(std::ostream &s, const tinystd::vector<T> &v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto &e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}

void __test_constructor() {
    std::vector<int> ivec;
    // c++11 initializer list syntax:
    tinystd::vector<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words1: " << words1 << '\n';

    // words2 == words1
    tinystd::vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: " << words2 << '\n';

    // words3 == words1
    tinystd::vector<std::string> words3(words1);
    std::cout << "words3: " << words3 << '\n';

    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    tinystd::vector<std::string> words4(5, "Mo");
    std::cout << "words4: " << words4 << '\n';
}
void __display_sizes(char const *comment,
                     const tinystd::vector<int> &nums1,
                     const tinystd::vector<int> &nums2,
                     const tinystd::vector<int> &nums3) {
    std::cout << comment
              << " nums1: " << nums1.size() << ','
              << " nums2: " << nums2.size() << ','
              << " nums3: " << nums3.size() << '\n';
}

void __display(char const *comment, const tinystd::vector<int> &v) {
    std::cout << comment << "{ ";
    for (int e : v) {
        std::cout << e << ' ';
    }
    std::cout << "}\n";
}

void __test_operator_equal() {
    tinystd::vector<int> nums1{3, 1, 4, 6, 5, 9};
    tinystd::vector<int> nums2;
    tinystd::vector<int> nums3;

    __display_sizes("Initially:\n", nums1, nums2, nums3);

    // copy assignment copies data from nums1 to nums2
    nums2 = nums1;

    __display_sizes("After assignment:\n", nums1, nums2, nums3);

    // move assignment moves data from nums1 to nums3,
    // modifying both nums1 and nums3
    nums3 = std::move(nums1);

    __display_sizes("After move assignment:\n", nums1, nums2, nums3);

    __display("Now nums3 = ", nums3);

    // copy assignment of an initializer_list copies data to nums3
    nums3 = {1, 2, 3};

    __display("After assignment of initializer_list \n nums3 = ", nums3);
}

void __test_memory_leak() {
    // Create a vector containing integers
    tinystd::vector<int> v = {7, 5, 16, 8,};
    tinystd::vector<int> nums1{3, 1, 4, 6, 5, 9};
    tinystd::vector<int> nums2{3, 1, 4, 6, 5, 9};
    tinystd::vector<int> nums3{1,2,3};

    // Add two more integers to vector
    v.push_back(25);
    v.push_back(13);

    tinystd::__default_alloc_template<0>::__debug_print_mem_pool();
}

void __test_push_back() {

    tinystd::vector<int> myvector{ 1, 2, 3, 4, 5 };
    myvector.push_back(6);
 
    // Vector becomes 1, 2, 3, 4, 5, 6
 
    for (auto it = myvector.begin(); it != myvector.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << std::endl;
}

struct President
{
    std::string name;
    std::string country;
    int year;
 
    President(std::string p_name, std::string p_country, int p_year)
        : name(std::move(p_name)), country(std::move(p_country)), year(p_year)
    {
        std::cout << "I am being constructed.\n";
    }
    // President(President&& other)
    //         : name(std::move(other.name)), country(std::move(other.country)), year(other.year)
    //     {
    //     std::cout << "I am being moved.\n";
    // }
    President& operator=(const President& other) = default;
};
/*
>> test emplace back ========================
emplace_back:
I am being constructed.
I am being constructed.
I am being moved.

push_back:
I am being constructed.
I am being moved.

Contents:
Nelson Mandela was elected president of South Africa in 1994.
Nelson Mandela was elected president of South Africa in 1994.
Franklin Delano Roosevelt was re-elected president of the USA in 1936.
*/
void __test_emplace_back() {
    tinystd::vector<President> elections;
    std::cout << "emplace_back:\n";
    auto &ref = elections.emplace_back("Nelson Mandela", "South Africa", 1994);
    assert(ref.year == 1994);
 
    tinystd::vector<President> reElections;
    std::cout << "\npush_back:\n";
    reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));
 
    std::cout << "\nContents:\n";
    for (President const& president: elections) {
        std::cout << president.name << " was elected president of "
                  << president.country << " in " << president.year << ".\n";
    }
    for (President const& president: reElections) {
        std::cout << president.name << " was re-elected president of "
                  << president.country << " in " << president.year << ".\n";
    }
}