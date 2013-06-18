/*
    Code is based on the dicussion of 2 3 4, 2 3 and red-black trees at http://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf
    and the association java implementation at http://www.cs.princeton.edu/~rs/talks/LLRB/Java/. This code
    uses only the 2 3 4 species of implementation.
 */

template<typename Key, typename Value>  class RedBlackTree {
        
  private:
      enum { BLACK = false, RED = true};
   /*
   const int BST = 0;      // binary search tree: unbalanced
   const int TD234 = 1;    // 2 3 4 implementation technique   
   const int BU23 = 2;     // 2 3 implementation technique   
   */
   
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
          this->N = 1;
          this->height = 1;
        }
        int N;            // number of nodes in tree rooted here
        int height;       // height of tree rooted here
        double xc, yc;    // for drawing
    
   };


   Node *root;            // root of the BST
   int k;                // ordinal for drawing
   int heightBLACK;      // black height of tree 

   int size(Node *p)
   { 
      return (p == 0) ? 0 : p->N;
   }

   int height(Node *p)
   { 
      return (p == 0) ? 0 : p->height;  
   }

   Value get(Node *p, Key key);

   Key min()
   {  
      return (root == 0) ? 0 : min(root);
   }

   Key min(Node *p)
   {
      return (p == 0) ? p->key : min(p->left);
   }

   Key max(Node p)
   {
      return (p == 0) ? p->key : max(p->right);
   }

   Node *insert(Node *p, Key key, Value value);

   int height(Node p)
   { 
      return (p == 0) ? 0 : p->height;
   }

        
   Node *setN(Node *p)
   {
      p->N = size(p->left) + size(p->right) + 1;

      if (height(p->left) > height(p->right)) p->height = height(p->left) + 1;
      else                                    p->height = height(p->right) + 1;
      return p;
   }
      
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
  
  Node *rotateLeft(Node *p)
  {  // Make a right-leaning 3-node lean to the left.
     Node *x = p->right;
     p->right = x->left;
     x->left = setN(p);
     x->color      = x->left->color;                   
     x->left->color = RED;                     
     return setN(x);
  }

  Node *rotateRight(Node *p)
  {  // Make a left-leaning 3-node lean to the right.
     Node *x = p->left;
     p->left = x->right;
     x->right = setN(p);
     x->color       = x->right->color;                   
     x->right->color = RED;                     
     return setN(x);
  }



 public:

   RedBlackTree() {};

   int rootRank()
   { 
       return (root == 0) ? 0 : size(root.left);
   }

   int height()
   {  return height(root);  }

   int heightB()
   {  return heightBLACK;  }

   bool contains(Key key)
   {  return get(key) != 0;  }

   Value get(Key key)
   {  return get(root, key);  }

   void put(Key key, Value value)
   {
      root = insert(root, key, value);
      if (isRed(root)) heightBLACK++;
      root->color = BLACK;
   }

 
};

template<typename Key, typename Value>  Value RedBlackTree<Key, Value>::get(Node x, Key key)
{
   if (x == 0)    return 0;
   if (key == x.key) return x.value;
   if (key < x.key)  return get(x.left,  key);
   else              return get(x.right, key);
}

template<typename Key, typename Value>  Value RedBlackTree<Key, Value>::get(Node *p, Key key)
{
    if (p == 0) {

       return 0;
    }

    if (key == p->key) { 

        return p->value;
    }

    if (key < p->key) {

         return get(p->left,  key);

    }   else  {

          return get(p->right, key);
    }
}


template<typename Key, typename Value>   typename RedBlackTree<Key, Value>::Node *RedBlackTree<Key, Value>::insert(RedBlackTree<Key, Value>::Node *p, Key key, Value value)
{ 
   if (p == 0) 
      return new Node(key, value);

   // 2 3 4 tree implementation
 /* if (species == TD234) */
    if (isRed(p->left) && isRed(p->right))
        colorFlip(p);

   if (key == p->key)
      p->value = value;
   else if (key < p->key) 
      p->left = insert(p->left, key, value); 
   else 
      p->right = insert(p->right, key, value); 

 /* if (species == BST) return setN(h); */

   if (isRed(p->right))
      p = rotateLeft(p);

   if (isRed(p->left) && isRed(p->left))
      p = rotateRight(p);

   return setN(p);
}


