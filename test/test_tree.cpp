#include <cstdlib>
#include <ctime>
#include <ostream>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstring>
#include "tinystl_pair.h"
#include "tinystl_tree.h"
#include "tinystl_functional.h"
#include "tinystl_vector.h"
#include "tinystl_iterator.h"

#define __test_avl

void __test_tree_node_base_iterator();
void __test_insert_unique();
void __test_rotate();

int main() {
    srand(time(0x0));

    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the tree node base iterator... ", __test_tree_node_base_iterator },
        { "test insert_unique...", __test_insert_unique },
        { "test insert with retote...", __test_rotate },
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
        
        for (int i = 0; i < 100; ++i) {
            int random_shit = rand();
            tree.insert_unique(random_shit);
        }

        assert(__is_sorted(tree.begin(), tree.end()));

        std::cout << "...ok" << std::endl;
    }
}
#undef __test_avl

