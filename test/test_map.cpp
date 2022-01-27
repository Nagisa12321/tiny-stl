#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <map>

#include <tinystl_map.h>
#include <tinystl_pair.h>

void __test_constructor();
void __test_operator_equal();

int main() {
    srand(time(0x0));

    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test constructor of map...", __test_constructor },
        { "test operator=() ... ", __test_operator_equal },
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

