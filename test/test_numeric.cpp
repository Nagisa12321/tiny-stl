#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <random>
#include "tinystl_numeric.h"
#include "tinystl_vector.h"
#include "tinystl_functional.h"
#include "tinystl_list.h"

void __test_accmulate();
void __test_adjacent_difference();
void __test_inner_product();
void __test_partial_sum();
void __test_iota();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test accmulate()...", __test_accmulate },
        { "test adjacent_difference()", __test_adjacent_difference },
        { "test inner_product()... ", __test_inner_product },
        { "test partial_sum()...", __test_partial_sum },
        { "test iota()... ", __test_iota },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

// sum: 55
// product: 3628800
// dash-separated string: 1-2-3-4-5-6-7-8-9-10
// dash-separated string (right-folded): 10-9-8-7-6-5-4-3-2-1
void __test_accmulate() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 
    int sum = tinystd::accumulate(v.begin(), v.end(), 0);
 
    int product = tinystd::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
 
    auto dash_fold = [](std::string a, int b) {
                         return std::move(a) + '-' + std::to_string(b);
                     };
 
    std::string s = tinystd::accumulate(std::next(v.begin()), v.end(),
                                    std::to_string(v[0]), // start with first element
                                    dash_fold);
 
    // Right fold using reverse iterators
    std::string rs = tinystd::accumulate(std::next(v.rbegin()), v.rend(),
                                     std::to_string(v.back()), // start with last element
                                     dash_fold);
 
    std::cout << "sum: " << sum << '\n'
              << "product: " << product << '\n'
              << "dash-separated string: " << s << '\n'
              << "dash-separated string (right-folded): " << rs << '\n';
}

auto print = [](auto comment, auto const& sequence) {
    std::cout << comment;
    for (const auto& n : sequence)
        std::cout << n << ' ';
    std::cout << '\n';
};

// Initially, v = 4 6 9 13 18 19 19 15 10
// Modified v = 4 2 3 4 5 1 0 -4 -5
// Fibonacci, a = 1 1 2 3 5 8 13 21 34 55
void __test_adjacent_difference() {
    // Default implementation - the difference b/w two adjacent items

    std::vector v{4, 6, 9, 13, 18, 19, 19, 15, 10};
    print("Initially, v = ", v);
    tinystd::adjacent_difference(v.begin(), v.end(), v.begin());
    print("Modified v = ", v);

    // Fibonacci

    std::array<int, 10> a{1};
    tinystd::adjacent_difference(begin(a), std::prev(end(a)), std::next(begin(a)), std::plus<>{});
    print("Fibonacci, a = ", a);
}

// Inner product of a and b: 21
// Number of pairwise matches between a and b: 2
void __test_inner_product() {
    tinystd::vector<int> a{0, 1, 2, 3, 4};
    tinystd::vector<int> b{5, 4, 2, 3, 1};
 
    int r1 = tinystd::inner_product(a.begin(), a.end(), b.begin(), 0);
    std::cout << "Inner product of a and b: " << r1 << '\n';
 
    int r2 = tinystd::inner_product(a.begin(), a.end(), b.begin(), 0,
                                tinystd::plus<int>(), tinystd::equal_to<int>());
    std::cout << "Number of pairwise matches between a and b: " <<  r2 << '\n';
}

// The first 10 even numbers are: 2 4 6 8 10 12 14 16 18 20 
// The first 10 powers of 2 are: 2 4 8 16 32 64 128 256 512 1024
void __test_partial_sum() {
    tinystd::vector<int> v = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}; // or std::vector<int>v(10, 2);
 
    std::cout << "The first 10 even numbers are: ";
    tinystd::partial_sum(v.begin(), v.end(), 
                     std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
 
    tinystd::partial_sum(v.begin(), v.end(), v.begin(), tinystd::multiplies<int>());
    std::cout << "The first 10 powers of 2 are: ";
    for (auto n : v) {
        std::cout << n << " ";
    }
    std::cout << '\n';
}

// Contents of the list: -4 -3 -2 -1 0 1 2 3 4 5
// Contents of the list, shuffled: 0 -1 3 4 -4 1 -2 -3 2 5
void __test_iota() {
    tinystd::list<int> l(10);
    tinystd::iota(l.begin(), l.end(), -4);
 
    tinystd::vector<tinystd::list<int>::iterator> v(l.size());
    std::iota(v.begin(), v.end(), l.begin());
 
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
 
    std::cout << "Contents of the list: ";
    for(auto n: l) std::cout << n << ' ';
    std::cout << '\n';
 
    std::cout << "Contents of the list, shuffled: ";
    for(auto i: v) std::cout << *i << ' ';
    std::cout << '\n';
}