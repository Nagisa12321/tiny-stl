#include "tinystl_list.h"
#include "tinystl_vector.h"
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

void __test_std_list();
void __test_list();
void __test_push_back_with_copy_construct();
void __test_clear();
void __test_remove(); 
void __test_unique();
void __test_splice();
void __test_merge();
void __test_reverse();
void __test_sort();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the std list", __test_std_list },
        { "test my list", __test_list },
        { "test push back with copy constructors", __test_push_back_with_copy_construct },
        { "test clear", __test_clear },
        { "test remove", __test_remove },
        { "test unique", __test_unique },
        { "test splice", __test_splice },
        { "test merge", __test_merge },
        { "test reverse", __test_reverse },
        { "test sort", __test_sort },
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

void __test_clear()
{
    tinystd::list<int> container{1, 2, 3};
 
    auto print = [](const int& n) { std::cout << " " << n; };
 
    std::cout << "Before clear:";
    std::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize=" << container.size() << '\n';
 
    std::cout << "Clear\n";
    container.clear();
 
    std::cout << "After clear:";
    std::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize=" << container.size() << '\n';
}

/**
 * @brief 
 * 2 3 10 -1
 */
void __test_remove() {
    tinystd::list<int> l = {1, 100, 2, 3, 10, 1, 11, -1, 12};

    l.remove(1); // remove both elements equal to 1
    l.remove_if([](int n){ return n > 10; }); // remove all elements greater than 10
    for (int n : l) {
        std::cout << n << ' '; 
    }
    std::cout << '\n';
}

template <typename _Container>
void print(std::string remark, _Container &container) {
    std::cout << remark;
    for (auto const &val : container)
        std::cout << ' ' << val;
    std::cout << '\n';
}

/**
 * @brief 
 * Before unique(): 1 2 2 3 3 2 1 1 2
 * After  unique(): 1 2 3 2 1 2
 * Before unique(pred): 1 2 12 23 3 2 51 1 2
 * After  unique(pred): 1 2 23 2 51 2
 */
void __test_unique() {
    tinystd::list<int> c = {1, 2, 2, 3, 3, 2, 1, 1, 2};
    print("Before unique():", c);
    c.unique();
    print("After  unique():", c);
    c = {1, 2, 12, 23, 3, 2, 51, 1, 2};
    print("Before unique(pred):", c);
    c.unique([mod = 10](int x, int y) { return (x % mod) == (y % mod); });
    print("After  unique(pred):", c);
}

std::ostream& operator<<(std::ostream& ostr, const tinystd::list<int>& list)
{
    for (auto &i : list) {
        ostr << " " << i;
    }
    return ostr;
}
/**
 * @brief 
 * list1:  1 2 10 20 30 40 50 3 4 5
 * list2: 
 * list1:  1 2 10 20 30 40 50
 * list2:  3 4 5
 */
void __test_splice() {
    tinystd::list<int> list1 = { 1, 2, 3, 4, 5 };
    tinystd::list<int> list2 = { 10, 20, 30, 40, 50 };
 
    auto it = list1.begin();
    tinystd::advance(it, 2);
 
    list1.splice(it, list2);
 
    std::cout << "list1: " << list1 << "\n";
    std::cout << "list2: " << list2 << "\n";
 
    list2.splice(list2.begin(), list1, it, list1.end());
 
    std::cout << "list1: " << list1 << "\n";
    std::cout << "list2: " << list2 << "\n";
}
/**
 * @brief 
 * list1:   1 3 3 5 9
 * list2:   2 3 4 4 7 8
 * merged:  1 2 3 3 3 4 4 5 7 8 9
 */
void __test_merge() {
    tinystd::list<int> list1 = {1, 3, 3, 5, 9};
    tinystd::list<int> list2 = {2, 3, 4, 4, 7, 8};

    std::cout << "list1:  " << list1 << '\n';
    std::cout << "list2:  " << list2 << '\n';
    list1.merge(list2);
    std::cout << "merged: " << list1 << '\n';
}
/**
 * @brief 
 * before:      8 7 5 9 0 1 3 2 6 4
 * ascending:   0 1 2 3 4 5 6 7 8 9
 * descending:  9 8 7 6 5 4 3 2 1 0
 */
void __test_reverse() {
    tinystd::list<int> list = {8, 7, 5, 9, 0, 1, 3, 2, 6, 4};

    std::cout << "before:     " << list << "\n";
    list.sort();
    std::cout << "ascending:  " << list << "\n";
    list.reverse();
    std::cout << "descending: " << list << "\n";
}

void __test_sort() {
    tinystd::list<int> list = { 8,7,5,9,0,1,3,2,6,4 };
 
    std::cout << "before:     " << list << "\n";
    list.sort();
    std::cout << "ascending:  " << list << "\n";
    list.sort(std::greater<int>());
    std::cout << "descending: " << list << "\n";
}