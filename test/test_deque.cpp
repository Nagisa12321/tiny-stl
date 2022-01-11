#include "__tinystl_utils.h"
#include "tinystl_deque.h"
#include "tinystl_list.h"
#include "tinystl_vector.h"
#include "tinystl_algobase.h"
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

void __test_time();
void __test_std_deque();
void __test_init();
void __test_push();
void __test_pop();
void __test_clear();
void __test_erase();
void __test_operator_sub();
void __test_insert();

// template definition.
template class tinystd::deque<int>;

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test std::deque", __test_std_deque },
        { "test init", __test_init }, 
        { "test push_back and push_front", __test_push }, 
        { "test pop_back and pop_front", __test_pop }, 
        { "test clear.", __test_clear },
        { "test erase.", __test_erase },
        { "test operator sub.", __test_operator_sub },
        { "test insert", __test_insert }, 
        { "test the time against std::deque", __test_time },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}



void __test_std_deque() {
    std::deque<int> __ideq(20, 9);
    std::cout << "size=" << __ideq.size() << std::endl;

    for (int i = 0; i < __ideq.size(); ++i)
        __ideq[i] = i;

    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;

    for (int i = 0; i < 3; ++i)
        __ideq.push_back(i);

    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;
    std::cout << "size=" << __ideq.size() << std::endl;

    __ideq.push_back(3);
    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;
    std::cout << "size=" << __ideq.size() << std::endl;

    __ideq.push_front(99);
    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;
    std::cout << "size=" << __ideq.size() << std::endl;

    __ideq.push_front(98);
    __ideq.push_front(97);
    for (int i = 0; i < __ideq.size(); ++i)
        std::cout << __ideq[i] << ' ';
    std::cout << std::endl;
    std::cout << "size=" << __ideq.size() << std::endl;

    std::deque<int>::iterator it;
    it = std::find(__ideq.begin(), __ideq.end(), 99);
    std::cout << *it << std::endl;
    std::cout << *(it._M_cur) << std::endl;
}

void __test_init() {
    {
        std::cout << "test1: deque(size_type __n, const _Tp &__value)" << std::endl;
        tinystd::deque<int> __ideq(100, 2048);
        for (int i : __ideq)
            std::cout << i << ' ';
        std::cout << std::endl;
    }
    {
        std::cout << "test1: init the deque of string" << std::endl;

        // tinystd::deque<std::string> __sdeq(100, "hello world");
    }
}

void __test_push() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::deque<int> __ideq;
        std::cout << "__ideq.size()=" << __ideq.size() << std::endl;
        for (int i = 0; i < 10; ++i)
            __ideq.push_back(2048);
        std::cout << "__ideq.size()=" << __ideq.size() << std::endl;
        for (int i : __ideq)
            std::cout << i << ' ';
        std::cout << std::endl;
        for (int i = 0; i < 256; ++i)
            __ideq.push_back(i);
        std::cout << "__ideq.size()=" << __ideq.size() << std::endl;
        for (int i : __ideq)
            std::cout << i << ' ';
        std::cout << std::endl;
    }
    std::cout << "test2: " << std::endl; 
    {
        tinystd::deque<std::string> __sdeq;
        __sdeq.push_back("hello");
        __sdeq.push_front("hi");
        __print_container(__sdeq.begin(), __sdeq.end());
    }
    std::cout << "test3: " << std::endl;
    {
        tinystd::deque<std::string> __sdeq;
        for (int i = 0; i < 500; ++i) {
            __sdeq.push_back(std::to_string(i));
            __sdeq.push_front(std::to_string(i));
        }
        __print_container(__sdeq.begin(), __sdeq.end());
    }
}
/**
 * @brief 
 * test1: pop_front: 
 * ABCDE
 * BCDEF
 * CDEFG
 * DEFGH
 * test2: pop_back: 
 * []
 * [5, 3, 4, ]
 * [5, 3, ]
 * test3: pop too many
 * 32768
 * 0
 */
