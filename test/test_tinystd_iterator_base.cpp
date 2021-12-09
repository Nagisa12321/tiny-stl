#include <iostream>
#include <ostream>
#include <vector>
#include "tinystl_iterator_base.h"

void __test_pointer_traits();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases {
        { "test the iterator of pointer. ", __test_pointer_traits },

    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

template <typename _Iterator>
void __pass_iterator(_Iterator __it) {
    typedef typename tinystd::__iterator_traits<_Iterator>::value_type value_type;
    value_type __val = *__it;
}

void __test_pointer_traits() {
    {
        std::cout << "test 1: pass a vector<int> iterator" << std::endl;
        std::vector<int> ivec{10};
        __pass_iterator(ivec.begin());    
    }

    {
        std::cout << "test 2: pass a int *" << std::endl;
        int *__pi = new int;
        __pass_iterator(__pi);
        delete __pi;
    }

    {
        std::cout << "test 3: pass a const int *" << std::endl;
        const int *__cpi = new int;
        __pass_iterator(__cpi);
        delete __cpi;
    }

    std::cout << "__test_pointer_traits pass. " << std::endl;
}