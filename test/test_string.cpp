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
void __test_add_operator();
void __test_assignment_operator();
void __test_assign();
void __test_at();
void __test_brackets_operator();
void __test_front_and_back();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the std::string. ", __test_std_string },
        { "test init. ", __test_init },
        { "test constructor, ", __test_constructor },
        { "test size and length, ", __test_size_length },
        { "test push_back, ", __test_push_back },
        { "test operator+(), ", __test_add_operator },
        { "test operator=(), ", __test_assignment_operator },
        { "test assign() ", __test_assign },
        { "test at(), ", __test_at },
        { "test operator[]. ", __test_brackets_operator },
        { "test front() and back(). ", __test_front_and_back },
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
        tinystd::string s(4, '=');
        std::cout << "\"" << s << "\"\n"; // "===="
    }
    {
        std::cout << "3) string(const string& other, size_type pos, size_type count): ";
        tinystd::string const other("Exemplary");
        tinystd::string s(other, 0, other.length() - 1);
        std::cout << "\"" << s << "\"\n"; // "Exemplar"
    }

    {
        std::cout << "4) string(const string& other, size_type pos): ";
        tinystd::string const other("Mutatis Mutandis");
        tinystd::string s(other, 8);
        std::cout << "\"" << s << "\"\n";  // "Mutandis", i.e. [8, 16)`
    }

    {
        std::cout << "5) string(charT const* s, size_type count): ";
        tinystd::string s("C-style string", 7);
        std::cout << "\"" << s << "\"\n"; // "C-style", i.e. [0, 7)
    }

    {
        std::cout << "6) string(charT const* s): ";
        tinystd::string s("C-style\0string");   
        std::cout << "\"" << s << "\"\n"; // "C-style"
    }

    {
        std::cout << "7) string(InputIt first, InputIt last): ";
        char mutable_c_str[] = "another C-style string";
        tinystd::string s(std::begin(mutable_c_str) + 8, std::end(mutable_c_str) - 1);
        std::cout << "\"" << s << "\"\n"; // "C-style string"
    }

    {
        std::cout << "8) string(string&): ";
        tinystd::string const other("Exemplar");
        tinystd::string s(other);
        std::cout << "\"" << s << "\"\n"; // "Exemplar"
    }

    {
        std::cout << "9) string(string&&): ";
        tinystd::string s(tinystd::string("C++ by ") + tinystd::string("example"));
        std::cout << "\"" << s << "\"\n"; // "C++ by example"
    }

    {
        std::cout << "α) string(std::initializer_list<charT>): ";
        tinystd::string s({'C', '-', 's', 't', 'y', 'l', 'e'});
        std::cout << "\"" << s << "\"\n"; // "C-style"
    }

    {
        // before C++11, overload resolution selects string(InputIt first, InputIt last)
        // [with InputIt = int] which behaves *as if* string(size_type count, charT ch)
        // after C++11 the InputIt constructor is disabled for integral types and calls:
        std::cout << "β) string(size_type count, charT ch) is called: ";
        tinystd::string s(3, std::toupper('a'));
        std::cout << "\"" << s << "\"\n"; // "AAA"
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
            for (int i = 0; i < 2; ++i) {
                __s.push_back(c);
                std::cout << __s << std::endl;
            }
        }
    }
}

void __test_add_operator() {
    tinystd::string s1 = "Hello";
    tinystd::string s2 = "world";
    std::cout << s1 + ' ' + s2 + "!\n";
}

/**
 * @brief 
 * "alpha" "alpha"
 * "alpha" ""
 * "beta"
 * "!"
 * "gamma"
 * "#"
 *  
 */
