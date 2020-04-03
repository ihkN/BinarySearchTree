#include <iostream>
#include <algorithm>
#include <iterator>
#include <utility>

#include "bst.hpp"

int main()
{
    Bst<int, std::string> tree;
    std::cout << tree;

    std::pair p1 = std::make_pair(12, "SR");
    std::pair p2 = std::make_pair(4, "QP");
    std::pair p3 = std::make_pair(8, "NE");
    tree.insert(std::make_pair(1, "AB"));
    tree.insert(std::make_pair(3, "GR"));
    tree.insert(std::make_pair(2, "DM"));
    tree.insert(std::make_pair(6, "RE"));
    tree.insert(p1);
    tree.insert(p2);
    std::cout << tree;
   
    auto f = tree.find(p2);
    std::cout << "f   = " << (*f).first << std::endl;
    f++;
    std::cout << "f++ = " << (*f).first << std::endl;
    ++f;
    std::cout << "++f = " << (*f).first << std::endl;
    f--;
    std::cout << "f-- = " << (*f).first << std::endl;
    --f;
    std::cout << "--f = " << (*f).first << std::endl;
   
    auto max = tree.findMax();
    std::cout << max.first << std::endl;
    auto min = tree.findMin();
    std::cout << min.first << ", " << min.second << std::endl;
    
    auto b = tree.begin();
    auto cb = tree.cbegin();
    std::cout << "begin: " << (*++b).first << std::endl;
    std::cout << "cbegin: " << (*++cb).first << std::endl;

    std::cout << "******" << std::endl;
    std::cout << tree << std::endl;

    tree.clear();
    std::cout << tree << std::endl;
}
