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
#include <algorithm>
#include "tinystl_iterator.h"

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
void __test_data();
void __test_c_str();
void __test_iterators();
void __test_empty();
void __test_reverve();
void __test_capacity();
void __test_shrink_to_fit(); 
void __test_clean();
void __test_insert();
void __test_erase();
void __test_add_eq_operator();
void __test_compare();
void __test_replace();
void __test_append();
void __test_substr();
void __test_copy();
void __test_resize();
void __test_swap();
void __test_find();
void __test_first_not_of();
void __test_first_of();
void __test_find_last_of();
void __test_find_last_not_of();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the std::string. ", __test_std_string },
        { "test init. ", __test_init },
        { "test constructor, ", __test_constructor },
        { "test push_back, ", __test_push_back },
        { "test operator+(), ", __test_add_operator },
        { "test operator=(), ", __test_assignment_operator },
        { "test assign() ", __test_assign },
        { "test at(), ", __test_at },
        { "test operator[]. ", __test_brackets_operator },
        { "test front() and back(). ", __test_front_and_back },
        { "test data(), ", __test_data },
        { "test c_str(), ", __test_c_str },
        { "test iterators, ", __test_iterators, },
        { "test empty(). ", __test_empty },
        { "test size and length, ", __test_size_length },
        { "test reserve(). ", __test_reverve },
        { "test capacity(), ", __test_capacity },
        { "test shrink_to_fit()", __test_shrink_to_fit },
        { "test clean(). ", __test_clean },
        { "test insert()... ", __test_insert },
        { "test erase()...", __test_erase },
        { "test operator+=()...", __test_add_eq_operator },
        { "test compare()... ", __test_compare }, 
        { "test replace()... ", __test_replace }, 
        { "test append()... ", __test_append }, 
        { "test substr()... ", __test_substr },
        { "test copy()... ", __test_copy },
        { "test resize()... ", __test_resize },
        { "test swap()... ", __test_swap }, 
        { "test find()... ", __test_find },
        { "test fitst_not_of()...", __test_first_not_of },
        { "test first_of()...", __test_first_of },
        { "test find_last_of()... ", __test_find_last_of },
        { "test find_last_not_of()", __test_find_last_not_of }

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
        std::cout << "\"" << s << "\"\n"; // "Mutandis", i.e. [8, 16)`
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
    tinystd::string s("Exemplar");
    assert(8 == s.size());
    assert(s.size() == s.length());
    assert(s.size() == static_cast<tinystd::string::size_type>(std::distance(s.begin(), s.end())));
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
    str1 = 35U;                                // equivalent to str1 = static_cast<char>(35U);
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
    } catch (std::out_of_range const &exc) {
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
        char &f = s.front();
        f = 'e';
        std::cout << s << '\n'; // "exemplary"
    }

    {
        tinystd::string const c("Exemplary");
        char const &f = c.front();
        std::cout << &f << '\n'; // "Exemplary"
    }
    {
        tinystd::string s("Exemplary");
        char &back = s.back();
        back = 's';
        std::cout << s << '\n'; // "Exemplars"
    }

    {
        tinystd::string const c("Exemplary");
        char const &back = c.back();
        std::cout << back << '\n'; // 'y'
    }
}

void __test_data() {
    std::string const s("Emplary");
    assert(s.size() == std::strlen(s.data()));
    assert(std::equal(s.begin(), s.end(), s.data()));
    assert(std::equal(s.data(), s.data() + s.size(), s.begin()));
    assert(0 == *(s.data() + s.size()));
}

void __test_c_str() {
    std::string const s("Emplary");
    const char *p = s.c_str();
    assert(s.size() == std::strlen(p));
    assert(std::equal(s.begin(), s.end(), p));
    assert(std::equal(p, p + s.size(), s.begin()));
    assert(0 == *(p + s.size()));
}