void __test_pop() {
    std::cout << "test1: pop_front: " << std::endl;
    {
        auto print = [](const auto &con) {
            for (auto const &item : con)
                std::cout << item;
            std::cout << '\n';
        };

        tinystd::deque<char> letters{'A', 'B', 'C', 'D'};

        for (char c{letters.back()}; c != 'H';) {
            letters.push_back(++c);
            print(letters);
            letters.pop_front();
        }
    }
    std::cout << "test2: pop_back: " << std::endl;
    {
        tinystd::deque<int> numbers;
    
        __print_container(numbers.begin(), numbers.end()); 
    
        numbers.push_back(5);
        numbers.push_back(3);
        numbers.push_back(4);
    
        __print_container(numbers.begin(), numbers.end()); 
    
        numbers.pop_back();
    
        __print_container(numbers.begin(), numbers.end()); 
    }
    std::cout << "test3: pop too many" << std::endl;
    {
        int __k = 0xffff;
        tinystd::deque<int> numbers;
        for (int i = 0; i < __k; ++i)
            numbers.push_back(i);
        int a = __k >> 1;
        for (int i = 0; i < a; ++i)
            numbers.pop_back();
        std::cout << numbers.size() << std::endl;
        for (int i = 0; i < __k - a; ++i)
            numbers.pop_front();
        std::cout << numbers.size() << std::endl;
    }
}

void __test_clear() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::deque<int> container{1, 2, 3};
    
        auto print = [](const int& n) { std::cout << " " << n; };
    
        std::cout << "Before clear:";
        tinystd::for_each(container.begin(), container.end(), print);
        std::cout << "\nSize=" << container.size() << '\n';
    
        std::cout << "Clear\n";
        container.clear();
    
        std::cout << "After clear:";
        tinystd::for_each(container.begin(), container.end(), print);
        std::cout << "\nSize=" << container.size() << '\n';

    }
    std::cout << "test2: " << std::endl;
    {
        tinystd::deque<int> __ideq;
        for (int i = 0; i < 1000; ++i) {
            for (int j = 0; j < 1000; ++j)
                __ideq.push_back(j);
            __ideq.clear();
        }
    }

}

