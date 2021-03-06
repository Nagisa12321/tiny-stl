#define __copy_test
#include <iostream>
#include <vector>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <random>
#include "tinystl_algobase.h"
#include "tinystl_vector.h"
#include "tinystl_numeric.h"
#include "tinystl_list.h"
#include "tinystl_deque.h"
#include "tinystl_string.h"
#include "__tinystl_utils.h"

void __test_equal();
void __test_fill();
void __test_fill_n();
void __test_iter_swap();
void __test_lexicographical_compare();
void __test_mismatch();
void __test_copy();
void __test_copy_backward();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test equal. ", __test_equal },
        { "test fill. ", __test_fill },
        { "test fill_n... ", __test_fill_n },
        { "test iter_swap...", __test_iter_swap },
        { "test lexicographical_compare...", __test_lexicographical_compare },
        { "test mismatch...", __test_mismatch },
        { "test copy...", __test_copy },
        { "test copy_backward...", __test_copy_backward },
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

void __test_lexicographical_compare() {
    std::vector<char> v1 {'a', 'b', 'c', 'd'};
    std::vector<char> v2 {'a', 'b', 'c', 'd'};
 
    std::mt19937 g{std::random_device{}()};
    while (!tinystd::lexicographical_compare(v1.begin(), v1.end(),
                                         v2.begin(), v2.end())) {
        for (auto c : v1) std::cout << c << ' ';
        std::cout << ">= ";
        for (auto c : v2) std::cout << c << ' ';
        std::cout << '\n';
 
        std::shuffle(v1.begin(), v1.end(), g);
        std::shuffle(v2.begin(), v2.end(), g);
    }
 
    for (auto c : v1) std::cout << c << ' ';
    std::cout << "< ";
    for (auto c : v2) std::cout << c << ' ';
    std::cout << '\n';
}

template<class ForwardIt>
void selection_sort(ForwardIt begin, ForwardIt end)
{
    for (ForwardIt i = begin; i != end; ++i)
        tinystd::iter_swap(i, std::min_element(i, end));
}

// Before sort: -9 -3 +2 -8 +0 -1 +8 -4 -5 +1 -4 -5 +4 -9 -8 -6 -6 +8 -4 -6 
// After sort : -9 -9 -8 -8 -6 -6 -6 -5 -5 -4 -4 -4 -3 -1 +0 +1 +2 +4 +8 +8
void __test_iter_swap() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-9, +9);
    std::vector<int> v;
    std::generate_n(back_inserter(v), 20, bind(dist, gen));
 
    std::cout << "Before sort: " << std::showpos;
    for(auto e : v) std::cout << e << ' ';
 
    selection_sort(v.begin(), v.end());
 
    std::cout << "\nAfter sort : ";
    for(auto e : v) std::cout << e << ' ';
    std::cout << '\n';
}

std::string mirror_ends(const std::string& in)
{
    return std::string(in.begin(),
                       tinystd::mismatch(in.begin(), in.end(), in.rbegin()).first);
}

// ab
// a
// aba
void __test_mismatch() {
    std::cout << mirror_ends("abXYZba") << '\n'
              << mirror_ends("abca") << '\n'
              << mirror_ends("aba") << '\n';
}

class C {
public:
    C() : _data(3) {}
private:
    int _data;
};

