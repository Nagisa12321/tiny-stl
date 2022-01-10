#include "tinystl_deque.h"
#include "tinystl_list.h"
#include "tinystl_queue.h"
#include <deque>
#include <iostream>
#include <ostream>
#include <vector>
#include <queue>
#include <complex.h>

// template init
template class tinystd::queue<int>;
template class tinystd::queue<int, tinystd::list<int>>;

void __test_std_queue();
void __test_tiny_queue();
void __test_list_queue();
void __test_obj_queue();
void __test_priority_queue();
void __test_constructor();
void __test_top();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the STL queue. ", __test_std_queue },
        { "test my queue. ", __test_tiny_queue },
        { "test my queue (container is list). ", __test_list_queue },
        { "test my queue of objects . ", __test_obj_queue },
        { "test priority_queue... ", __test_priority_queue },
        { "test priority_queue's constructors... ", __test_constructor },
        { "test priority_queue's top()... ", __test_top },
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

// pq1.size() = 1
// pq2.size() = 1
// pq3.size() = 5
// pq3 : 5 4 3 1 1 
// pq4.top() = (3,2)
// pq4.top() = (5,1)
// pq4.top() = (7,3)
void __test_constructor() {
    tinystd::priority_queue<int> pq1;
    pq1.push(5);
    std::cout << "pq1.size() = " << pq1.size() << '\n';

    tinystd::priority_queue<int> pq2{pq1};
    std::cout << "pq2.size() = " << pq2.size() << '\n';

    tinystd::vector<int> vec{3, 1, 4, 1, 5};
    tinystd::priority_queue<int> pq3{tinystd::less<int>(), vec};
    std::cout << "pq3.size() = " << pq3.size() << '\n';

    for (std::cout << "pq3 : "; !pq3.empty(); pq3.pop()) {
        std::cout << pq3.top() << ' ';
    }
    std::cout << '\n';

    // Demo With Custom Comparator:

    using my_value_t = std::complex<double>;
    using my_container_t = tinystd::vector<my_value_t>;

    auto my_comp = [](const my_value_t &z1, const my_value_t &z2) {
        return z2.real() < z1.real();
    };

    tinystd::priority_queue<my_value_t,
                            my_container_t,
                            decltype(my_comp)>
        pq4{my_comp};

    using namespace std::complex_literals;
    pq4.push(5.0 + 1i);
    pq4.push(3.0 + 2i);
    pq4.push(7.0 + 3i);

    for (; !pq4.empty(); pq4.pop()) {
        const auto &z = pq4.top();
        std::cout << "pq4.top() = " << z << '\n';
    }
}

struct Event {
    int priority{};
    char data{' '};

    friend bool operator<(Event const &lhs, Event const &rhs) {
        return lhs.priority < rhs.priority;
    }

    friend std::ostream &operator<<(std::ostream &os, Event const &e) {
        return os << "{ " << e.priority << ", '" << e.data << "' } ";
    }
};

// Fill the events queue:
// { 6, 'L' }  { 8, 'I' }  { 9, 'S' }  { 1, 'T' }  { 5, 'E' }  { 3, 'N' }
// Process events:
// { 9, 'S' }  { 8, 'I' }  { 6, 'L' }  { 5, 'E' }  { 3, 'N' }  { 1, 'T' }
void __test_top() {
    tinystd::priority_queue<Event> events;

    std::cout << "Fill the events queue:\n";

    for (auto const e : {Event{6, 'L'}, {8, 'I'}, {9, 'S'}, {1, 'T'}, {5, 'E'}, {3, 'N'}}) {
        std::cout << e << ' ';
        events.push(e);
    }

    std::cout << "\n"
                 "Process events:\n";

    for (; !events.empty(); events.pop()) {
        Event const &e = events.top();
        std::cout << e << ' ';
    }
}