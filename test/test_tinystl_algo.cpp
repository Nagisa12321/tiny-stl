#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <string.h>
#include "tinystl_algo.h"

/*
v contains 3
v does not contain 5
*/
void test_find();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases {
        { "find", test_find },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << ">> " << __p.first << ": " << std::endl;
        __p.second();
    }
}

void test_find() {
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