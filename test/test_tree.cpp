#define __test_avl
#include <cstdlib>
#include <ctime>
#include <ostream>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstring>
#include <unordered_set>
#include <set>
#include "tinystl_algobase.h"
#include "tinystl_pair.h"
#include "tinystl_tree.h"
#include "tinystl_functional.h"
#include "tinystl_vector.h"
#include "tinystl_iterator.h"
#include "__tinystl_utils.h"

void __test_tree_node_base_iterator();
void __test_insert_unique();
void __test_rotate();
void __test_insert_equal();
void __test_erase();
void __test_consrtuctor_and_operator_equal();
void __test_bound();

int main() {
    srand(time(0x0));

    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the tree node base iterator... ", __test_tree_node_base_iterator },
        { "test insert_unique...", __test_insert_unique },
        { "test insert with retote...", __test_rotate },
        { "test insert equal...", __test_insert_equal },
        { "test erase...", __test_erase },
        { "test constructor and operator equal...", __test_consrtuctor_and_operator_equal },
        { "test bound...", __test_bound },
    };

    for (const std::pair<std::string, void (*)()> &__p : __test_cases) {
        std::cout << "\n>> " << __p.first << " ========================" << std::endl;
        __p.second();
    }
}

void __test_tree_node_base_iterator() {
    typedef tinystd::__avl_tree_node_base tree_node;
    typedef tinystd::__avl_tree_base_iterator iterator;
    std::cout << "test1: " << std::endl;
    {
        tree_node nodes[5];
        std::memset(nodes, 0x0, sizeof(tree_node) * 5);
        nodes[0]._M_right = nodes + 1;
        nodes[1]._M_left = nodes + 2;
        nodes[2]._M_left = nodes + 3;
        nodes[3]._M_left = nodes + 4;
        nodes[4]._M_parent = nodes + 3;
        nodes[3]._M_parent = nodes + 2;
        nodes[2]._M_parent = nodes + 1;
        nodes[1]._M_parent = nodes;

        iterator i0(nodes); 
        i0._M_increment();
        assert(i0._M_node == nodes + 4);

        i0._M_increment();
        assert(i0._M_node == nodes + 3);

        i0._M_increment();
        assert(i0._M_node == nodes + 2); 

        i0._M_increment();
        assert(i0._M_node == nodes + 1);

        i0._M_increment();
        assert(i0._M_node == 0);

        iterator i1(nodes + 1);
        i1._M_decrement();
        assert(i1._M_node == nodes + 2);

        i1._M_decrement();
        assert(i1._M_node == nodes + 3);

        i1._M_decrement();
        assert(i1._M_node == nodes + 4);

        i1._M_decrement();
        assert(i1._M_node == nodes);

        std::cout << "...ok" << std::endl;
    }
    std::cout << "test2: " << std::endl;
    {
        tree_node nodes[7];
        std::memset(nodes, 0x0, 7 * sizeof(tree_node));
        nodes[0]._M_left = nodes + 1;
        nodes[0]._M_right = nodes + 2;
        nodes[1]._M_left = nodes + 3;
        nodes[1]._M_right = nodes + 4;
        nodes[2]._M_left = nodes + 5;
        nodes[2]._M_right = nodes + 6;
        nodes[1]._M_parent = nodes;
        nodes[2]._M_parent = nodes;
        nodes[3]._M_parent = nodes + 1;
        nodes[4]._M_parent = nodes + 1;
        nodes[5]._M_parent = nodes + 2;
        nodes[6]._M_parent = nodes + 2;

        iterator i0(nodes + 3);
        i0._M_increment();
        assert(i0._M_node == nodes + 1);

        i0._M_increment();
        assert(i0._M_node == nodes + 4);

        i0._M_increment();
        assert(i0._M_node == nodes);

        i0._M_increment();
        assert(i0._M_node == nodes + 5);

        i0._M_increment();
        assert(i0._M_node == nodes + 2);

        i0._M_increment();
        assert(i0._M_node == nodes + 6);

        iterator i1(nodes + 6);
        std::vector<tree_node *> pvec;
        for (int i = 0; i < 7; ++i) {
            pvec.push_back(i1._M_node);
            i1._M_decrement();
        }

        std::vector<tree_node *> res{ 
            nodes + 6, nodes + 2, nodes + 5,
            nodes, nodes + 4, nodes + 1, nodes + 3 
        };
        assert(pvec == res);

        std::cout << "...ok" << std::endl;
    }
}