void __test_time() {
    std::cout << "\ntest1: push operation" << std::endl;
    {
        std::cout << " >> push 100000000 string" << std::endl;
        tinystd::deque<std::string> __my_deque;
        std::deque<std::string> __std_deque; 

        __timer::__start();
        for (int i = 0; i < 50000000; ++i) {
            __my_deque.push_back(std::to_string(i));
            if (!(i % 5000000))
                std::cout << "." << std::flush;
        }
        for (int i = 0; i < 50000000; ++i) {
            __my_deque.push_front(std::to_string(i));
            if (!(i % 5000000))
                std::cout << "." << std::flush;
        }
        __timer::__end();
        __timer::__time_type __my = __timer::__get_ms();
        printf("my deque ok.\n");

        __timer::__start();
        for (int i = 0; i < 50000000; ++i) {
            __std_deque.push_back(std::to_string(i));
            if (!(i % 5000000))
                std::cout << "." << std::flush;
        }
        for (int i = 0; i < 50000000; ++i) {
            __std_deque.push_front(std::to_string(i));
            if (!(i % 5000000))
                std::cout << "." << std::flush;
        }
        __timer::__end();
        __timer::__time_type __std = __timer::__get_ms();
        printf("stl deque ok.\n");

        std::cout << "my deque's size: " << __my_deque.size() << std::endl;
        std::cout << "stl deque's size: " << __std_deque.size() << std::endl;

        std::cout << "tinystd::deque: " << __my << " ms" << std::endl;
        std::cout << "std::deque: " << __std << " ms" << std::endl;
    }
    std::cout << "\ntest2: pop operation" << std::endl;
    {
        std::cout << " >> init with 100000000 int, and call 50000000 pop_back \n"
                  << "\tand 50000000 pop_front." << std::endl;
        std::deque<int> __std_deque(100000000); 
        tinystd::deque<int> __my_deque(100000000);
        __timer::__start();
        for (int i = 0; i < 50000000; ++i) {
            __std_deque.pop_back();
            __std_deque.pop_front();
            if (!(i % 5000000))
                std::cout << "." << std::flush;
        }
        __timer::__end();
        __timer::__time_type __std = __timer::__get_ms();
        printf("stl deque ok.\n");

        __timer::__start();
        for (int i = 0; i < 50000000; ++i) {
            __my_deque.pop_back();
            __my_deque.pop_front();
            if (!(i % 5000000))
                std::cout << "." << std::flush;
        }
        __timer::__end();
        __timer::__time_type __my = __timer::__get_ms();
        printf("my deque ok.\n");

        std::cout << "my deque's size: " << __my_deque.size() << std::endl;
        std::cout << "stl deque's size: " << __std_deque.size() << std::endl;

        std::cout << "tinystd::deque: " << __my << " ms" << std::endl;
        std::cout << "std::deque: " << __std << " ms" << std::endl;
    }
    std::cout << "\ntest3: test clear and re-push" << std::endl;
    {
        std::cout << " >> use 1000000 push (back and front) operation and then clear, "
                  << "\n\tdo this 1000 times" << std::endl;
        std::deque<int> __std_deque; 
        tinystd::deque<int> __my_deque;
        __timer::__start();
        for (int i = 0; i < 1000; ++i) {
            for (int i = 0; i < 500000; ++i) {
                __std_deque.push_back(i);
                __std_deque.push_front(i);
            }
            __std_deque.clear();
            if (!(i % 20))
                std::cout << "." << std::flush;
        }
        __timer::__end();
        __timer::__time_type __std = __timer::__get_ms();
        printf("stl deque ok.\n");

        __timer::__start();
        for (int i = 0; i < 1000; ++i) {
            for (int i = 0; i < 500000; ++i) {
                __my_deque.push_back(i);
                __my_deque.push_front(i);
            }
            __my_deque.clear();
            if (!(i % 20))
                std::cout << "." << std::flush;
        }
        __timer::__end();
        __timer::__time_type __my = __timer::__get_ms();
        printf("my deque ok.\n");

        std::cout << "my deque's size: " << __my_deque.size() << std::endl;
        std::cout << "stl deque's size: " << __std_deque.size() << std::endl;

        std::cout << "tinystd::deque: " << __my << " ms" << std::endl;
        std::cout << "std::deque: " << __std << " ms" << std::endl;
    }
    std::cout << "\ntest4: test erase and re-push" << std::endl;
    {
        std::cout << " >> use 1000000 push (back and front) operation and then erase all, "
                  << "\n\tdo this 1000 times" << std::endl;
        std::deque<int> __std_deque; 
        tinystd::deque<int> __my_deque;
        __timer::__start();
        for (int i = 0; i < 1000; ++i) {
            for (int i = 0; i < 100000; ++i) {
                __std_deque.push_back(i);
            }
            for (int i = 0; i < 50000; ++i) {
                __std_deque.erase(__std_deque.begin());
                __std_deque.erase(__std_deque.end() - 1);
            }
            if (!(i % 20))
                std::cout << "." << std::flush;
        }
        __timer::__end();
        __timer::__time_type __std = __timer::__get_ms();
        printf("my deque ok.\n");
        __timer::__start();
        for (int i = 0; i < 1000; ++i) {
            for (int i = 0; i < 100000; ++i) {
                __my_deque.push_back(i);
            }
            for (int i = 0; i < 50000; ++i) {
                __my_deque.erase(__my_deque.begin());
                __my_deque.erase(__my_deque.end() - 1);
            }
            if (!(i % 20))
                std::cout << "." << std::flush;
        }
        __timer::__end();
        __timer::__time_type __my = __timer::__get_ms();
        printf("stl deque ok.\n");

        std::cout << "my deque's size: " << __my_deque.size() << std::endl;
        std::cout << "stl deque's size: " << __std_deque.size() << std::endl;

        std::cout << "tinystd::deque: " << __my << " ms" << std::endl;
        std::cout << "std::deque: " << __std << " ms" << std::endl;
    }
}
/**
 * @brief 
 * test1: 
 * [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ]
 * [1, 2, 3, 4, 5, 6, 7, 8, 9, ]
 * [1, 2, 6, 7, 8, 9, ]
 * [1, 7, 9, ]
 */