//
// exemplar
// e
// Exemplar
//
void __test_iterators() {
    tinystd::string s("Exemplar");
    *s.begin() = 'e';
    std::cout << s << '\n';

    auto i = s.cbegin();
    std::cout << *i << '\n';
    //  *i = 'E'; // error: i is a constant iterator()

    tinystd::string s1("Exemparl");
    std::next_permutation(s1.begin(), s1.end());

    tinystd::string c;
    std::copy(s1.cbegin(), s1.cend(), std::back_inserter(c));
    std::cout << c << '\n'; // "Exemplar"
}

void __test_empty() {
    tinystd::string s;
    std::boolalpha(std::cout);
    std::cout << "s.empty():" << s.empty() << "\t s:'" << s << "'\n";
 
    s = "Exemplar";
    std::cout << "s.empty():" << s.empty() << "\t s:'" << s << "'\n";
 
    s = "";
    std::cout << "s.empty():" << s.empty() << "\t s:'" << s << "'\n";
}

void __test_reverve() {
    tinystd::string s("hello world!");
    std::cout << s << std::endl;
    tinystd::string::size_type new_capacity{100u};
    assert(new_capacity > s.capacity());

    s.reserve(new_capacity);
    assert(new_capacity <= s.capacity());
    std::cout << s << std::endl;
}

void show_capacity(tinystd::string const& s) {
    std::cout << "'" << s << "' has capacity " << s.capacity() << ".\n";
}
 
void __test_capacity() {
    tinystd::string s{"Exemplar"};
    show_capacity(s);
 
    s += " is an example string.";
    show_capacity(s);
}

void __test_shrink_to_fit() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::string s("hello world!");
        std::cout << s << std::endl;
        s += " let me have a look... ";
        std::cout << s << std::endl;
        s = "hi";
        std::cout << s << std::endl;
    }
    std::cout << "test2: " << std::endl;
    {
        tinystd::string s;
        std::cout << "Size of std::string is " << sizeof s << " bytes\n"
                  << "Default-constructed capacity is " << s.capacity()
                  << " and size is " << s.size() << '\n';
        for (int i = 0; i < 42; i++)
            s.append(" 42 ");
        std::cout << "Capacity after 42 appends is " << s.capacity()
                  << " and size is " << s.size() << '\n';
        s.clear();
        std::cout << "Capacity after clear() is " << s.capacity()
                  << " and size is " << s.size() << '\n';
        s.shrink_to_fit();
        std::cout << "Capacity after shrink_to_fit() is " << s.capacity()
                  << " and size is " << s.size() << '\n';
    }
    std::cout << "test3: " << std::endl; 
    {
        tinystd::string s(100, 'a');
        std::cout << s << std::endl;
        std::cout << "size: " << s.size() << ", capacity: " << s.capacity() << std::endl;
        s = tinystd::string(50, '#');
        std::cout << s << std::endl;
        std::cout << "size: " << s.size() << ", capacity: " << s.capacity() << std::endl;
        s.shrink_to_fit();
        std::cout << s << std::endl;
        std::cout << "size: " << s.size() << ", capacity: " << s.capacity() << std::endl;

    }
}

void __test_clean() {
    tinystd::string s{"Exemplar"};
    tinystd::string::size_type const capacity = s.capacity();

    s.clear();
    assert(s.capacity() == capacity); // <- not guaranteed
    assert(s.empty());
    assert(s.size() == 0);
}

