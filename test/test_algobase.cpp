#include <iostream>
#include <vector>
#include <string_view>
#include <algorithm>
#include <iterator>
#include "tinystl_algobase.h"
#include "tinystl_vector.h"

void __test_equal();
void __test_fill();
void __test_fill_n();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test equal. ", __test_equal },
        { "test fill. ", __test_fill },
        { "test fill_n... ", __test_fill_n },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

bool is_palindrome(const std::string_view& s)
{
    return tinystd::equal(s.begin(), s.begin() + s.size()/2, s.rbegin());
}
 
void test(const std::string_view& s)
{
    std::cout << "\"" << s << "\" "
        << (is_palindrome(s) ? "is" : "is not")
        << " a palindrome\n";
}

// "radar" is a palindrome
// "hello" is not a palindrome 
void __test_equal() {
    test("radar");
    test("hello");
}

void __test_fill() {
    tinystd::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 
    tinystd::fill(v.begin(), v.end(), -1);
 
    for (auto elem : v) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

// -1 -1 -1 -1 -1 5 6 7 8 9 
void __test_fill_n() {
    tinystd::vector<int> v1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 
    tinystd::fill_n(v1.begin(), 5, -1);
 
    std::copy(std::begin(v1), std::end(v1), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
}