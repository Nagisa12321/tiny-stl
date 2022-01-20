#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <cmath>
#include <tinystl_set.h>

void __test_constructor();

int main() {
    srand(time(0x0));

    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test constructor...", __test_constructor },
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
