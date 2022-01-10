#include "tinystl_deque.h"
#include "tinystl_list.h"
#include "tinystl_queue.h"
#include <deque>
#include <iostream>
#include <ostream>
#include <vector>
#include <queue>

// template init
template class tinystd::queue<int>;
template class tinystd::queue<int, tinystd::list<int>>;

void __test_std_queue();
void __test_tiny_queue();
void __test_list_queue();
void __test_obj_queue();
void __test_priority_queue();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the STL queue. ", __test_std_queue },
        { "test my queue. ", __test_tiny_queue },
        { "test my queue (container is list). ", __test_list_queue },
        { "test my queue of objects . ", __test_obj_queue },
        { "test priority_queue... ", __test_priority_queue },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

template <typename _Queue, typename _Sequence>
void __generic_test_case() {
    std::cout << "test1: constructor" << std::endl;
    {
        _Queue c1;
        c1.push(5);
        std::cout << c1.size() << '\n';

        _Queue c2(c1);
        std::cout << c2.size() << '\n';

        _Sequence seq{3, 1, 4, 1, 5};
        _Queue c3(seq);
        std::cout << c3.size() << '\n';
    }
    std::cout << "test2: empty" << std::endl;
    {
        std::cout << std::boolalpha;
        _Queue container;
        std::cout << "Initially, container.empty(): " << container.empty() << '\n';
        container.push(42);
        std::cout << "After adding elements, container.empty(): " << container.empty() << '\n';
    }
    std::cout << "test3: size" << std::endl;
    {
        _Queue container;
        std::cout << "Initially, container.size(): " << container.size() << '\n';
        for (int i = 0; i < 7; ++i)
            container.push(i);
        std::cout << "After adding elements, container.size(): " << container.size() << '\n';
    }
}


struct __obj {
    __obj() : _M_pi(new int) {}
    __obj(int i) : _M_pi(new int(i)) {}
    __obj(const __obj &__o) : _M_pi(new int(*__o._M_pi)) {}
    ~__obj() { delete _M_pi; }
    __obj &operator=(const __obj &__o) {
        if (&__o == this) return *this;
        delete _M_pi;
        _M_pi = new int(*__o._M_pi);
        return *this;
    }

    int *_M_pi;
};


std::ostream &operator<<(std::ostream &__o, const __obj &__obj) {
    return __o << *__obj._M_pi;
}

void __test_std_queue() { __generic_test_case<std::queue<int>, std::deque<int>>(); }
void __test_tiny_queue() { __generic_test_case<tinystd::queue<int>, tinystd::deque<int>>(); }
void __test_list_queue() { __generic_test_case<tinystd::queue<int, tinystd::list<int>>, tinystd::list<int>>(); } 
void __test_obj_queue() { __generic_test_case<tinystd::queue<__obj>, tinystd::deque<__obj>>(); } 

template<typename T>
void print_queue(T q) { // NB: pass by value so the print uses a copy
    while(!q.empty()) {
        std::cout << q.top() << ' ';
        q.pop();
    }
    std::cout << '\n';
}

// 9 8 7 6 5 4 3 2 1 0 
// 0 1 2 3 4 5 6 7 8 9 
// 8 9 6 7 4 5 2 3 0 1
void __test_priority_queue() {
    tinystd::priority_queue<int> q;

    const auto data = {1, 8, 5, 6, 3, 4, 0, 9, 7, 2};

    for (int n : data)
        q.push(n);

    print_queue(q);

    tinystd::priority_queue<int, tinystd::vector<int>, tinystd::greater<int>>
        q2(data.begin(), data.end());

    print_queue(q2);

    // Using lambda to compare elements.
    auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1); };
    tinystd::priority_queue<int, tinystd::vector<int>, decltype(cmp)> q3(cmp);

    for (int n : data)
        q3.push(n);

    print_queue(q3);
}