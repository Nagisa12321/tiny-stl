#include "tinystl_list.h"
#include "tinystl_stack.h"
#include <iostream>
#include <ostream>
#include <vector>
#include <stack>

// init the template
template class tinystd::stack<int>;
template class tinystd::stack<int, tinystd::list<int>>;

void __test_std_stack();
void __test_tinystd_stack();
void __test_list_stack();
void __test_obj_stack();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test STL stack. ", __test_std_stack },
        { "test my stack. ", __test_tinystd_stack },
        { "test my stack (container is list). ", __test_list_stack },
        { "test my stack of objects . ", __test_obj_stack },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}


template <typename _Stack>
void create_stack(_Stack __st) {
    _Stack ms = __st;
    while (!ms.empty()) {
        std::cout << ' ' << ms.top();
        ms.pop();
    }
    std::cout << '\n';
}

void __test_std_stack() {
    std::cout << "test1: " << std::endl;
    {
        std::stack<int> st;
        st.push(10);
        st.push(20);
        st.push(30);
        st.push(40);

        st.pop();
        st.pop();

        while (!st.empty()) {
            std::cout << ' ' << st.top();
            st.pop();
        }
        std::cout << std::endl;
    }
    std::cout << "test2: " << std::endl;
    {
        std::stack<int> st;
        st.push(32);
        st.push(21);
        st.push(39);
        st.push(89);
        st.push(25);

        std::cout << "The stack st is: ";
        create_stack(st);
        std::cout << "\n st.size() : " << st.size();
        std::cout << "\n st.top() : " << st.top();
        std::cout << "\n st.pop() : ";
        st.pop();
        create_stack(st);
    }
}

/**
 * @brief
 *  test1:
 *  20 10
 * test2:
 * The stack st is:  25 89 39 21 32
 *
 *  st.size() : 5
 *  st.top() : 25
 *  st.pop() :  89 39 21 32
 *
 */
void __test_tinystd_stack() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::stack<int> st;
        st.push(10);
        st.push(20);
        st.push(30);
        st.push(40);

        st.pop();
        st.pop();

        while (!st.empty()) {
            std::cout << ' ' << st.top();
            st.pop();
        }
        std::cout << std::endl;
    }
    std::cout << "test2: " << std::endl;
    {
        tinystd::stack<int> st;
        st.push(32);
        st.push(21);
        st.push(39);
        st.push(89);
        st.push(25);

        std::cout << "The stack st is: ";
        create_stack(st);
        std::cout << "\n st.size() : " << st.size();
        std::cout << "\n st.top() : " << st.top();
        std::cout << "\n st.pop() : ";
        st.pop();
        create_stack(st);
    }
}

void __test_list_stack() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::stack<int, tinystd::list<int>> st;
        st.push(10);
        st.push(20);
        st.push(30);
        st.push(40);

        st.pop();
        st.pop();

        while (!st.empty()) {
            std::cout << ' ' << st.top();
            st.pop();
        }
        std::cout << std::endl;
    }
    std::cout << "test2: " << std::endl;
    {
        tinystd::stack<int, tinystd::list<int>> st;
        st.push(32);
        st.push(21);
        st.push(39);
        st.push(89);
        st.push(25);

        std::cout << "The stack st is: ";
        create_stack(st);   
        std::cout << "\n st.size() : " << st.size();
        std::cout << "\n st.top() : " << st.top();
        std::cout << "\n st.pop() : ";
        st.pop();
        create_stack(st);
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

void __test_obj_stack() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::stack<__obj> st;
        st.push(10);
        st.push(20);
        st.push(30);
        st.push(40);

        st.pop();
        st.pop();

        while (!st.empty()) {
            std::cout << ' ' << st.top();
            st.pop();
        }
        std::cout << std::endl;
    }
    std::cout << "test2: " << std::endl;
    {
        tinystd::stack<__obj> st;
        st.push(32);
        st.push(21);
        st.push(39);
        st.push(89);
        st.push(25);

        std::cout << "The stack st is: ";
        create_stack(st);   
        std::cout << "\n st.size() : " << st.size();
        std::cout << "\n st.top() : " << st.top();
        std::cout << "\n st.pop() : ";
        st.pop();
        create_stack(st);
    }
}