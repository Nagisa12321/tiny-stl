#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "tinystl_numeric.h"
#include "tinystl_vector.h"
#include "tinystl_functional.h"

void __test_accmulate();
void __test_adjacent_difference();
void __test_inner_product();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test accmulate()...", __test_accmulate },
        { "test adjacent_difference()", __test_adjacent_difference },
        { "test inner_product()... ", __test_inner_product },
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