void __test_insert() {
    tinystd::string s = "xmplr";

    // insert(size_type index, size_type count, char ch)
    s.insert((int) 0, 1, 'E');
    assert(tinystd::string("Exmplr") == s);

    // insert(size_type index, const char* s)
    s.insert(2, "e");
    assert(tinystd::string("Exemplr") == s);

    // insert(size_type index, string const& str)
    s.insert(6, tinystd::string("a"));
    assert(tinystd::string("Exemplar") == s);

    // insert(size_type index, string const& str,
    //     size_type index_str, size_type count)
    s.insert(8, tinystd::string(" is an example string."), 0, 14);
    assert(tinystd::string("Exemplar is an example") == s);

    // insert(const_iterator pos, char ch)
    s.insert(s.cbegin() + s.find_first_of('n') + 1, ':');
    assert(tinystd::string("Exemplar is an: example") == s);

    // insert(const_iterator pos, size_type count, char ch)
    s.insert(s.cbegin() + s.find_first_of(':') + 1, 2, '=');
    assert(tinystd::string("Exemplar is an:== example") == s);

    // insert(const_iterator pos, InputIt first, InputIt last)
    {
        tinystd::string seq = " string";
        s.insert(s.begin() + s.find_last_of('e') + 1,
                 std::begin(seq), std::end(seq));
        assert(tinystd::string("Exemplar is an:== example string") == s);
    }

    // insert(const_iterator pos, std::initializer_list<char>)
    s.insert(s.cbegin() + s.find_first_of('g') + 1, {'.'});
    assert(tinystd::string("Exemplar is an:== example string.") == s);
}

// 1) This Is An Example
// 2) This Is Example
// 3) ThisIs Example
// 4) ThisIs
// 5) This
void __test_erase() {
    tinystd::string s = "This Is An Example";
    std::cout << "1) " << s << '\n';

    s.erase(7, 3); // erases " An" using overload (1)
    std::cout << "2) " << s << '\n';

    s.erase(std::find(s.begin(), s.end(), ' ')); // erases first ' '; overload (2)
    std::cout << "3) " << s << '\n';

    s.erase(s.find(' ')); // trims from ' ' to the end of the string; overload (1)
    std::cout << "4) " << s << '\n';

    auto it = tinystd::next(s.begin(), s.find('s')); // obtains iterator to the first 's'
    s.erase(it, std::next(it, 2));               // erases "sI"; overload (3)
    std::cout << "5) " << s << '\n';
}
// ""
// "This"
// "This is "
// "This is a"
// "This is a string."
// "This is a string.L"
void __test_add_eq_operator() {
    tinystd::string str;
    str.reserve(50);                       // reserves sufficient storage space to avoid memory reallocation
    std::cout << "\"" << str << "\"" << '\n'; // empty string

    str += "This";
    std::cout << "\"" << str << "\"" << '\n';

    str += tinystd::string(" is ");
    std::cout << "\"" << str << "\"" << '\n';

    str += 'a';
    std::cout << "\"" << str << "\"" << '\n';

    str += {' ', 's', 't', 'r', 'i', 'n', 'g', '.'};
    std::cout << "\"" << str << "\"" << '\n';

    str += 76.85; // equivalent to str += static_cast<char>(76.85), might not be the intent
    std::cout << "\"" << str << "\"" << '\n';
}
// Batman comes before Superman
// Superman comes before man
// man and man are the same.
// Batman comes before Superman
// Superman comes before man
// Bat comes before Super
void __test_compare() {
    // 1) Compare with other string
    {
        int compare_value{
            tinystd::string{"Batman"}.compare(tinystd::string{"Superman"})};
        std::cout << (compare_value < 0 ? "Batman comes before Superman\n" :
                      compare_value > 0 ? "Superman comes before Batman\n" :
                                          "Superman and Batman are the same.\n");
    }

    // 2) Compare substring with other string
    {
        int compare_value{
            tinystd::string{"Batman"}.compare(3, 3, tinystd::string{"Superman"})};
        std::cout << (compare_value < 0 ? "man comes before Superman\n" :
                      compare_value > 0 ? "Superman comes before man\n" :
                                          "man and Superman are the same.\n");
    }

    // 3) Compare substring with other substring
    {
        tinystd::string a{"Batman"};
        tinystd::string b{"Superman"};

        int compare_value{a.compare(3, 3, b, 5, 3)};

        std::cout << (compare_value < 0 ? "man comes before man\n" :
                      compare_value > 0 ? "man comes before man\n" :
                                          "man and man are the same.\n");
        // Compare substring with other substring
        // defaulting to end of other string
        assert(compare_value == a.compare(3, 3, b, 5));
    }

    // 4) Compare with char pointer
    {
        int compare_value{tinystd::string{"Batman"}.compare("Superman")};

        std::cout << (compare_value < 0 ? "Batman comes before Superman\n" :
                      compare_value > 0 ? "Superman comes before Batman\n" :
                                          "Superman and Batman are the same.\n");
    }

    // 5) Compare substring with char pointer
    {
        int compare_value{tinystd::string{"Batman"}.compare(3, 3, "Superman")};

        std::cout << (compare_value < 0 ? "man comes before Superman\n" :
                      compare_value > 0 ? "Superman comes before man\n" :
                                          "man and Superman are the same.\n");
    }

    // 6) Compare substring with char pointer substring
    {
        int compare_value{tinystd::string{"Batman"}.compare(0, 3, "Superman", 5)};

        std::cout << (compare_value < 0 ? "Bat comes before Super\n" :
                      compare_value > 0 ? "Super comes before Bat\n" :
                                          "Super and Bat are the same.\n");
    }
}