void __test_assignment_operator() {
    tinystd::string str1;
    tinystd::string str2{"alpha"};

    // (1) operator=( const basic_string& );
    str1 = str2;
    std::cout << "\"" << str1 << "\"" << ' '   // "alpha"
              << "\"" << str2 << "\"" << '\n'; // "alpha"

    // (2) operator=( basic_string&& );
    str1 = std::move(str2);
    std::cout << "\"" << str1 << "\"" << ' '   // "alpha"
              << "\"" << str2 << "\"" << '\n'; // "alpha"


    // (3) operator=( const CharT* );
    str1 = "beta";
    std::cout << "\"" << str1 << "\"" << '\n'; // "beta"

    // (4) operator=( CharT );
    str1 = '!';
    std::cout << "\"" << str1 << "\"" << '\n'; // "!"

    // (5) operator=( std::initializer_list<CharT> );
    str1 = {'g', 'a', 'm', 'm', 'a'};
    std::cout << "\"" << str1 << "\"" << '\n'; // "gamma"

    // (6) operator=( const T& );
    str1 = 35U;                             // equivalent to str1 = static_cast<char>(35U);
    std::cout << "\"" << str1 << "\"" << '\n'; // "#" (ASCII = 35)
}

void __test_assign() {
    tinystd::string s;
    // assign(size_type count, CharT ch)
    s.assign(4, '=');
    std::cout << s << '\n'; // "===="

    tinystd::string const c("Exemplary");
    // assign(basic_string const& str)
    s.assign(c);
    std::cout << c << " == " << s << '\n'; // "Exemplary == Exemplary"

    // assign(basic_string const& str, size_type pos, size_type count)
    s.assign(c, 0, c.length() - 1);
    std::cout << s << '\n'; // "Exemplar";

    // assign(basic_string&& str)
    s.assign(tinystd::string("C++ by ") + "example");
    std::cout << s << '\n'; // "C++ by example"

    // assign(charT const* s, size_type count)
    s.assign("C-style string", 7);
    std::cout << s << '\n'; // "C-style"

    // assign(charT const* s)
    s.assign("C-style\0string");
    std::cout << s << '\n'; // "C-style"

    char mutable_c_str[] = "C-style string";
    // assign(InputIt first, InputIt last)
    s.assign(std::begin(mutable_c_str), std::end(mutable_c_str) - 1);
    std::cout << s << '\n'; // "C-style string"

    // assign(std::initializer_list<charT> ilist)
    s.assign({'C', '-', 's', 't', 'y', 'l', 'e'});
    std::cout << s << '\n'; // "C-style"
}

void __test_at() {
    tinystd::string s("message"); // for capacity
 
    s = "abc";
    s.at(2) = 'x'; // ok
    std::cout << s << '\n';
 
    std::cout << "string size = " << s.size() << '\n';
    std::cout << "string capacity = " << s.capacity() << '\n';
 
    try {
        // This will throw since the requested offset is greater than the current size.
        s.at(3) = 'x';
    }
    catch (std::out_of_range const& exc) {
        std::cout << exc.what() << '\n';
    }
}

void __test_brackets_operator() {
    tinystd::string const e("Exemplar");
    for (unsigned i = e.length() - 1; i != 0; i /= 2)
        std::cout << e[i];
    std::cout << '\n';

    const char *c = &e[0];
    std::cout << c << '\n'; // print as a C string

    // Change the last character of s into a 'y'
    std::string s("Exemplar ");
    s[s.size() - 1] = 'y'; // equivalent to s.back() = 'y';
    std::cout << s << '\n';
}

void __test_front_and_back() {
  {
    tinystd::string s("Exemplary");
    char& f = s.front();
    f = 'e';
    std::cout << s << '\n'; // "exemplary"
  }
 
  {
    tinystd::string const c("Exemplary");
    char const& f = c.front();
    std::cout << &f << '\n'; // "Exemplary"
  }
    {
    tinystd::string s("Exemplary");
    char& back = s.back();
    back = 's';
    std::cout << s << '\n'; // "Exemplars"
  }
 
  {
    tinystd::string const c("Exemplary");
    char const& back = c.back();
    std::cout << back << '\n'; // 'y'
  }
}
