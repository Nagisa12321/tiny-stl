#include "__tinystl_utils.h"
#include "tinystl_algobase.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <string_view>
#include <algorithm>
#include <cassert>

#include <tinystl_map.h>
#include <tinystl_pair.h>
#include <tinystl_iterator.h>
#include <tinystl_string.h>

void __test_constructor();
void __test_operator_equal();
void __test_operator_index();
void __test_begin_end();
void __test_empty();
void __test_size();
void __test_erase();
void __test_swap();
void __test_find();
void __test_map();

int main() {
    srand(time(0x0));

    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test constructor of map...", __test_constructor },
        { "test operator=() ... ", __test_operator_equal },
        { "test operator[]() ... ", __test_operator_index }, 
        { "test begin() or cbrgin() ... ", __test_begin_end },
        { "test empty() ... ", __test_empty },
        { "test size() ... ", __test_size },
        { "test erase() ... ", __test_erase },
        { "test swap() ... ", __test_swap },
        { "test find() ... ", __test_find },
        { "test map ", __test_map },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}
 
template<typename Map>
void print_map(Map& m)
{
   std::cout << '{';
   for(auto& p: m)
        std::cout << p.first << ':' << p.second << ' ';
   std::cout << "}\n";
}
 
struct Point { double x, y; };
struct PointCmp {
    bool operator()(const Point& lhs, const Point& rhs) const { 
        return lhs.x < rhs.x; // NB. intentionally ignores y
    }
};
 
// map1 = {anything:199 something:69 that thing:50 }
//  
// iter = {anything:199 something:69 that thing:50 }
// map1 = {anything:199 something:69 that thing:50 }
//  
// copied = {anything:199 something:69 that thing:50 }
// map1 = {anything:199 something:69 that thing:50 }
//  
// moved = {anything:199 something:69 that thing:50 }
// map1 = {}
//  
// init = {be:100 can:100 const:100 this:100 }
// The magnitude of (-8, -15) is 17
// The magnitude of (3, 4) is 5
// The magnitude of (5, -12) is 13
//  
// The magnitude of (3, 4) is 5
// The magnitude of (5, -12) is 13
// The magnitude of (-8, -15) is 17
void __test_constructor()
{
    // (1) Default constructor
    tinystd::map<std::string, int> map1;
    map1["something"] = 69;
    map1["anything"] = 199;
    map1["that thing"] = 50;
    std::cout << "map1 = "; print_map(map1);
 
    // (2) Range constructor
    tinystd::map<std::string, int> iter(map1.find("anything"), map1.end());
    std::cout << "\niter = "; print_map(iter);
    std::cout << "map1 = "; print_map(map1);
 
    // (3) Copy constructor
    tinystd::map<std::string, int> copied(map1);
    std::cout << "\ncopied = "; print_map(copied);
    std::cout << "map1 = "; print_map(map1);
 
    // (4) Move constructor (Sorry for not support move now...)
    tinystd::map<std::string, int> moved(std::move(map1));
    std::cout << "\nmoved = "; print_map(moved);
    std::cout << "map1 = "; print_map(map1);
 
    // (5) Initializer list constructor
    const tinystd::map<std::string, int> init {
      {"this", 100},
      {"can", 100},
      {"be", 100},
      {"const", 100},
    };
    std::cout << "\ninit = "; print_map(init);
 
 
    // Custom Key class option 1:
    // Use a comparison struct
    tinystd::map<Point, double, PointCmp> mag = {
        { {5, -12}, 13 },
        { {3, 4},   5 },
        { {-8, -15}, 17 }
    };
 
    for(auto p : mag)
        std::cout << "The magnitude of (" << p.first.x
                  << ", " << p.first.y << ") is "
                  << p.second << '\n';
 
    // Custom Key class option 2:    
    // Use a comparison lambda
    // This lambda sorts points according to their magnitudes, where note that
    //  these magnitudes are taken from the local variable mag
    auto cmpLambda = [&mag](const Point &lhs, const Point &rhs) { return mag[lhs] < mag[rhs]; };
    //You could also use a lambda that is not dependent on local variables, like this:
    //auto cmpLambda = [](const Point &lhs, const Point &rhs) { return lhs.y < rhs.y; };
    tinystd::map<Point, double, decltype(cmpLambda)> magy(cmpLambda);
 
    //Various ways of inserting elements:
    magy.insert(tinystd::pair<Point, double>({5, -12}, 13));
    magy.insert({ {3, 4}, 5});
    magy.insert({Point{-8.0, -15.0}, 17});
 
    std::cout << '\n';
    for(auto p : magy)
        std::cout << "The magnitude of (" << p.first.x
                  << ", " << p.first.y << ") is "
                  << p.second << '\n';
}