tinystd::size_t replace_all(tinystd::string& inout, std::string_view what, std::string_view with);
tinystd::size_t remove_all(tinystd::string& inout, std::string_view what);
void test_replace_remove_all();
// A quick red fox jumps over the lazy dog.
// 
// #1 ftp: ftpftp: ftp:
// #2 http: httphttp: http:
// #3 http: httphttp: http:
// #4 : : :
void __test_replace() {
    tinystd::string str{"The quick brown fox jumps over the lazy dog."};

    str.replace(10, 5, "red"); // (5)

    str.replace(str.begin(), str.begin() + 3, 1, 'A'); // (6)

    std::cout << str << "\n\n";

    test_replace_remove_all();
}

tinystd::size_t replace_all(tinystd::string &inout, std::string_view what, std::string_view with) {
    tinystd::size_t count{};
    for (tinystd::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}   

tinystd::size_t remove_all(tinystd::string &inout, std::string_view what) {
    return replace_all(inout, what, "");
}

void test_replace_remove_all() {
    tinystd::string str2{"ftp: ftpftp: ftp:"};
    std::cout << "#1 " << str2 << '\n';

    auto count = replace_all(str2, "ftp", "http");
    assert(count == 4);
    std::cout << "#2 " << str2 << '\n';

    count = replace_all(str2, "ftp", "http");
    assert(count == 0);
    std::cout << "#3 " << str2 << '\n';

    count = remove_all(str2, "http");
    assert(count == 4);
    std::cout << "#4 " << str2 << '\n';
}

// 1) ***
// 2) ***string
// 3) ***stringing
// 4) ***stringing Two 
// 5) ***stringing Two C-string
// 6) ***stringing Two C-string and one
// 7) ***stringing Two C-string and one list
// 
void __test_append() {
    tinystd::basic_string<char> str = "string";
    const char *cptr = "C-string";
    const char carr[] = "Two and one";

    tinystd::string output;

    // 1) Append a char 3 times.
    // Notice, this is the only overload accepting chars.
    output.append(3, '*');
    std::cout << "1) " << output << "\n";

    //  2) Append a whole string
    output.append(str);
    std::cout << "2) " << output << "\n";

    // 3) Append part of a string (last 3 letters, in this case)
    output.append(str, 3, 3);
    std::cout << "3) " << output << "\n";

    // 4) Append part of a C-string
    // Notice, because `append` returns *this, we can chain calls together
    output.append(1, ' ').append(carr, 4);
    std::cout << "4) " << output << "\n";

    // 5) Append a whole C-string
    output.append(cptr);
    std::cout << "5) " << output << "\n";

    // 6) Append range
    output.append(&carr[3], std::end(carr) - 1);
    std::cout << "6) " << output << "\n";

    // 7) Append initializer list
    output.append({' ', 'l', 'i', 's', 't'});
    std::cout << "7) " << output << "\n";
}

// abcdefghij
// 567
// hij
// pos exceeds string size
void __test_substr() {
    tinystd::string a = "0123456789abcdefghij";

    // count is npos, returns [pos, size())
    tinystd::string sub1 = a.substr(10);
    std::cout << sub1 << '\n';

    // both pos and pos+count are within bounds, returns [pos, pos+count)
    tinystd::string sub2 = a.substr(5, 3);
    std::cout << sub2 << '\n';

    // pos is within bounds, pos+count is not, returns [pos, size())
    tinystd::string sub4 = a.substr(a.size() - 3, 50);
    // this is effectively equivalent to
    // std::string sub4 = a.substr(17, 3);
    // since a.size() == 20, pos == a.size()-3 == 17, and a.size()-pos == 3

    std::cout << sub4 << '\n';

    try {
        // pos is out of bounds, throws
        tinystd::string sub5 = a.substr(a.size() + 3, 50);
        std::cout << sub5 << '\n';
    } catch (const std::out_of_range &e) {
        std::cout << "pos exceeds string size\n";
    }
}

void __test_copy() {
    tinystd::string foo("quuuux");
    char bar[7]{};
    foo.copy(bar, sizeof bar);
    std::cout << bar << '\n';
}

// Basic functionality:
// Shorten:
// 1. Before: "Where is the end??????????????"
// 2. After:  "Where is"
// Lengthen:
// 3. Before: "Ha"
// 4. After:  "Haaaaaaa"
// 
// Errors:
// 1. Exception: std::bad_alloc
// 2. Exception: std::bad_alloc
// 3. Length error: basic_string::_M_replace_aux
void __test_resize() {
    const unsigned desired_length{8};
    tinystd::string long_string("Where is the end??????????????");
    tinystd::string short_string("Ha");

    std::cout << "Basic functionality:\n"
              << "Shorten:\n"
              << "1. Before: " << "\"" << long_string << "\"" << '\n';

    long_string.resize(desired_length);

    std::cout << "2. After:  " << "\"" << long_string << "\"" << '\n';

    std::cout << "Lengthen:\n"
              << "3. Before: " << "\"" << short_string << "\"" << '\n';

    short_string.resize(desired_length, 'a');

    std::cout << "4. After:  " << "\"" << short_string << "\"" << "\n\n";

    std::cout << "Errors:\n";
    {
        tinystd::string s;

        try {
            // size is OK, no length_error
            // (may throw bad_alloc)
            s.resize(s.max_size() - 1, 'x');
        } catch (const std::bad_alloc &ex) {
            std::cout << "1. Exception: " << ex.what() << '\n';
        }

        try {
            // size is OK, no length_error
            // (may throw bad_alloc)
            s.resize(s.max_size(), 'x');
        } catch (const std::bad_alloc &ex) {
            std::cout << "2. Exception: " << ex.what() << '\n';
        }

        try {
            // size is BAD, throw length_error
            s.resize(s.max_size() + 1, 'x');
        } catch (const std::length_error &ex) {
            std::cout << "3. Length error: " << ex.what() << '\n';
        }
    }
}
// before swap
// a: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
// b: bbbbb
// after swap
// a: bbbbb
// b: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
void __test_swap() {
    tinystd::string a(50, 'A');
    tinystd::string b(5, 'b');

    std::cout << "before swap" << '\n';
    std::cout << "a: " << a << '\n';
    std::cout << "b: " << b << '\n';

    a.swap(b);

    std::cout << "after swap" << '\n';
    std::cout << "a: " << a << '\n';
    std::cout << "b: " << b << '\n';
}

void print(tinystd::string::size_type n, tinystd::string const &s) {
    if (n == std::string::npos) {
        std::cout << "not found\n";
    } else {
        std::cout << "found: " << s.substr(n) << '\n';
    }
}
// 
// >> test find()...  ========================
// found: is is a string
// found: is a string
// found: a string
// not found
// 
void __test_find() {
    tinystd::string::size_type n;
    tinystd::string const s = "This is a string";

    // search from beginning of string
    n = s.find("is");
    print(n, s);

    // search from position 5
    n = s.find("is", 5);
    print(n, s);

    // find a single character
    n = s.find('a');
    print(n, s);

    // find a single character
    n = s.find('q');
    print(n, s);
}

// Before: Some data with %MACROS to substitute
// After: Some data with some very nice macros to substitute
void __test_first_not_of() {
    tinystd::string to_search = "Some data with %MACROS to substitute";

    std::cout << "Before: " << to_search << '\n';

    auto pos = tinystd::string::npos;
    while ((pos = to_search.find('%')) != std::string::npos) {
        // Permit uppercase letters, lowercase letters and numbers in macro names
        const auto after = to_search.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                       "abcdefghijklmnopqrstuvwxyz"
                                                       "0123456789",
                                                       pos + 1);

        // Now to_search[pos] == '%' and to_search[after] == ' ' (after the 'S')

        if (after != tinystd::string::npos)
            to_search.replace(pos, after - pos, "some very nice macros");
    }

    std::cout << "After: " << to_search << '\n';
}
// 4
// 7
// 1
// 4
// 18446744073709551615
void __test_first_of() {
    // the test string
    tinystd::string str = tinystd::string("Hello World!");

    // strings and chars to search for
    tinystd::string search_str = tinystd::string("o");
    const char *search_cstr = "Good Bye!";

    std::cout << str.find_first_of(search_str) << '\n';
    std::cout << str.find_first_of(search_str, 5) << '\n';
    std::cout << str.find_first_of(search_cstr) << '\n';
    std::cout << str.find_first_of(search_cstr, 0, 4) << '\n';
    // 'x' is not in "Hello World', thus it will return std::string::npos
    std::cout << str.find_first_of('x') << '\n';
}