template <typename _Key, typename _Value>
struct __pair_key_of_value {
    _Key operator()(const tinystd::pair<_Key, _Value> &p) const
        { return p.first; }
};

void __test_insert_unique() {
    std::cout << "test1(not rorate): " << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, tinystd::pair<int, std::string>, 
            __pair_key_of_value<int, std::string>, decltype(cmp)> tree;
        
        for (int i = 0; i < 10; ++i)
            tree.insert_unique({i, std::to_string(i)});
        
        auto it = tree.begin();
        for (int i = 0; i < 10; ++i) {
            assert((it++)->second == std::to_string(i));
        }
        std::cout << "...ok" << std::endl;
    }
    std::cout << "test2(not rorate): " << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, tinystd::pair<int, std::string>, 
            __pair_key_of_value<int, std::string>, decltype(cmp)> tree;
        
        int ra[7] = { 3, 1, 5, 0, 2, 4, 6 };
        for (int i = 0; i < 7; ++i) {
            tree.insert_unique({ra[i], std::to_string(ra[i])});
        }

        auto it = tree.begin();
        for (int i = 0; i < 7; ++i) {
            assert((it++)->second == std::to_string(i));
        }
        std::cout << "...ok" << std::endl;
    }
    std::cout << "test3(no rorate): " << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, tinystd::pair<int, std::string>, 
            __pair_key_of_value<int, std::string>, decltype(cmp)> tree;
        
        int ra[7] = { 3, 1, 5, 0, 2, 4, 6 };
        for (int i = 0; i < 7; ++i) {
            tree.insert_unique({ra[i], std::to_string(ra[i])});
        }
        int height[7] = { 1, 2, 1, 3, 1, 2, 1 };
        auto it = tree.begin();
        for (int i = 0; i < 7; ++i) {
            assert(((tinystd::__avl_tree_node<int> *) it._M_node)->_M_height == height[i]);
            assert((it++)->second == std::to_string(i));
        }
        std::cout << "...ok" << std::endl;
    }
}

template <typename _Iterator>
bool __is_sorted(_Iterator __first, _Iterator __last) {
    if (__first == __last) return true;
    while (tinystd::next(__first) != __last) {
        if (*__first > *tinystd::next(__first))
            return false;
        tinystd::advance(__first, 1);
    }
    return true;
}

struct return_itself {
    int operator()(int i) const 
        { return i; }
};

void __test_rotate() {
    std::cout << "test1: insert 100 elements's" << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, tinystd::pair<int, std::string>, 
            __pair_key_of_value<int, std::string>, decltype(cmp)> tree;
        
        for (int i = 0; i < 100; ++i)
            tree.insert_unique({i, std::to_string(i)});

        auto it = tree.begin();
        for (int i = 0; i < 100; ++i)
            assert(it++->second == std::to_string(i));

        std::cout << "...ok" << std::endl;
    }
    std::cout << "test2: random insert..." << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;
        for (int i = 0; i < 0xffff; ++i) {
            int random_shit = rand() % 0xffff;
            tree.insert_unique(random_shit);
        }

        assert(__is_sorted(tree.begin(), tree.end()));

        std::cout << "...ok" << std::endl;
    }
    std::cout << "test3: insert unique and assert size" << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;

        std::unordered_set<int> s;
        for (int i = 0; i < 0xffff; ++i) {
            s.insert(rand() % 0xffff);
        }
        for (int i : s) {
            tree.insert_unique(i);
        }
        assert(__is_sorted(tree.begin(), tree.end()));
        assert(tree.size() == s.size());

        std::cout << "...ok" << std::endl;
    }
}

void __test_insert_equal() {
    std::cout << "test1: insert 10 same elements" << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;

        for (int i = 0; i < 10; ++i) 
            tree.insert_equal(i);

        assert(tree.size() == 10);
        assert(__is_sorted(tree.begin(), tree.end()));

        std::cout << "...ok" << std::endl;
    }
    std::cout << "test2: insert many random shit..." << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;

        for (int i = 0; i < 0xffff; ++i) 
            tree.insert_equal(rand() % 0xff);

        assert(tree.size() == 0xffff);
        assert(__is_sorted(tree.begin(), tree.end()));

        auto begin = tree.begin();
        auto end = tree.end();
        size_t sz = 0;
        while (begin != end) {
            ++sz;
            ++begin;
        }

        assert(tree.size() == sz);

        std::cout << "...ok" << std::endl;
    }
}

