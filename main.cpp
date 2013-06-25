/* 
 * File:   main.cpp
 * Author: kurt
 *
 * Created on June 17, 2013, 1:09 PM
 */

#include <cstdlib>
#include "rb.h"
#include "treeprinter.h"
#include <iostream>

using namespace std;

/*
 * 
 */

int main(int argc, char** argv) 
{
    RedBlackTree<int, int> tree;
    
    for (int i = 1; i < 25; i++) {
    
        tree.put(i, i);
    }
    
    RBPrinter tree_printer(cout);
    
    tree.traverse(tree_printer);

    for (int i = 1; i < 25; i++) {
    
        tree.remove(i);
        tree.traverse(tree_printer);
        cout << "\n---------"   << endl;
    }
    
    
    return 0;
}

