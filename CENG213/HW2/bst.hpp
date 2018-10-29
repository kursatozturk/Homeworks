  #ifndef _BIN_SEARCH_TREE_H_
  #define _BIN_SEARCH_TREE_H_

  #include <functional>
  #include <cstddef>
  #include <stack>
  #include <list>
  #include <ostream>
  //do not add any other library
  //modify parts as directed by assignment text & comments here

  template <typename Key, typename Object, 
            typename Comparator=std::less<Key> >
  class BinarySearchTree 
  {
    private: //do not change
      struct BinaryNode //node definition: a dependent type
      {
        Key key; //keys will be kept constant
        Object data; //objects that are referred to by keys may change
        BinaryNode * left;
        BinaryNode * right;
        size_t height; //height info should be updated per insert & delete

        BinaryNode(const Key &, const Object &,
                   BinaryNode *, BinaryNode *, size_t =0);
      };

    public: //do not change except for your own private utility functions
      class Iterator //iterator class will encapsulate the location within the BST
      {
        public:
          Iterator( ); //dummy constructor for type declaration purposes
          Iterator & operator++( ); //inorder increment
   
          Object & operator*( );  //update data
          const Object & operator*( ) const; //view data

          bool operator==(const Iterator & rhs) const; //compare iterators
          bool operator!=(const Iterator & rhs) const; //compare iterators
        
        private:
          BinaryNode * current; //position
          const BinaryNode * root; //for error check not implemented
          std::stack<BinaryNode *> s; //will be used to conduct in order traversal if
          bool useStack; //this variable is set to true, ignored in == and !=
          
        private:
          Iterator(BinaryNode *, const BinarySearchTree &, bool =true);
          //other private utility functions can be declared by you

        friend class BinarySearchTree<Key, Object, Comparator>; 
      };

    public: //do not change
      BinarySearchTree( ); //empty tree
      ~BinarySearchTree( ); //reclaim all dyn allocated mem

      void insert(const Key &, const Object &); //to insert new key,item
      void remove(const Key &); //remove the node with the key value (and also data) 

    public: //do not change
      Iterator find(const Key &) const; //single item
      std::list<Iterator> find(const Key &, const Key &) const;//range queries

      Iterator begin( ) const; //inorder begin
      Iterator end( ) const; //dummy NULL iterator

    public: //do not change
      int height( ) const; //return height of the tree
      size_t size( ) const; //return the number of items in the tree
      bool empty( ) const; //return whether the tree is empty or not
      void print(std::ostream &) const;
    
    private: //do not change
      BinaryNode * root; //designated root
      size_t nodes; //number of nodes 
      Comparator isLessThan; //function object to compare keys

    private:
      /* private utility functions that are implemented */
      void makeEmpty(BinaryNode * &);

      BinaryNode * find(const Key &, BinaryNode *) const;
      int height(BinaryNode *) const;
      void print(BinaryNode *, std::ostream &) const;

      template <typename T> //static utility function
      static const T & max(const T &, const T &);

      //balancing functions 
      void rotateWithLeftChild(BinaryNode * &);
      void rotateWithRightChild(BinaryNode * &);
      void doubleWithLeftChild(BinaryNode * &);
      void doubleWithRightChild(BinaryNode * &);
      
      //you may add your own private utility functions down here
    void inserth(const Key &, const Object &, BinaryNode*&,size_t&);
    void removeh(const Key &,BinaryNode*&);
    void balance(const Key& k,BinaryNode*& p,bool left = true, bool right = true);
    void swap(BinaryNode*&);
    private: //not copiable, DO NOT IMPLEMENT or change
      BinarySearchTree(const BinarySearchTree &);
      const BinarySearchTree & operator=(const BinarySearchTree &);
  };

  //node constructor, implemented do not change
  template <typename K, typename O, typename C>
  BinarySearchTree<K,O,C>::BinaryNode::
  BinaryNode(const K & _k, const O & _d,
             BinaryNode * _l, BinaryNode * _r, size_t _h)
    : key(_k), data(_d), left(_l), right(_r), height(_h)
  {
  }

  //default constructor, implemented do not change
  template <typename K, typename O, typename C>
  BinarySearchTree<K,O,C>::BinarySearchTree( )
    : root(NULL), nodes(0) 
  {
  }

  //destructor, implemented do not change
  template <typename K, typename O, typename C>
  BinarySearchTree<K,O,C>::~BinarySearchTree( )
  {
    makeEmpty(root);
  }

  //private utility function for destructor, do not change
  template <typename K, typename O, typename C>
  void
  BinarySearchTree<K,O,C>::makeEmpty(BinaryNode * & t)
  {
    if (t != NULL)
    {
      makeEmpty(t->left);
      makeEmpty(t->right);
      delete t;
      --nodes;
    }

    t = NULL;
  }

  //public function to insert into BST, IMPLEMENT
  template <typename K, typename O, typename C>
  void
  BinarySearchTree<K,O,C>::insert(const K & k, const O & x)
  {
    inserth(k,x,this->root,nodes);
  }

  template <typename K, typename O, typename C>
  void 
  BinarySearchTree<K,O,C>::inserth(const K & k, const O & x, BinaryNode* &p,size_t &nodes){

    if(p == NULL){
      p = new BinaryNode(k,x,NULL,NULL);
      nodes++;
      return;
    }
    else if(isLessThan(k,p->key)){
            inserth(k,x, p->left,nodes);
            if(height(p->left) - height(p->right) == 2)
            if(p->left && isLessThan(k,p->left->key))
               rotateWithLeftChild(p);
            else
               doubleWithLeftChild(p);  
      
    }
    
    else if(isLessThan(p->key,k)){
               inserth(k,x, p->right,nodes);
               if(height(p->right) - height(p->left) == 2)
               if(p->right && isLessThan(p->right->key,k))
                  rotateWithRightChild(p);
               else
                  doubleWithRightChild(p);
    } 
    else{
      p->data = x;
      return;
    }

      p->height = max(height(p->left), height(p->right)) + 1;

  }

  //public function to remove key, IMPLEMENT
  template <typename K, typename O, typename C>
  void
  BinarySearchTree<K,O,C>::remove(const K & k)
  {
    removeh(k,this->root);
  }
  template <typename K, typename O, typename C>
  void
  BinarySearchTree<K,O,C>::balance(const K& k,BinaryNode*& p,bool left,bool right){
        if(right)
        if(height(p->right) - height(p->left) == 2){
          if(height(p->right->left) > height(p->right->right))
            doubleWithRightChild(p);
          else
            rotateWithRightChild(p);
        }
        if(left)
        if(height(p->left) - height(p->right) == 2){
          if(height(p->left->right) > height(p->left->left))
            doubleWithLeftChild(p);  
          else
            rotateWithLeftChild(p);
        }

  }
  template <typename K, typename O, typename C>
  void
  BinarySearchTree<K,O,C>::swap(BinaryNode*& p){
    BinaryNode *a = p;
    BinaryNode *t = p->right;
    BinaryNode *pr = t;
    while(t->left){
      pr = t;
      t = t->left;}
      if( pr == t){
        BinaryNode *tmp(t->right);
        t->right = a;
        a->right = tmp;
        t->left = a->left;
        a->left = NULL;
        p = t;
      }
      else{
        BinaryNode *tmp(t->right);
        t->right = a->right;
        a->right = tmp;
        t->left = a->left;
        a->left = NULL;
        pr->left = a;
        p = t;
      }

}
  template <typename K, typename O, typename C>
  void
  BinarySearchTree<K,O,C>::removeh(const K & k,BinaryNode* &p){
    if (p == NULL){
          return;
    }
    else if (isLessThan(k, p->key)){
      removeh(k,p->left);
      balance(k,p,false,true);
    }
    else if (isLessThan(p->key,k)){
      removeh(k,p->right);
      balance(k,p,true,false);
    }
    else{
      if( p->left ==NULL && p->right == NULL){
        delete p;
        p = NULL;
        nodes--;
        return;
      }
      else if( p->left ==NULL || p->right == NULL){
        if(p->left){
          BinaryNode *t = p->left;
          delete p;
          p = t;
          nodes--;
          return;
        }
        else if(p->right){
          BinaryNode *t = p->right;
          delete p;
          p = t;
          nodes--;
          return;
        }
      }
      else{
        swap(p);
        removeh(k,p->right);
        balance(k,p,true,false);
        }
    }

    p->height = max(height(p->left), height(p->right)) + 1;
} 
  //public function to search elements, do not change
  template <typename K, typename O, typename C>
  typename BinarySearchTree<K,O,C>::Iterator
  BinarySearchTree<K,O,C>::find(const K & key) const
  {
    BinaryNode * node = find(key, root);

    if (node == NULL)
    {
      return end();
    }
    else
    {               //not inorder iterator
      return Iterator(node, *this, false);
    }
  }

  /*
   * private utility function to search elements
   * do not change
   */
  template <typename K, typename O, typename C>
  typename BinarySearchTree<K,O,C>::BinaryNode *
  BinarySearchTree<K,O,C>::find(const K & key, BinaryNode * t) const
  {
    if (t == NULL)
    {
      return NULL;
    }
    else if (isLessThan(key, t->key))
    {
      return find(key, t->left);
    }
    else if (isLessThan(t->key, key))
    {
      return find(key, t->right);
    }
    else //found
    {
      return t;
    }
  }

  //range queries those within range are inserted to the list
  //IMPLEMENT
  template <typename K, typename O, typename C>
  std::list<typename BinarySearchTree<K,O,C>::Iterator>
  BinarySearchTree<K,O,C>::find(const K & lower, const K & upper) const
  {
    std::list<Iterator> its;
    Iterator it(this->root,*this);
    Iterator ends = this->end();
    while( it != ends && isLessThan(it.current->key,lower)){
      ++it;
    }
    while(it != ends && isLessThan(it.current->key,upper)){
      its.push_back(Iterator(it.current,*this,false));
      ++it;
    }
    it = this->find(upper);
    if(it != this->end())its.push_back(Iterator(it.current,*this,false));
    return its;
  }

  //INORDER iterator begins at ++root, do not change
  template <typename K, typename O, typename C>
  typename BinarySearchTree<K,O,C>::Iterator
  BinarySearchTree<K,O,C>::begin() const
  {
    return Iterator(root, *this);
  }

  //no more increment after end() also
  //returned in case of unsuccessful search
  //or when no more applicance of ++ is possible
  //do not change
  template <typename K, typename O, typename C>
  typename BinarySearchTree<K,O,C>::Iterator
  BinarySearchTree<K,O,C>::end() const
  {
    return Iterator(NULL, *this);
  }

  //public function to return height, do not change
  template <typename K, typename O, typename C>
  int
  BinarySearchTree<K,O,C>::height( ) const
  {
    return height(root);
  }

  /* private utility function for computing height */
  //do not change
  template <typename K, typename O, typename C>
  int
  BinarySearchTree<K,O,C>::height(BinaryNode * t) const
  {
    return (t == NULL) ? -1 : t->height;
  }

  //public function to return number of nodes in the tree
  //do not change
  template <typename K, typename O, typename C>
  size_t
  BinarySearchTree<K,O,C>::size( ) const
  {
    return nodes;
  }

  //public true if empty false o.w.
  //do not change
  template <typename K, typename O, typename C>
  bool
  BinarySearchTree<K,O,C>::empty( ) const
  {
    return nodes == 0;
  }

  //public function to print keys inorder to some ostream
  //do not change
  template <typename K, typename O, typename C>
  void
  BinarySearchTree<K,O,C>::print(std::ostream & out) const
  {
    print(root, out);
    out << '\n';
  }

  /* private utility function to print, do not change */
  template <typename K, typename O, typename C>
  void
  BinarySearchTree<K,O,C>::print(BinaryNode * t, std::ostream & out) const
  {
    if (t != NULL && t->left != NULL) 
    {
      out << '[';
      print( t->left, out );
    }
    else if (t != NULL && t->left == NULL && t->right != NULL)
    {
      out << "[";
    }

    if (t != NULL)
    {
      if (t->left == NULL && t->right == NULL)
      {
        out << '(' << (t->key) << ')';
      }
      else if (t->left != NULL || t->right != NULL)
      {
        out << '{' << (t->key) << ",H" << t->height << '}';
      }
    }
    
    if (t != NULL && t->right != NULL)
    {
      print( t->right, out );
      out << ']';
    }
    else if (t != NULL && t->left != NULL && t->right == NULL)
    {
      out << "]";
    }
  }

  /* static function to compute maximum of two elements */
  //do not change
  template <typename K, typename O, typename C>
  template <typename T>
  const T &
  BinarySearchTree<K,O,C>::max(const T & el1, const T & el2)
  {
    return el1 > el2 ? el1 : el2;
  }

  /* ROTATIONS, do not change */
  template <typename K, typename O, typename C>
  void 
  BinarySearchTree<K,O,C>::rotateWithLeftChild(BinaryNode * & k2)
  {
    BinaryNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = BinarySearchTree<K,O,C>::max(height(k2->left), 
                                              height(k2->right)) + 1;
    k1->height = BinarySearchTree<K,O,C>::max(height(k1->left), 
                                              height(k2)) + 1;
    k2 = k1;
  }

  template <typename K, typename O, typename C>
  void 
  BinarySearchTree<K,O,C>::rotateWithRightChild(BinaryNode * & k1)
  {
    BinaryNode *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = BinarySearchTree<K,O,C>::max(height(k1->left), 
                                              height(k1->right)) + 1;
    k2->height = BinarySearchTree<K,O,C>::max(height(k2->right), 
                                              height(k1)) + 1;
    k1 = k2;
  }

  template <typename K, typename O, typename C>
  void 
  BinarySearchTree<K,O,C>::doubleWithLeftChild(BinaryNode * & k3)
  {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
  }

  template <typename K, typename O, typename C>
  void 
  BinarySearchTree<K,O,C>::doubleWithRightChild(BinaryNode * & k1)
  {
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
  }

  /* ITERATOR functions */

  // dummy constructor, do not change
  template <typename K, typename O, typename C>
  BinarySearchTree<K,O,C>::Iterator::Iterator( ) 
    : current(NULL), root(NULL), useStack(false)
  {
  }

  // dereferencing operator non-const version, IMPLEMENT
  template <typename K, typename O, typename C>
  O &
  BinarySearchTree<K,O,C>::Iterator::operator*( )
  {
    return current->data;
  }

  // dereferencing operator const version, IMPLEMENT
  template <typename K, typename O, typename C>
  const O & 
  BinarySearchTree<K,O,C>::Iterator::operator*( ) const
  { 
    return current->data;
  }

  // compare Iterators ignoring useStack var, do not change
  template <typename K, typename O, typename C>
  bool 
  BinarySearchTree<K,O,C>::Iterator::
  operator==(const Iterator & rhs) const
  { 
    return current == rhs.current && 
           root == rhs.root; 
  }

  // compare Iterators ignoring useStack var, do not change
  template <typename K, typename O, typename C>
  bool 
  BinarySearchTree<K,O,C>::Iterator::
  operator!=(const Iterator & rhs) const
  { 
    return !(*this == rhs); 
  }

  // increment Iterator to point to the inorder next
  // node of then-current node, in case that no further
  // advances are possible return an Iterator that is
  // equal to end( ) , IMPLEMENT
  template <typename K, typename O, typename C>
  typename BinarySearchTree<K,O,C>::Iterator &
  BinarySearchTree<K,O,C>::Iterator::operator++( )
  {
    if(!useStack)current = NULL;
    else if(current && current->right){
      current = current->right;
      while(current->left){
        BinaryNode *a = current;
        s.push(a);
        current = current->left;
     }
    }
    else{
      if(s.empty())
       current = NULL;
      else{
        current = s.top();
        s.pop();
      }
    }
    
    
    return *this;
  }

  /* real Iterator constructor will be invoked by 
   * BST member function only. if no inorder iterator
   * is required by the computation designer should 
   * explicitly set useStack variable to false, o.w.
   * it will be assumed to be true. IMPLEMENT
   */
  template <typename K, typename O, typename C>
  BinarySearchTree<K,O,C>::Iterator::
  Iterator(BinaryNode * p, const BinarySearchTree & rhs, bool stk)
  {
    useStack = stk;
    current = p;
    root = rhs.root;
    if(stk && current){
      while(current->left){
        BinaryNode *a = current;
        s.push(a);
        current = current->left;
      }
    }
  }


   #endif
/*        BinaryNode * current; //position
          const BinaryNode * root; //for error check not implemented
          std::stack<BinaryNode *> s; //will be used to conduct in order traversal if
          bool useStack; //this variable is set to true, ignored in == and !=*/