// to_vector contains: +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 
// odd numbers in to_vector are: +1 +3 +5 +7 +9 
// to_vector contains these multiples of 3:
// +0 +3 +6 +9
void __test_copy() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::vector<int> from_vector(10);
        tinystd::iota(from_vector.begin(), from_vector.end(), 0);
    
        tinystd::vector<int> to_vector;
        tinystd::copy(from_vector.begin(), from_vector.end(),
                  std::back_inserter(to_vector));
        // or, alternatively,
        //  std::vector<int> to_vector(from_vector.size());
        //  std::copy(from_vector.begin(), from_vector.end(), to_vector.begin());
        // either way is equivalent to
        //  std::vector<int> to_vector = from_vector;
    
        std::cout << "to_vector contains: ";
    
        tinystd::copy(to_vector.begin(), to_vector.end(),
                  std::ostream_iterator<int>(std::cout, " "));
        std::cout << '\n';
    
        std::cout << "odd numbers in to_vector are: ";
    
        std::copy_if(to_vector.begin(), to_vector.end(),
                     std::ostream_iterator<int>(std::cout, " "),
                     [](int x) { return x % 2 != 0; });
        std::cout << '\n';
    
        std::cout << "to_vector contains these multiples of 3:\n";
    
        to_vector.clear();
        std::copy_if(from_vector.begin(), from_vector.end(),
                     std::back_inserter(to_vector),
                     [](int x) { return x % 3 == 0; });
    
        for (int x : to_vector)
            std::cout << x << ' ';
        std::cout << '\n';
    }
    std::cout << "test2: " << std::endl;
    {
        std::cout << "(1) will call copy(const char *)" << std::endl;
        const char ccs[5] = { 'a', 'b', 'c', 'd', 'e', };
        char ccd[5];
        tinystd::copy(ccs, ccs + 5, ccd);
        std::cout << "after copy, ccd is ";
        __print_container(ccd, ccd + 5);

        std::cout << "\n(2) will call copy(const wchar_t *)" << std::endl;
        const wchar_t cwcs[5] = { 'a', 'b', 'c', 'd', 'e', };
        wchar_t cwcd[5];
        tinystd::copy(cwcs, cwcs + 5, cwcd);
        std::cout << "after copy, cwcd is ";
        __print_container(cwcd, cwcd + 5);

        std::cout << "\n(3) will call __copy_t(__true_type)" << std::endl;
        int ia[5] = { 0, 1, 2, 3, 4, };
        tinystd::copy(ia, ia + 5, ia);
        std::cout << "after copy, ia is "; 
        __print_container(ia, ia + 5);

        std::cout << "\n(4) will call __copy(__input_iter)" << std::endl;
        tinystd::list<int> ilists(ia, ia + 5);
        tinystd::list<int> ilistd(5);
        tinystd::copy(ilists.begin(), ilists.end(), ilistd.begin());
        std::cout << "after copy, ilistd is ";
        __print_container(ilistd.begin(), ilistd.end());

        std::cout << "\n(5) vector's iterator is a T*" << std::endl;
        tinystd::vector<int> ivecs(ia, ia + 5);
        tinystd::vector<int> ivecd(5);
        std::cout << "---" << std::endl;
        tinystd::copy(ivecs.begin(), ivecs.end(), ivecd.begin());
        std::cout << "after copy, ivecd is ";
        __print_container(ivecd.begin(), ivecd.end());

        std::cout << "\n(6) will call __copy_t(__false_type) " << std::endl;
        C c[5];
        tinystd::vector<C> cvecs(c, c + 5);
        std::cout << "---" << std::endl;
        tinystd::vector<C> cvecd(5);
        tinystd::copy(cvecs.begin(), cvecs.end(), cvecd.begin());

        std::cout << "\n(7) deque's iterator is random access iterator" << std::endl;
        tinystd::deque<C> cdeqs(c, c + 5);
        tinystd::deque<C> cdeqd(5);
        tinystd::copy(cdeqs.begin(), cdeqs.end(), cdeqd.begin());

        std::cout << "\n(8) string is false type!" << std::endl;
        tinystd::vector<tinystd::string> svecs(5);
        tinystd::vector<tinystd::string> svecd(5);
        svecs[0] = "jjhou";
        svecs[1] = "grace";
        svecs[2] = "david";
        svecs[3] = "jason";
        svecs[4] = "jerry";
        tinystd::copy(svecs.begin(), svecs.end(), svecd.begin());
        std::cout << "after copy, svecd is ";
        __print_container(svecd.begin(), svecd.end());

        std::cout << "\n(9) call random access iterator function... " << std::endl;
        tinystd::deque<tinystd::string> sdeqs;
        tinystd::deque<tinystd::string> sdeqd(5);
        sdeqs.push_back("cjt");
        sdeqs.push_back("807");
        sdeqs.push_back("916");
        tinystd::copy(sdeqs.begin(), sdeqs.end(), sdeqd.begin());
        std::cout << "after copy, sdeqd is ";
        __print_container(sdeqd.begin(), sdeqd.end());
    }
}

// to_vector contains: 0 0 0 0 0 0 1 2 3 4 5 6 7 8 9
void __test_copy_backward() {
    tinystd::vector<int> from_vector;
    for (int i = 0; i < 10; i++) {
        from_vector.push_back(i);
    }
 
    tinystd::vector<int> to_vector(15);
 
    tinystd::copy_backward(from_vector.begin(), from_vector.end(), to_vector.end());
 
    std::cout << "to_vector contains: ";
    for (auto i: to_vector) {
        std::cout << i << " ";
    }
}

#undef __copy_test