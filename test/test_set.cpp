#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <cmath>
#include <tinystl_set.h>

void __test_constructor();
void __test_operator_eq();
void __test_begin_end();
void __test_empty();
void __test_size();
void __test_max_size();
void __test_clear();

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

