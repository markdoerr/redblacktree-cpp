#ifndef RBTREE_SDFSEWRSDPGSCP
#define RBTREE_SDFSEWRSDPGSCP
#include <exception>
/*
 * This code is like rb.h but also contains the invariant self-checking logic contained in the original code
 * java code at http://www.cs.princeton.edu/~rs/talks/LLRB/Java/RedBlackBST.java  
 */

class KeyDoesnotExist :  public std::exception {
public:
  virtual const char* what() const throw()
  {
    return "Key does not exist in tree";
  }
};

template<typename Key, typename Value>  class RedBlackTree {
        
  private:
  enum { BLACK = false, RED = true}; 

   class  Node {
      public:    
        Key   key;            // key
        Value value;          // its associated data
        Node *left;           // left...
        Node *right;          // ...and right subtrees
        bool color;           // color of parent link
        int N;         // These threee members are used for invariant checking 
        int height;    // to show that the tree is as balanced as possible.
        double xc, yc; // 

        Node(Key key, Value value)
        {
          this->key   = key;
          this->value = value;
          this->color = RED;
          this->N = 1;
          this->height = 1;
        }
   };

   Node *root;           // root of the BST
   
   Value get(Node *p, Key key) throw(KeyDoesnotExist);

   Node *getInOrderSuccessorNode(Node *p);
   
   void DestroyTree(Node *root);
         
   /*
    * Returns minimum key of subtree rooted at p
    */ 
   Key min(Node *p)
   {
      return (p->left == 0) ? p->key : min(p->left);
   }

   Key max(Node *p)
   {
      return (p->right == 0) ? p->key : max(p->right);
   }

   Node *insert(Node *p, Key key, Value value);
      
  bool isRed(Node *p)
  {
      return (p == 0) ? false : (p->color == RED);
  }

  void colorFlip(Node *p)
  {  
      p->color        = !p->color;
      p->left->color  = !p->left->color;
      p->right->color = !p->right->color;
  }

  int size(Node *p)
  { 
     return (p == 0) ?  0 : p->N;
  }

  int height(Node *p)
  { 
      return (p == 0) ? 0 : p->height;
  }      

  Node *setN(Node *p)
  {
     p->N = size(p->left) + size(p->right) + 1;

     if (height(p->left) > height(p->right)) {

             p->height = height(p->left) + 1;

     } else {
              p->height = height(p->right) + 1;
     }

     return p;
  }

  bool isBST() 
  {  // Is this tree a BST?
     return isBST(root, min(), max());
  }

  bool isBST(Node *p, Key min, Key max)
  {  
     // Are all the values in the BST rooted at x between min and max,
     // and does the same property hold for both subtrees?
     if (p == 0) 
         return true;

     if (less(p->key, min) || less(max, p->key)) 
         return false;

     return isBST(p->left, min, p->key) && isBST(p->right, p->key, max);
  } 

  bool is234() { return is234(root); }

  bool is234(Node *p)
  {  
     // Does the tree have no red right links, and at most two (left)
     // red links in a row on any path?
     if (p == 0) 
          return true;

     if (isRed(p->right)) 
          return false;

     if (isRed(p)) {

       if (isRed(p->left)) {

         if (isRed(p->left->left)) {

            return false;
         }
       }
     }     

     return is234(p->left) && is234(p->right);
  } 

  bool isBalanced()
  { 
     // Do all paths from root to leaf have same number of black edges?
     int black = 0;     // number of black links on path from root to min

     Node *p = root;

     while (p != 0) {

        if (!isRed(p)) {

            black++;
        }  

        p = p->left;
     }

     return isBalanced(root, black);
  }


  Node *rotateLeft(Node *p);
  Node *rotateRight(Node *p);
  
  Node *moveRedLeft(Node *p);
  Node *moveRedRight(Node *p);

  Node *deleteMax(Node *p);
  Node *deleteMin(Node *p);
  
  Node *fixUp(Node *p);

  Node *remove(Node *p, Key key);
  
  template<typename Functor> void traverse(Functor f, Node *root);

 public:

   int rootRank()
   { 
       return (root == 0) ?  0 : size(root->left);
   }

   int height()
   {  return height(root);  }

   /*
   int heightB()
   {  return heightBLACK;  }
   */
    
   int size()
   {  return size(root);  }

   RedBlackTree() { root = 0; }
   
  ~RedBlackTree();
   
   bool contains(Key key)
   {  return get(key) != 0;  }

   Value get(Key key)
   {  return get(root, key);  }

   void put(Key key, Value value)
   {
      root = insert(root, key, value);
      root->color = BLACK;
   }

   template<typename Functor> void traverse(Functor f);

   Key min()
   {  
      return (root == 0) ? 0 : min(root);
   }

   Key max()
   {  
      return (root == 0) ? 0 : max(root);
   }

   void deleteMin()
   {
      root = deleteMin(root);
      root->color = BLACK;
   }

   void deleteMax()
   {
      root = deleteMax(root);
      root->color = BLACK;
   }
    
   void remove(Key key)
   { 
      if (root == 0) return;

      root = remove(root, key);

      if (root != 0) { 
        root->color = BLACK;
      }
   }

   // Integrity checks
   bool check() 
   {  // Is this tree a red-black tree?
      return isBST() && is234() && isBalanced();
   }
 

};

template<typename Key, typename Value> inline RedBlackTree<Key, Value>::~RedBlackTree()
{
   DestroyTree(root);    
}
/*
 *  Do post order traversal deleting nodes.
 */
