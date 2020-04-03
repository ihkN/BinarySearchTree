#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>
#include <random>
#include <chrono>

#include "bst.hpp"

void printLine(int num=30)
{
    for(int i=0; i<num; ++i)
        std::cout << "*";
    std::cout << "\n";
}

std::vector<std::pair<int, double>> getRandPairs(long int num=100)
{
    int key, klb=0, kub=100;
    double val, vlb=0, vub=100;
    std::pair<int, double> p;
    std::vector<std::pair<int, double>> vec(num);
    
    std::random_device rd;
    std::default_random_engine eng{rd()};
    std::uniform_int_distribution<int> iunif(klb, kub);
    std::uniform_real_distribution<double> dunif(vlb, vub);
    auto idist = std::bind(iunif, eng);
    auto ddist = std::bind(dunif, eng);
    
    for(int i=0; i<num; ++i)
    {
        key = idist();
        val = ddist();
        p = std::make_pair(key, val);
        vec[i] = p;
    }    

    return vec;
}

int main()
{
   // Bst<int, std::string> tree;
   // std::cout << tree;

/*
 *    std::pair p1 = std::make_pair(12, "SR");
 *    std::pair p2 = std::make_pair(4, "QP");
 *    std::pair p3 = std::make_pair(8, "NE");
 *    tree.insert(std::make_pair(1, "AB"));
 *    tree.insert(std::make_pair(3, "GR"));
 *    tree.insert(std::make_pair(2, "DM"));
 *    tree.insert(std::make_pair(6, "RE"));
 *    tree.insert(p1);
 *    tree.insert(p2);
 *    std::cout << tree;
 *   
 *    auto f = tree.find(p2);
 *    std::cout << "f   = " << (*f).first << std::endl;
 *    f++;
 *    std::cout << "f++ = " << (*f).first << std::endl;
 *    ++f;
 *    std::cout << "++f = " << (*f).first << std::endl;
 *    f--;
 *    std::cout << "f-- = " << (*f).first << std::endl;
 *    --f;
 *    std::cout << "--f = " << (*f).first << std::endl;
 *   
 *    auto max = tree.findMax();
 *    std::cout << max.first << std::endl;
 *    auto min = tree.findMin();
 *    std::cout << min.first << ", " << min.second << std::endl;
 *    
 *    auto b = tree.begin();
 *    auto cb = tree.cbegin();
 *    std::cout << "begin: " << (*++b).first << std::endl;
 *    std::cout << "cbegin: " << (*++cb).first << std::endl;
 *
 *    std::cout << "******" << std::endl;
 *    std::cout << tree << std::endl;
 *
 *    tree.clear();
 *    std::cout << tree << std::endl;
 */

    Bst<int, double> tree;

    auto vec = getRandPairs(1000000);
    printLine();
    auto t1 = std::chrono::high_resolution_clock::now();
    for(auto p: vec) tree.insert(p);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto et = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    std::cout << "time to insert " << vec.size() << " pairs: " << et << "ms" << std::endl;
    printLine();
    
    tree.insert()
    auto p = std::make_pair(1, 1.0);
    auto itr = tree.insert(p);


}
