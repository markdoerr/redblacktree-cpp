/* 
 * File:   main.cpp
 * Author: kurt
 *
 * Created on June 17, 2013, 1:09 PM
 */

#include <cstdlib>
#include "red-black.h"
#include <iostream>
#include <vector>

using namespace std;

inline void int_out(int i)
{
  std::cout << i << ' ';
}

int get_random_int()
{
static bool initialized = false;

   if (!initialized) {

      srand (time(NULL));
      initialized = true;
   } 

   return rand() % 100 + 1;
}

int main(int argc, char** argv) 
{
    RedBlackTree<int, int> tree;
    int tree_size = 50;
  
    vector<int> tree_values(tree_size);

   
    for (auto i = 0; i < tree_size; i++) {

        tree_values[i] = get_random_int();
 
        tree.put(tree_values[i], tree_values[i]);
    }
       
    tree.traverse(int_out);
    
    std::cout << "\n------" << std::endl;
    

    for (auto i = 0; i < tree_size; i++) {
        
        int remove_value = tree_values[i];
 
        tree.remove(remove_value);

        tree.traverse(int_out);

        std::cout << "\n---------"   << std::endl;
    }
    
    return 0;
}
