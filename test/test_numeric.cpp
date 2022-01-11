#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "tinystl_numeric.h"

void __test_accmulate();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test accmulate()...", __test_accmulate },
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