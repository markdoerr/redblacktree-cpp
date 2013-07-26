/* 
 * File:   main.cpp
 * Author: kurt
 *
 * Created on June 17, 2013, 1:09 PM
 */

#include <cstdlib>
#include "rb.h"
#include <iostream>


/*
 * 
 */

inline void int_out(int i)
{
  std::cout << i << ' ';
}

int main(int argc, char** argv) 
{
    RedBlackTree<int, int> tree;
    
    for (int i = 1; i < 25; i++) {
    
        tree.put(i, i);
    }
    
       
    tree.traverse(int_out);
    
    std::cout << "\n------" << std::endl;
    

    for (int i = 1; i < 25; i++) {
    
        tree.remove(i);
        tree.traverse(int_out);
        std::cout << "\n---------"   << std::endl;
    }
    
    
    return 0;
}

