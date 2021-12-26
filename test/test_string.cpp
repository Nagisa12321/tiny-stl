#include "tinystl_string.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <cassert>

void __test_std_string();
void __test_init();
void __test_constructor();
void __test_size_length(); 
void __test_push_back();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the std::string. ", __test_std_string },
        { "test init. ", __test_init },
        { "test constructor, ", __test_constructor },
        { "test size and length, ", __test_size_length },
        { "test push_back, ", __test_push_back },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

void __test_std_string() {
    std::cout << "test1: size of string. " << std::endl;
    {
        std::cout << "size of string is " << sizeof(std::string) << std::endl;
    }
}

void __test_init() {
    std::cout << "\ntest1: test debug message. " << std::endl;
    {
        tinystd::string __str;
        __str.__debug_message();
    }
    std::cout << "\ntest2: test init with const char. " << std::endl;
    {
        tinystd::string __s1("hello");
        std::cout << __s1 << std::endl;

        tinystd::string __s2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        std::cout << __s2 << std::endl;
    }
}

/**
 * @brief
 * 1) string(); s.capacity(): 22
 * 2) string(size_type count, charT ch): "===="
 * 3) string(const string& other, size_type pos, size_type count): "Exemplar"
 * 4) string(const string& other, size_type pos): "Mutandis"
 * 5) string(charT const* s, size_type count): "C-style"
 * 6) string(charT const* s): "C-style"
 * 7) string(InputIt first, InputIt last): "C-style string"
 * 8) string(string&): "Exemplar"
 * 9) string(string&&): "C++ by example"
 * α) string(std::initializer_list<charT>): "C-style"
 * β) string(size_type count, charT ch) is called: "AAA"
 */

void __test_constructor() {
    {
        std::cout << "1) string(); ";
        tinystd::string s;
        assert(s.empty() && (s.length() == 0) && (s.size() == 0));
        std::cout << "s.capacity(): " << s.capacity() << '\n'; // unspecified
    }

    {
        std::cout << "2) string(size_type count, charT ch): ";
        std::string s(4, '=');
        std::cout << std::quoted(s) << '\n'; // "===="
    }
    {
        std::cout << "3) string(const string& other, size_type pos, size_type count): ";
        std::string const other("Exemplary");
        std::string s(other, 0, other.length() - 1);
        std::cout << quoted(s) << '\n'; // "Exemplar"
    }

    {
        std::cout << "4) string(const string& other, size_type pos): ";
        std::string const other("Mutatis Mutandis");
        std::string s(other, 8);
        std::cout << quoted(s) << '\n'; // "Mutandis", i.e. [8, 16)
    }

    {
        std::cout << "5) string(charT const* s, size_type count): ";
        std::string s("C-style string", 7);
        std::cout << quoted(s) << '\n'; // "C-style", i.e. [0, 7)
    }

    {
        std::cout << "6) string(charT const* s): ";
        std::string s("C-style\0string");
        std::cout << quoted(s) << '\n'; // "C-style"
    }

    {
        std::cout << "7) string(InputIt first, InputIt last): ";
        char mutable_c_str[] = "another C-style string";
        std::string s(std::begin(mutable_c_str) + 8, std::end(mutable_c_str) - 1);
        std::cout << quoted(s) << '\n'; // "C-style string"
    }

    {
        std::cout << "8) string(string&): ";
        std::string const other("Exemplar");
        std::string s(other);
        std::cout << quoted(s) << '\n'; // "Exemplar"
    }

    {
        std::cout << "9) string(string&&): ";
        std::string s(std::string("C++ by ") + std::string("example"));
        std::cout << quoted(s) << '\n'; // "C++ by example"
    }

    {
        std::cout << "α) string(std::initializer_list<charT>): ";
        std::string s({'C', '-', 's', 't', 'y', 'l', 'e'});
        std::cout << quoted(s) << '\n'; // "C-style"
    }

    {
        // before C++11, overload resolution selects string(InputIt first, InputIt last)
        // [with InputIt = int] which behaves *as if* string(size_type count, charT ch)
        // after C++11 the InputIt constructor is disabled for integral types and calls:
        std::cout << "β) string(size_type count, charT ch) is called: ";
        std::string s(3, std::toupper('a'));
        std::cout << quoted(s) << '\n'; // "AAA"
    }
}

void __test_size_length() {
    // tinystd::string s("Exemplar");
    // assert(8 == s.size());
    // assert(s.size() == s.length());
    // assert(s.size() == static_cast<tinystd::string::size_type>(
    //     std::distance(s.begin(), s.end())));
}

void __test_push_back() {
    std::cout << "test1: test two tansfrom of push back " << std::endl;
    {
        tinystd::string __s;
        for (char c = 'a'; c <= 'z'; ++c) {
            for (int i = 0; i < 5; ++i) {
                __s.push_back(c);
                std::cout << __s << std::endl;
            }
        }
    }
}