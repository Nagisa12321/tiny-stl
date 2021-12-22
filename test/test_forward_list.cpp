#include "tinystl_forward_list.h"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <forward_list>

// template init
template class tinystd::forward_list<int>;

void __test_std_forward_list();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test STL's forward_list", __test_std_forward_list },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

template <typename T>
std::ostream &operator<<(std::ostream &s, const std::forward_list<T> &v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto &e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}


template <typename T>
std::ostream &operator<<(std::ostream &s, const tinystd::forward_list<T> &v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto &e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}


template <typename _ForwardIntList, typename _ForwardStringList>
void __generic_test_case() {
    std::cout << "test1: Normal use"<< std::endl;
    {
        _ForwardIntList mylist = {34, 77, 16, 2};
        std::cout << "mylist contains:";
        for (auto it = mylist.begin(); it != mylist.end(); ++it)
            std::cout << ' ' << *it;
        std::cout << '\n';
    }
    std::cout << "test2: constructor: " << std::endl;
    {
        // c++11 initializer list syntax:
        _ForwardStringList words1{"the", "frogurt", "is", "also", "cursed"};
        std::cout << "words1: " << words1 << '\n';

        // words2 == words1
        _ForwardStringList words2(words1.begin(), words1.end());
        std::cout << "words2: " << words2 << '\n';

        // words3 == words1
        _ForwardStringList words3(words1);
        std::cout << "words3: " << words3 << '\n';

        // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
        _ForwardStringList words4(5, "Mo");
        std::cout << "words4: " << words4 << '\n';
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

void __test_std_forward_list() { __generic_test_case<std::forward_list<int>, std::forward_list<std::string>>(); }
