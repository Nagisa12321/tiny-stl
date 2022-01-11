#include <iostream>
#include <vector>
#include <string_view>
#include <algorithm>
#include "tinystl_algobase.h"

void __test_equal();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test equal. ", __test_equal },
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