void __test_erase() {
    std::cout << "test1(no rorate): erase 3 ways but still sorted..." << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;

        for (int i = 0; i < 10; ++i) {
            tree.insert_unique(i);
        }

        std::cout << "before remove 3: ";
        __print_container(tree.begin(), tree.end()); 
        tree.erase(3);
        assert(__is_sorted(tree.begin(), tree.end()));
        std::cout << "after remove 3: ";
        __print_container(tree.begin(), tree.end()); 

        std::cout << "before remove 5: ";
        __print_container(tree.begin(), tree.end()); 
        tree.erase(5);
        assert(__is_sorted(tree.begin(), tree.end()));
        std::cout << "after remove 5: ";
        __print_container(tree.begin(), tree.end()); 

        std::cout << "before remove 9: ";
        __print_container(tree.begin(), tree.end()); 
        tree.erase(9);
        assert(__is_sorted(tree.begin(), tree.end()));
        std::cout << "after remove 9: ";
        __print_container(tree.begin(), tree.end());

        std::cout << "...ok" << std::endl;
    }
    std::cout << "test2(no rorate): random erase but still sorted... " << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;

        for (int i = 0; i < 0xffff; ++i) {
            tree.insert_unique(rand() % 0xffff);
        }
        std::cout << "after insert 0xffff... size is "<< tree.size() << std::endl;
        for (int i = 0; i < 0xffff; ++i) {
            tree.erase(rand() % 0xffff);
        }
        std::cout << "after erase 0xffff... size is "<< tree.size() << std::endl;

        assert(__is_sorted(tree.begin(), tree.end()));
        std::cout << "...ok" << std::endl;

    }
}

void __test_consrtuctor_and_operator_equal() {
    std::cout << "test1: test constructor..." << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;
        
        for (int i = 0; i < 10; ++i) {
            tree.insert_unique(i);
        }

        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree2(tree);

        assert(tinystd::equal(tree.begin(), tree.end(), tree2.begin()));
        assert(tree.size() == tree.size());
        std::cout << "...ok" << std::endl;
    }
    std::cout << "test2: operator= ... " << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;
        
        for (int i = 0; i < 10; ++i) {
            tree.insert_unique(i);
        }

        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree2;

        for (int i = 0; i < 0xff; ++i) {
            tree2.insert_unique(rand() % 0xff);
        }

        tree2 = tree;

        assert(tinystd::equal(tree.begin(), tree.end(), tree2.begin()));
        assert(tree.size() == tree.size());
        std::cout << "...ok" << std::endl;
    }
}

void __test_bound() {
    std::cout << "test1: lower_bound and upper_bound" << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;
        std::set<int> s;
        
        for (int i = 0; i <= 10; i += 2) {
            tree.insert_unique(i);
            s.insert(i);
        }

// #ifdef __test_avl
//      tree.__show();
// #endif

        assert(*tree.lower_bound(0) == *s.lower_bound(0));
        assert(*tree.lower_bound(3) == *s.lower_bound(3));
        assert(*tree.upper_bound(8) == *s.upper_bound(8));
        assert(*tree.upper_bound(3) == *s.upper_bound(3));
        std::cout << "...ok" << std::endl;
    }
    std::cout << "test2: test random lower_bound" << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;
        std::set<int> s;
        
        for (int i = 0; i < 0xffff; ++i) {
            int random_shit = rand() % 0xfff;
            tree.insert_unique(random_shit);
            s.insert(random_shit);
        }

        for (int i = 0; i < 100; ++i) {
            int random_shit = rand() % 0xfff;
            auto it = s.lower_bound(random_shit);
            if (it == s.end())
                assert(tree.lower_bound(random_shit) == tree.end());
            else 
                assert(*tree.lower_bound(random_shit) == *it);
        }

        std::cout << "...ok" << std::endl;
    }
    std::cout << "test2: test random upper_bound" << std::endl;
    {
        tinystd::less<int> cmp;
        tinystd::__avl_tree<int, int, 
            return_itself, decltype(cmp)> tree;
        std::set<int> s;
        
        for (int i = 0; i < 0xffff; ++i) {
            int random_shit = rand() % 0xffff;
            tree.insert_unique(random_shit);
            s.insert(random_shit);
        }

        for (int i = 0; i < 100; ++i) {
            int random_shit = rand() % 0xffff;
            auto it = s.upper_bound(random_shit);
            if (it == s.end())
                assert(tree.upper_bound(random_shit) == tree.end());
            else {
                assert(*tree.upper_bound(random_shit) == *it);
            }
        }

        std::cout << "...ok" << std::endl;
    }
}

#undef __test_avl