void display_sizes(const tinystd::map<int, int> &nums1,
                   const tinystd::map<int, int> &nums2,
                   const tinystd::map<int, int> &nums3) {
    std::cout << "nums1: " << nums1.size() 
              << " nums2: " << nums2.size()
              << " nums3: " << nums3.size() << '\n';
}
 
// nums1: 6 nums2: 0 nums3: 0
// After assigment:
// nums1: 6 nums2: 6 nums3: 0
// After move assigment:
// nums1: 0 nums2: 6 nums3: 6
void __test_operator_equal() {
    tinystd::map<int, int> nums1 {{3, 1}, {4, 1}, {5, 9}, 
                              {6, 1}, {7, 1}, {8, 9}};
    tinystd::map<int, int> nums2; 
    tinystd::map<int, int> nums3;
 
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

auto print = [](auto const comment, auto const& map) {
    std::cout << comment << "{";
    for (const auto &pair : map) {
        std::cout << "{" << pair.first << ": " << pair.second << "}";
    }
    std::cout << "}\n";
};
 
// letter_counts initially contains: {{a: 27}{b: 3}{c: 1}}
// after modifications it contains: {{a: 27}{b: 42}{c: 1}{x: 9}}
// 2 occurrences of word 'a'
// 1 occurrences of word 'hoax'
// 2 occurrences of word 'is'
// 1 occurrences of word 'not'
// 3 occurrences of word 'sentence'
// 0 occurrences of word 'that'
// 2 occurrences of word 'this'
void __test_operator_index() {
    tinystd::map<char, int> letter_counts {{'a', 27}, {'b', 3}, {'c', 1}};
 
    print("letter_counts initially contains: ", letter_counts);
 
    letter_counts['b'] = 42;  // updates an existing value
    letter_counts['x'] = 9;  // inserts a new value
 
    print("after modifications it contains: ", letter_counts);
 
    // count the number of occurrences of each word
    // (the first call to operator[] initialized the counter with zero)
    std::map<std::string, int>  word_map;
    for (const auto &w : { "this", "sentence", "is", "not", "a", "sentence",
                           "this", "sentence", "is", "a", "hoax"}) {
        ++word_map[w];
    }
    word_map["that"]; // just inserts the pair {"that", 0}
 
    for (const auto &[word, count] : word_map) {
        std::cout << count << " occurrences of word '" << word << "'\n";
    }
}

// 1, 1.09
// 4, 4.13
// 9, 9.24
void __test_begin_end() {
    tinystd::map<int, float> num_map;
    num_map[4] = 4.13;
    num_map[9] = 9.24;
    num_map[1] = 1.09;
    // calls a_map.begin() and a_map.end()
    for (auto it = num_map.begin(); it != num_map.end(); ++it) {
      std::cout << it->first << ", " << it->second << '\n';
    }
}

// Initially, numbers.empty(): true
// After adding elements, numbers.empty(): false
void __test_empty() {
    tinystd::map<int,int> numbers;
    std::cout << std::boolalpha;
    std::cout << "Initially, numbers.empty(): " << numbers.empty() << '\n';
 
    numbers.insert({42, 13});
    numbers.insert({13317, 123}); 
    std::cout << "After adding elements, numbers.empty(): " << numbers.empty() << '\n';
}

// nums contains 4 elements.
void __test_size() {
    tinystd::map<int,char> nums {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'}};
    std::cout << "nums contains " << nums.size() << " elements.\n";
}

// two four six
void __test_erase() {
    tinystd::map<int, std::string> c = {
        {1, "one" }, {2, "two" }, {3, "three"},
        {4, "four"}, {5, "five"}, {6, "six"  }
    };
 
    // erase all odd numbers from c
    for(auto it = c.begin(); it != c.end(); ) {
        auto prev = it++;
        if(prev->first % 2 != 0) c.erase(prev);
    }
 
    for(auto& p : c) {
        std::cout << p.second << ' ';
    }
}

// print out a std::pair
template <class Os, class U, class V>
Os& operator<<(Os& os, const tinystd::pair<U, V>& p) {
    return os << p.first << ":" << p.second;
}
 
// print out a container
template <class Os, class Co>
Os& operator<<(Os& os, const Co& co) {
    os << "{";
    for (auto const& i : co) { os << ' ' << i; }
    return os << " }\n";
}
 
// ???????????????????????? before swap ????????????????????????
// m1: { ??:alpha ??:beta ??:gamma }
// m2: { ??:delta ??:epsilon }
// ref: ??:alpha
// iter: ??:beta
// ???????????????????????? after swap ????????????????????????
// m1: { ??:delta ??:epsilon }
// m2: { ??:alpha ??:beta ??:gamma }
// ref: ??:alpha
// iter: ??:beta
void __test_swap()
{
    tinystd::map<tinystd::string, tinystd::string>
        m1 { {"??", "gamma"}, {"??", "beta"}, {"??", "alpha"}, {"??", "gamma"}, },
        m2 { {"??", "epsilon"}, {"??", "delta"}, {"??", "epsilon"} };
 
    const auto& ref = *(m1.begin());
    const auto iter = tinystd::next(m1.cbegin());
 
    std::cout << "???????????????????????? before swap ????????????????????????\n"
              << "m1: " << m1 << "m2: " << m2 << "ref: " << ref
              << "\niter: " << *iter << '\n';
 
    m1.swap(m2);
 
    std::cout << "???????????????????????? after swap ????????????????????????\n"
              << "m1: " << m1 << "m2: " << m2 << "ref: " << ref
              << "\niter: " << *iter << '\n';
 
    // Note that every iterator referring to an element in one container before
    // the swap refers to the same element in the other container after the swap.
    // Same is true for references.
}


struct FatKey   { int x; int data[1000]; };
struct LightKey { int x; };
// Note: as detailed above, the container must use std::less<> (or other 
//   transparent Comparator) to access these overloads.
// This includes standard overloads, such as between std::string and std::string_view.
bool operator<(const FatKey& fk, const LightKey& lk) { return fk.x < lk.x; }
bool operator<(const LightKey& lk, const FatKey& fk) { return lk.x < fk.x; }
bool operator<(const FatKey& fk1, const FatKey& fk2) { return fk1.x < fk2.x; }
 
// Found 2 b
// Found 2 b
// Found 2 b
void __test_find() {
// simple comparison demo
    tinystd::map<int,char> example = {{1,'a'},{2,'b'}};
 
    auto search = example.find(2);
    if (search != example.end()) {
        std::cout << "Found " << search->first << " " << search->second << '\n';
    } else {
        std::cout << "Not found\n";
    }
//
// NOT SUPPORT transparent comparison NOW!!!
//

// transparent comparison demo
    // tinystd::map<FatKey, char, std::less<>> example2 = { { {1, {} },'a'}, { {2, {} },'b'} };
 
    // LightKey lk = {2};
    // auto search2 = example2.find(lk);
    // if (search2 != example2.end()) {
    //     std::cout << "Found " << search2->first.x << " " << search2->second << '\n';
    // } else {
    //     std::cout << "Not found\n";
    // }
// Obtaining const iterators.
// Compiler decides whether to return iterator of (non) const type by way of accessing
// map; to prevent modification on purpose, one of easiest choices is to access map by
// const reference.
    // const auto& example2ref = example2;
    // auto search3 = example2ref.find(lk);
    // if (search3 != example2.end()) {
    //     std::cout << "Found " << search3->first.x << ' ' << search3->second << '\n';
    // //  search3->second = 'c'; // error: assignment of member
    //                            // 'std::pair<const FatKey, char>::second'
    //                            // in read-only object
    // }
}

tinystd::string to_string(int __x) {
    return tinystd::string(std::to_string(__x).c_str());
}

template <class _T1, class _T2>
inline bool operator==(const tinystd::pair<_T1, _T2>& __x, const std::pair<const _T1, _T2>& __y)
{ 
  return __x.first == __y.first && __x.second == __y.second; 
}

void __test_map() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::map<tinystd::string, int> tiny_map;
        std::map<tinystd::string, int> std_map;
        for (int i = 0; i < 1000; ++i) {
            int random_shit = rand() % 1000;
            tiny_map.insert({to_string(random_shit), random_shit});
            std_map.insert({to_string(random_shit), random_shit});
        }
        assert(tiny_map.size() == std_map.size());
        auto entry1 = tiny_map.begin();
        auto entry2 = std_map.begin();
        while (entry1 != tiny_map.end()) {
            assert(*entry1++ == *entry2++);
        }
        std::cout << "...ok" << std::endl;
    }
    std::cout << "test2: test insert 0xffffff" << std::endl;
    tinystd::map<int, int> tiny_map;
    std::map<int, int> std_map;
    {
        __timer timer;
        timer.__start();
        for (int i = 0; i < 0xfffff; ++i) {
            int r = rand(); 
            tiny_map.insert({r, i});
        }
        timer.__end();
        std::cout << "tiny insert 0xfffff time is: " << timer.__get_ms() << " ms" << std::endl;

        timer.__start();
        for (int i = 0; i < 0xfffff; ++i) {
            int r = rand(); 
            std_map.insert({r, i});
        }
        timer.__end();
        std::cout << "std insert 0xfffff time is: " << timer.__get_ms() << " ms" << std::endl;
    }
}

