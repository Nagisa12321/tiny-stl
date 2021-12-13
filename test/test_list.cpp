#include "tinystl_list.h"
#include "tinystl_vector.h"
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

void __test_std_list();
void __test_list();
void __test_push_back_with_copy_construct();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the std list", __test_std_list },
        { "test my list", __test_list },
        { "test push back with copy constructors", __test_push_back_with_copy_construct },

    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

void __test_std_list() {
    // Create a list containing integers
    std::list<int> l = { 7, 5, 16, 8 };
 
    // Add an integer to the front of the list
    l.push_front(25);
    // Add an integer to the back of the list
    l.push_back(13);
 
    // Insert an integer before 16 by searching
    auto it = std::find(l.begin(), l.end(), 16);
    if (it != l.end()) {
        l.insert(it, 42);
    }
 
    // Print out the list
    std::cout << "l = { ";
    for (int n : l) {
        std::cout << n << ", ";
    }
    std::cout << "};\n";
}

void __test_push_back_with_copy_construct() {
    tinystd::list<int> __li;
    for (int i = 0; i < 20; ++i) { 
        __li.push_back(i); 
    }

    auto it = __li.begin();
    while (it != __li.end())
        std::cout << *it++ << ' ';
    std::cout << std::endl;

}

void __test_list() {
    // Create a list containing integers
    tinystd::list<int> l = { 7, 5, 16, 8 };
 
    // Add an integer to the front of the list
    l.push_front(25);
    // Add an integer to the back of the list
    l.push_back(13);
 
    // Insert an integer before 16 by searching
    auto it = tinystd::find(l.begin(), l.end(), 16);
    if (it != l.end()) {
        l.insert(it, 42);
    }
 
    // Print out the list
    std::cout << "l = { ";
    for (int n : l) {
        std::cout << n << ", ";
    }
    std::cout << "};\n";
}