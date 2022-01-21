#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <cmath>
#include <cassert>

#include <tinystl_set.h>
#include <tinystl_iterator.h>

void __test_constructor();
void __test_operator_eq();
void __test_begin_end();
void __test_empty();
void __test_size();
void __test_max_size();
void __test_clear();
void __test_insert();
void __test_swap();

int main() {
    srand(time(0x0));

    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test constructor...", __test_constructor },
        { "test operator= ...", __test_operator_eq },
        { "test begin() ... ", __test_begin_end },
        { "test empty() ... ", __test_empty },
        { "test size() ... ", __test_size },
        { "test max_size() ... ", __test_max_size },
        { "test clear() ... ", __test_clear },
        { "test insert() ... ", __test_insert },
        { "test swap() ... ", __test_swap },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

struct Point { double x, y; };
struct PointCmp {
    bool operator()(const Point& lhs, const Point& rhs) const { 
        return std::hypot(lhs.x, lhs.y) < std::hypot(rhs.x, rhs.y); 
    }
};

// cat dog horse
// dog horse
// another horse cat dog horse
// cat dog horse
// moved-from set is
// eight five one three two
// (1,1) (3,4) (2,5)
void __test_constructor() {
    // (1) Default constructor
    tinystd::set<std::string> a;
    a.insert("cat");
    a.insert("dog");
    a.insert("horse");
    for(auto& str: a) std::cout << str << ' ';
    std::cout << '\n';
 
    // (2) Iterator constructor
    tinystd::set<std::string> b(a.find("dog"), a.end());
    for(auto& str: b) std::cout << str << ' ';
    std::cout << '\n';
 
    // (3) Copy constructor
    tinystd::set<std::string> c(a);
    c.insert("another horse");
    for(auto& str: c) std::cout << str << ' ';
    std::cout << '\n';
 
    // (4) Move constructor
    tinystd::set<std::string> d(std::move(a));
    for(auto& str: d) std::cout << str << ' ';
    std::cout << '\n';
    std::cout << "moved-from set is ";
    for(auto& str: a) std::cout << str << ' ';
    std::cout << '\n';
 
    // (5) Initializer list constructor
    tinystd::set<std::string> e {"one", "two", "three", "five", "eight"};
    for(auto& str: e) std::cout << str << ' ';
    std::cout << '\n';
 
    // custom comparison
    tinystd::set<Point, PointCmp> z = {{2, 5}, {3, 4}, {1, 1}};
    z.insert({1, -1}); // this fails because the magnitude of 1,-1 equals 1,1
    for(auto& p: z) std::cout << '(' << p.x << ',' << p.y << ") ";
    std::cout << '\n';
}

void display_sizes(const tinystd::set<int> &nums1,
                   const tinystd::set<int> &nums2,
                   const tinystd::set<int> &nums3)
{
    std::cout << "nums1: " << nums1.size() 
              << " nums2: " << nums2.size()
              << " nums3: " << nums3.size() << '\n';
}
 
void __test_operator_eq()
{
    tinystd::set<int> nums1 {3, 1, 4, 6, 5, 9};
    tinystd::set<int> nums2; 
    tinystd::set<int> nums3;
 
    std::cout << "Initially:\n";
    display_sizes(nums1, nums2, nums3);
 
    // copy assignment copies data from nums1 to nums2
    nums2 = nums1;
 
    std::cout << "After assigment:\n"; 
    display_sizes(nums1, nums2, nums3);
 
    // move assignment moves data from nums1 to nums3,
    // modifying both nums1 and nums3
    nums3 = std::move(nums1);
 
    std::cout << "After move assigment:\n"; 
    display_sizes(nums1, nums2, nums3);
}

// 1 2 3 4 5 6 9
void __test_begin_end() {
    tinystd::set<int> set = { 3, 1, 4, 1, 5, 9, 2, 6, 5 };
    tinystd::for_each(set.cbegin(), set.cend(), [](int x) {
      std::cout << x << ' ';
    });
    std::cout << '\n';
}

void __test_empty() {
    tinystd::set<int> numbers;
    std::cout << std::boolalpha;
    std::cout << "Initially, numbers.empty(): " << numbers.empty() << '\n';
 
    numbers.insert(42);
    numbers.insert(13317); 
    std::cout << "After adding elements, numbers.empty(): " << numbers.empty() << '\n';
}

// nums contains 4 elements.
void __test_size() {
    tinystd::set<int> nums {1, 3, 5, 7};
    std::cout << "nums contains " << nums.size() << " elements.\n";
}

const char* separate(unsigned long long n)
{
    static char buf[64];
 
    int i{sizeof(buf) - 1}, j{};
    buf[i] = '\0';
 
    do {
        buf[--i] = '0' + (n % 10);
        if (j++ % 3 == 2)
            buf[--i] = '\'';
    } while (n /= 10);
 
    return buf + i + (buf[i] == '\'' ? 1 : 0);
}
 
void __test_max_size()
{
    tinystd::set<char> s;
    std::cout << "Maximum size of a 'set' is " << separate(s.max_size()) << "\n";
}

// Before clear: 1 2 3
// Size=3
// Clear
// After clear:
// Size=0
void __test_clear() {
    tinystd::set<int> container{1, 2, 3};
 
    auto print = [](const int& n) { std::cout << " " << n; };
 
    std::cout << "Before clear:";
    tinystd::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize=" << container.size() << '\n';
 
    std::cout << "Clear\n";
    container.clear();
 
    std::cout << "After clear:";
    tinystd::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize=" << container.size() << '\n';
}

// insert done
// no insertion
void __test_insert() {
    tinystd::set<int> set;
 
    auto result_1 = set.insert(3);
    assert(result_1.first != set.end()); // it's a valid iterator
    assert(*result_1.first == 3);
    if (result_1.second)
        std::cout << "insert done\n";
 
    auto result_2 = set.insert(3);
    assert(result_2.first == result_1.first); // same iterator
    assert(*result_2.first == 3);
    if (!result_2.second)
        std::cout << "no insertion\n";
}

template<class Os, class Co> Os& operator<<(Os& os, const Co& co) {
    os << "{";
    for (auto const& i : co) { os << ' ' << i; }
    return os << " } ";
}
 
// { 1 2 3 } { 4 5 } 2 5 1 4
// { 4 5 } { 1 2 3 } 2 5 1 4
// { 1 2 } { 3 4 } 6 9
// { 3 4 } { 1 2 } 9 6
void __test_swap() {
    tinystd::set<int> a1{3, 1, 3, 2}, a2{5, 4, 5};
 
    auto it1 = tinystd::next(a1.begin());
    auto it2 = tinystd::next(a2.begin());
 
    const int& ref1 = *(a1.begin());
    const int& ref2 = *(a2.begin());
 
    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';
    a1.swap(a2);
    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';
 
    // Note that every iterator referring to an element in one container before the swap
    // refers to the same element in the other container after the swap. Same is true
    // for references.
 
    struct Cmp : std::less<int> {
        int id{};
        Cmp(int i) : id{i} { }
    };
 
    tinystd::set<int, Cmp> s1{ {2, 2, 1, 1}, Cmp{6} }, s2{ {4, 4, 3, 3}, Cmp{9} };
 
    std::cout << s1 << s2 << s1.key_comp().id << ' ' << s2.key_comp().id << '\n';
    s1.swap(s2);
    std::cout << s1 << s2 << s1.key_comp().id << ' ' << s2.key_comp().id << '\n';
 
    // So, comparator objects (Cmp) are also exchanged after the swap.
}
