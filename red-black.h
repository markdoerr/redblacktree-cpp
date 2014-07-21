#ifndef REDBLACK_SDFSEWRSDPGSCP
#define REDBLACK_SDFSEWRSDPGSCP
#include <exception>

/*
   Code is based on the dicussion of 2 3 4 trees, 2 3 trees and red-black trees found at http://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf
   and the corresponding java implementation found at http://www.cs.princeton.edu/~rs/talks/LLRB/Java/. 
   The code below uses only the 2 3 4 species of implementation. 

The relationship between 2 3 4 trees and red black tree is also discussed at:
http://cw.felk.cvut.cz/lib/exe/fetch.php/courses/a4m33pal/paska12x.pdf and
http://www.cs.princeton.edu/courses/archive/sprnullptr7/cos226/lectures/balanced.pdf

A C++ implementation is also at: 
http://www.teachsolaisgames.com/articles/balanced_left_leaning.html
 */

// Comment to myself: It appears Node::height and Node::N and RedBlackTree::heightBlack are used by only the drawing code and can be
// safely deleted
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

        Node(Key key, Value value)
        {
          this->key   = key;
          this->value = value;
          this->color = RED;
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
      return (p->left == nullptr) ? p->key : min(p->left);
   }

   Key max(Node *p)
   {
      return (p->right == nullptr) ? p->key : max(p->right);
   }

   Node *insert(Node *p, Key key, Value value);
      
  bool isRed(Node *p)
  {
      return (p == nullptr) ? false : (p->color == RED);
  }

  void colorFlip(Node *p)
  {  
      p->color        = !p->color;
      p->left->color  = !p->left->color;
      p->right->color = !p->right->color;
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

   RedBlackTree() { root = nullptr; }
   
  ~RedBlackTree() { DestroyTree(root); }
   
   bool contains(Key key)
   {  return get(key) != nullptr;  }

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
      return (root == nullptr) ? nullptr : min(root);
   }

   Key max()
   {  
      return (root == nullptr) ? nullptr : max(root);
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
      if (root == nullptr) return;

      root = remove(root, key);

      if (root != nullptr) { 
        root->color = BLACK;
      }
   }

};

/*
 *  Do post order traversal deleting nodes.
 */
template<typename Key, typename Value> void RedBlackTree<Key, Value>::DestroyTree(Node *current)
{
    if (current == nullptr) return;
    
    DestroyTree(current->left); 
    DestroyTree(current->right);
    
    delete current;
}    

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::rotateLeft(Node *p)
{  
   // Make a right-leaning 3-node lean to the left.
   Node  *x = p->right;

   p->right = x->left;

   x->left  = p; 

   x->color = x->left->color;                   

   x->left->color = RED;                     

   return x;
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node * RedBlackTree<Key, Value>::rotateRight(Node *p)
{  
   // Make a left-leaning 3-node lean to the right.
   Node *x = p->left;

   p->left = x->right;

   x->right = p;

   x->color  = x->right->color;                   

   x->right->color = RED;                     

   return x;
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node * RedBlackTree<Key, Value>::moveRedLeft(Node *p)
{  
  // Assuming that p is red and both p->left and p->left->left
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
{  
   // Assuming that p is red and both p->right and p->right->left
   // are black, make p->right or one of its children red
   colorFlip(p);
   
   if (isRed(p->left->left)) { 

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

   if (p->right == nullptr)
      return nullptr;

   if (!isRed(p->right) && !isRed(p->right->left))
      p = moveRedRight(p);

   p->right = deleteMax(p->right);

   return fixUp(p);
}

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::deleteMin(Node *p)
{ 
   if (p->left == nullptr)
      return nullptr;

   if (!isRed(p->left) && !isRed(p->left->left))
      p = moveRedLeft(p);

   p->left = deleteMin(p->left);

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

      if ((key == p->key) && (p->right == nullptr)) {

         /* From code at http://www.teachsolaisgames.com/articles/balanced_left_leaning.html
          * Taken from the LeftLeaningRedBlack::DeleteRec method
          */
         delete p;    
         return nullptr;
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

         p->right = deleteMin(p->right); 

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
   while (p != nullptr) {
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
  
  while (p->left != nullptr) {

      p = p->left;
  }

  return p;
}

template<typename Key, typename Value>
typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::insert(RedBlackTree<Key, Value>::Node *p, Key key, Value value)
{ 
   if (p == nullptr) 
      return new Node(key, value);
 
   /* We view the left-leaning red black tree as a 2 3 4 tree. So first check if p is a
    * 4 node and needs to be "split" by flipping colors.  */
   if (isRed(p->left) && isRed(p->right))
       colorFlip(p);

   if (key == p->key)     /* if key already exists, overwrite its value */
      p->value = value;
   else if (key < p->key) /* otherwise recurse */
      p->left = insert(p->left, key, value); 
   else 
      p->right = insert(p->right, key, value); 

   /* rebalance tree */
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
  if (root == nullptr) { 
         return; 
  }

  traverse(f, root->left);

  f(root->value); 

  traverse(f, root->right);
}
#endif