void __test_find_last_of() {
    const tinystd::string path = "/root/config";
    auto const pos = path.find_last_of('/');
    const auto leaf = path.substr(pos + 1);

    std::cout << leaf << '\n';
}

void show_pos(const tinystd::string &str, tinystd::string::size_type found) {
    if (found != tinystd::string::npos) {
        std::cout << "[" << found << "] = \'" << str[found] << "\'\n";
    } else {
        std::cout << "not found"
                     "\n";
    }
}
// [3] = '_'
// [2] = 'c'
// [1] = 'b'
// [5] = '2'
// [2] = 'c'
// [6] = '3'
// not found
void __test_find_last_not_of() {
    tinystd::string str{"abc_123"};
    char const *skip_set{"0123456789"};
    tinystd::string::size_type str_last_pos{tinystd::string::npos};

    show_pos(str, str.find_last_not_of(skip_set)); // [3] = '_'

    str_last_pos = 2;
    show_pos(str, str.find_last_not_of(skip_set, str_last_pos)); // [2] = 'c'

    str_last_pos = 2;
    show_pos(str, str.find_last_not_of('c', str_last_pos)); // [1] = 'b'

    const char arr[]{'3', '4', '5'};
    show_pos(str, str.find_last_not_of(arr)); // [5] = '2'

    str_last_pos = 2;
    tinystd::string::size_type skip_set_size{4};
    show_pos(str, str.find_last_not_of(skip_set,
                                       str_last_pos,
                                       skip_set_size)); // [2] = 'c'

    show_pos(str, str.find_last_not_of("abc")); // [6] = '3'

    str_last_pos = 2;
    show_pos(str, str.find_last_not_of("abc", str_last_pos)); // not found
}