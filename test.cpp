#include <iostream>

#include "bst.hpp"

int main()
{
    Bst<int> tree;
    tree.insert(5);
    tree.insert(6);
    tree.insert(4);
    tree.insert(30);
    tree.insert(26);
    tree.insert(3);
    tree.findMax();
    tree.findMin();
    tree.find(5);
    tree.find(40);
    tree.isEmpty();
    tree.remove(26);
    tree.print();
    std::cout << "*******" << std::endl;
    Bst<int> tree2 = tree;
    tree2.print();
    std::cout << "*******" << std::endl;
    Bst<int> tree3 = std::move(tree2);
    tree3.print();
    
}