void __test_erase() {
    std::cout << "test1: " << std::endl;
    {
        tinystd::deque<int> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        __print_container(c.begin(), c.end());

        c.erase(c.begin());
        __print_container(c.begin(), c.end());

        c.erase(c.begin() + 2, c.begin() + 5);
        __print_container(c.begin(), c.end());

        // Erase all even numbers (C++11 and later)
        for (auto it = c.begin(); it != c.end();) {
            if (*it % 2 == 0) {
                it = c.erase(it);
            } else {
                ++it;
            }
        }
        __print_container(c.begin(), c.end());
    }
    std::cout << "test2: " << std::endl;
    {
        tinystd::deque<int> __ideq;
        for (int i = 0; i < 400; ++i)
            __ideq.push_back(i);
        
        // 0...14 385
        //
        for (int i = 0; i < 200; ++i) {
            __ideq.pop_back();
            __ideq.pop_front();
        }
    }
    std::cout << "test3: " << std::endl;
    {
        tinystd::deque<int> __ideq;
        for (int i = 0; i < 50; ++i)
            __ideq.push_back(i);
        for (int i = 0; i < 5; ++i) {
            __ideq.erase(__ideq.begin(), __ideq.begin() + 10);
            __print_container(__ideq.begin(), __ideq.end());
        }
    }
    std::cout << "test4: " << std::endl;
    {
        tinystd::deque<int> __ideq;
        for (int i = 0; i < 200; ++i) 
            { __ideq.push_back(i); }
        for (int i = 0; i < 20; ++i) {
            __ideq.erase(__ideq.end() - 5, __ideq.end());
            __ideq.erase(__ideq.begin(), __ideq.begin() + 5);
            __print_container(__ideq.begin(), __ideq.end());
        }
    }
}

void __test_operator_sub() {
    tinystd::deque<int> __ideq;
    for (int i = 0; i < 300; ++i) 
        __ideq.push_back(i);
    std::cout << *(__ideq.end() - 1) << std::endl;
    std::cout << *(__ideq.end() - 101) << std::endl;
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

void __test_insert() {
    std::cout << "test1: test insert front. " << std::endl;
    {
        tinystd::deque<__obj> __ideq;
        for (int i = 0; i < 10; ++i)
            __ideq.insert(__ideq.begin(), i);
        __print_container(__ideq.begin(), __ideq.end());
    }
    std::cout << "test2: test insert back. " << std::endl;
    {
        tinystd::deque<__obj> __ideq{1024, 2048};
        for (int i = 0; i < 10; ++i)
            __ideq.insert(__ideq.end(), i);
        __print_container(__ideq.begin(), __ideq.end());
    }
    std::cout << "test3: " << std::endl;
    {
        // 0 1 2 3 4 5 6 7 8 9 10
        tinystd::deque<__obj> __ideq;
        for (int i = 0; i < 10; ++i)
            { __ideq.push_back(i); }
        __print_container(__ideq.begin(), __ideq.end());
        
        // 0 1 2 3 4 1024 5 6 7 8 9 10
        __ideq.insert(__ideq.begin() + 5, 1024);
        __print_container(__ideq.begin(), __ideq.end());
    }
    std::cout << "test4: " << std::endl;
    {
        // 0 1 2 3 4 5 6 7 8 9 10
        tinystd::deque<__obj> __ideq;
        for (int i = 0; i < 10; ++i)
            { __ideq.push_back(i); }
        __print_container(__ideq.begin(), __ideq.end());
        
        // 0 1 2 3 4 1024 1024 1024 1024 1024 5 6 7 8 9 10
        __ideq.insert(__ideq.begin() + 5, 5, 1024);
        __print_container(__ideq.begin(), __ideq.end());
    }
    std::cout << "test5: " << std::endl;
    {
        tinystd::deque<__obj> __ideq{0, 1, 2, 3, 4}, __ideq2 = __ideq;

        __ideq2.insert(__ideq2.begin() + 1, 200, 10086);
        __ideq.insert(__ideq.end() - 2, 200, 10086);

        __print_container(__ideq.begin(), __ideq.end());
        __print_container(__ideq2.begin(), __ideq2.end());
    }
    std::cout << "test6: " << std::endl;
    {
        tinystd::deque<__obj> __ideq{0, 1, 2, 3, 4},  __ideq2(5, 1024);
        __print_container(__ideq.begin(), __ideq.end());
        __ideq.insert(__ideq.begin() + 1, __ideq2.begin(), __ideq2.end());
        __print_container(__ideq.begin(), __ideq.end());
    }
}