#ifndef RBTREE_SDFSEWRSDPGSCP
#define RBTREE_SDFSEWRSDPGSCP
/*
   Code is based on the dicussion of 2 3 4, 2 3 and red-black trees at http://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf
   and the java implementation at http://www.cs.princeton.edu/~rs/talks/LLRB/Java/. This code below uses only the 2 3 4 species
   implementation. 
 */
// TODO: I believe Node::height and Node::N and RedBlackTree::k iRedBlackTree::heightBlack are all only used by the drawing code and can be deleted and the functions
//that set these values?
template<typename Key, typename Value>  class RedBlackTree {
        
  private:
    enum { BLACK = false, RED = true};
   
    class Node {
      public:    
        Key   key;            // key
        Value value;          // associated data
        Node *left;            // left...
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
   
   Value get(Node *p, Key key);

   Key min(Node *p)
   {
      return (p == 0) ? p->key : min(p->left);
   }

   Key max(Node *p)
   {
      return (p == 0) ? p->key : max(p->right);
   }

   Node *insert(Node *p, Key key, Value value);
      
  Value get(Node x, Key key);

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
  
  Node *rotateLeft(Node *p);

 Node *rotateRight(Node *p);

 Node *moveRedLeft(Node *p);

 Node *moveRedRight(Node *p);
 Node *fixUp(Node *p);

 public:

   RedBlackTree() {};
   
   bool contains(Key key)
   {  return get(key) != 0;  }

   Value get(Key key)
   {  return get(root, key);  }

   void put(Key key, Value value)
   {
      root = insert(root, key, value);
      root->color = BLACK;
   }

   Key min()
   {  
      return (root == 0) ? 0 : min(root);
   }

   Key max()
   {  
      return (root == 0) ? 0 : max(root);
   }
 
};

template<typename Key, typename Value>  
typename RedBlackTree<Key, Value>::Node * RedBlackTree<Key, Value>::rotateLeft(Node *p)
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
{  // Assuming that h is red and both p->left and p->left->left
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
{  // Assuming that h is red and both p->right and p->right->left
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
typename RedBlackTree<Key, Value>::Node * RedBlackTree<Key, Value>::fixUp(Node *p)
{
   if (isRed(p->right))
      p = rotateLeft(p);

   if (isRed(p->left) && isRed(p->left->left))
      p = rotateRight(p);

   if (isRed(p->left) && isRed(p->right))
      colorFlip(p);

   return p;
}
 
//TODO: return a pair<> or return bool and value by reference
template<typename Key, typename Value>  Value RedBlackTree<Key, Value>::get(Node *p, Key key)
{
    
/* alternate, recursive code
   if (p == 0)    return 0;
   if (key == p->key) return p->value;
   if (key < p->key)  return get(p->left,  key);
   else              return get(p->right, key);
*/
   // non-recursive code:
   while (p != 0) {
       if      (key < p->key) p = p->left;
       else if (key > p->key) p = p->right;
       else              return p->val;
   }
   return 0;
  
}


template<typename Key, typename Value>   typename RedBlackTree<Key, Value>::Node *
RedBlackTree<Key, Value>::insert(RedBlackTree<Key, Value>::Node *p, Key key, Value value)
{ 
   if (p == 0) 
      return new Node(key, value);

   // 2 3 4 tree implementation: do the equivalent of splitting a four node.
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

   if (isRed(p->left) && isRed(p->left))
      p = rotateRight(p);

   return p;
}
#endif
