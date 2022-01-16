#include <vector>
#include <iostream>
#include <cassert>
#include <cstring>
#include "tinystl_tree.h"

void __test_tree_node_base_iterator();

int main() {
    std::vector<std::pair<std::string, void (*)()>> __test_cases{
        { "test the tree node base iterator... ", __test_tree_node_base_iterator },
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