template<typename Key, typename Value> void RedBlackTree<Key, Value>::DestroyTree(Node *current)
{
    if (current == 0) return;
    
    DestroyTree(current->left); 
    DestroyTree(current->right);
    
    delete current;
}    

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::rotateLeft(Node *p)
{  // Make a right-leaning 3-node lean to the left.
   Node  *x = p->right;
   p->right = x->left;
   x->left  = p; 
   x->color = x->left->color;                   
   x->left->color = RED;                     
   return x;
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node * RedBlackTree<Key, Value>::rotateRight(Node *p)
{  // Make a left-leaning 3-node lean to the right.
   Node *x = p->left;
   p->left = x->right;
   x->right = p;
   x->color       = x->right->color;                   
   x->right->color = RED;                     
   return x;
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node * RedBlackTree<Key, Value>::moveRedLeft(Node *p)
{  // Assuming that p is red and both p->left and p->left->left
   // are black, make p->left or one of its children red
   colorFlip(p);

   if (isRed(p->right->left)) { 

      p->right = rotateRight(p->right);
      p = rotateLeft(p);
      colorFlip(p);
   }
  return p;
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node * RedBlackTree<Key, Value>::moveRedRight(Node *p)
{  // Assuming that p is red and both p->right and p->right->left
   // are black, make p->right or one of its children red
   colorFlip(p);
   
   if (isRed(p->left->left))
   { 
      p = rotateRight(p);
      colorFlip(p);
   }
   return p;
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::fixUp(Node *p)
{
   if (isRed(p->right))
      p = rotateLeft(p);

   if (isRed(p->left) && isRed(p->left->left))
      p = rotateRight(p);

   if (isRed(p->left) && isRed(p->right)) // four node
      colorFlip(p);

   return p;
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::deleteMax(Node *p)
{ 
   if (isRed(p->left))
      p = rotateRight(p);

   if (p->right == 0)
      return 0;

   if (!isRed(p->right) && !isRed(p->right->left))
      p = moveRedRight(p);

   p->right = deleteMax(p->right);

   return fixUp(p);
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::deleteMin(Node *p)
{ 
   if (p->left == 0)
      return 0;

   if (!isRed(p->left) && !isRed(p->left->left))
      p = moveRedLeft(p);

   Node *ptemp = p->left; // <-- req'd for C++ 

   p->left = deleteMin(p->left);

   delete ptemp; // req'd for C++

   return fixUp(p);
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::remove(Node *p, Key key)
{ 
   if (key < p->key) {

      if (!isRed(p->left) && !isRed(p->left->left)) {

         p = moveRedLeft(p);
      } 

      p->left = remove(p->left, key);

   } else {

      if (isRed(p->left)) {

         p = rotateRight(p);
      }

      if ((key == p->key) && (p->right == 0)) {
         // should  
         return 0;
      }  

      if (!isRed(p->right) && !isRed(p->right->left)) {

         p = moveRedRight(p);
      } 

      if (key == p->key) {
         /*  
         p->value = get(p->right, min(p->right)); // Set the value of p to be value in-order successor of key

         p->key = min(p->right);    // Set key of p to be key of in-order successor     
         */
         /* added instead of code above */
         Node *successor = getInOrderSuccessorNode(p);
         p->value  = successor->value;  // Assign p in-order successor key and value
         p->key    = successor->key;

         p->right = deleteMin(p->right); // <-- deleteMin() appears to be where C++ delete should occur. 

      } else {

         p->right = remove(p->right, key);
      }
   }

   return fixUp(p);
}

/*
 * Returns key's associated value. The search for key starts in the subtree rooted at p.
 */
template<typename Key, typename Value>  Value RedBlackTree<Key, Value>::get(Node *p, Key key) throw(KeyDoesnotExist)
{
    
/* alternate recursive code
   if (p == 0) {   ValueNotFound(key);}
   if (key == p->key) return p->value;
   if (key < p->key)  return get(p->left,  key);
   else              return get(p->right, key);
*/
   // non-recursive version
   while (p != 0) {
       if      (key < p->key) p = p->left;
       else if (key > p->key) p = p->right;
       else             return p->value;
   }
   
   throw KeyDoesnotExist();
}

template<typename Key, typename Value> inline
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::getInOrderSuccessorNode(RedBlackTree<Key, Value>::Node *p)
{
  p = p->right;

  while (p != 0) {

      p = p->left;
  }
  return p;
}

template<typename Key, typename Value>
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::insert(RedBlackTree<Key, Value>::Node *p, Key key, Value value)
{ 
   if (p == 0) 
      return new Node(key, value);
 
   /*
    * This test checks for a 4 node (viewing the left-leaning rb tree as a 2 3 4 tree) and splits it.
    */
    if (isRed(p->left) && isRed(p->right))
        colorFlip(p);

   if (key == p->key)
      p->value = value;
   else if (key < p->key) 
      p->left = insert(p->left, key, value); 
   else 
      p->right = insert(p->right, key, value); 

   if (isRed(p->right))
      p = rotateLeft(p);

   if (isRed(p->left) && isRed(p->left->left))
      p = rotateRight(p);

   return p;
}
template<typename Key, typename Value> template<typename Functor> inline void RedBlackTree<Key, Value>::traverse(Functor f)
{
   return traverse(f, root);
}

/* in order traversal */
template<typename Key, typename Value>  template<typename Functor> void RedBlackTree<Key, Value>::traverse(Functor f, RedBlackTree<Key, Value>::Node *root)
{
  if (root == 0) { 
         return; 
  }

  traverse(f, root->left);

  f(root->value); 

  traverse(f, root->right);
}
#endif
