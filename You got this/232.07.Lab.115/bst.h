/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>          // because I am paranoid
#include <memory>           // for std::allocator
#include <functional>       // for std::less
#include <utility>          // for std::pair
#include <initializer_list> // for the initializer list, of course!

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return numElements == 0; }
   size_t size()  const noexcept { return numElements;   }
   
private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode()
   {
      pLeft = pRight = pParent = nullptr;
      isRed = false;
   }
   BNode(const T &  t) : data(t)
   {
      pLeft = pRight = pParent = nullptr;
      isRed = false;
   }
   BNode(T && t) : data(std::move(t))
   {
      pLeft = pRight = pParent = nullptr;
      isRed = true;
   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return pNode && pNode->pParent && pNode->pParent->pRight == pNode; }
   bool isLeftChild( BNode * pNode) const { return pNode && pNode->pParent && pNode->pParent->pLeft == pNode; }

   // balance the tree
   void balance();

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode * p = nullptr)          
   {
      this->pNode = p;
   }
   iterator(const iterator & rhs)         
   {
      this->pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
      this->pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return this->pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
      return this->pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      return *this;;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) 
{
   root = nullptr;
   numElements = 0;
   
   *this = rhs;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) 
{
   root = rhs.root;
   numElements = rhs.numElements;
   
   rhs.root = nullptr;
   rhs.numElements = 0;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il)
{
   root = nullptr;
   numElements = 0;
   
   *this = il;
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   clear();
//   root = copy(rhs);
   numElements = rhs.numElements;
   
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   clear();
   for (auto t : il)
      insert(t);
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   clear();
   swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
   BNode * tempRoot = rhs.root; // temporarily store root
   rhs.root = root; // change rhs.root
   root = tempRoot; // change root
   
   size_t tempElements = rhs.numElements; // temporarily store numEls
   rhs.numElements = numElements; // change root.numEls
   numElements = tempElements; // change numEls
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(const T& t, bool keepUnique)
{
   if (root == nullptr)
   {
      BNode* pNew = new BNode(t);
      pNew->isRed = false;
      numElements++;
      root = pNew;
      return { pNew, true };
   }

   BNode* pNode = root;
   BNode* pNew = pNode; // Sets up tracking for new node
   bool done = false;

   while (done == false)
   {
      if (pNode->data < t)
      {
         if (pNode->pRight == nullptr)
         {
            pNode->addRight(t);
            pNew = pNode->pRight;
            done = true;
         }
         else
         {
            pNode = pNode->pRight;
         }
      }
      else
      {
         if (pNode->pLeft == nullptr)
         {
            pNode->addLeft(t);
            pNew = pNode->pLeft;
            done = true;
         }
         else
         {
            pNode = pNode->pLeft;
         }
      }
   }
   numElements = numElements + 1;
   std::pair<iterator, bool> pairReturn(pNew, true);
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T&& t, bool keepUnique)
{
   if (root == nullptr)
   {
      BNode* pNew = new BNode(t);
      root = pNew;
      pNew->isRed = false;
      numElements++;
      return { pNew, true };
   }
   BNode* pNode = root;
   BNode* pNew = pNode; // Sets up tracking for new node
   bool done = false;
   while (done == false)
   {
      if (pNode->data < t)
      {
         if (pNode->pRight == nullptr)
         {
            pNode->addRight(t);
            pNew = pNode->pRight;
            done = true;
         }
         else
         {
            pNode = pNode->pRight;
         }
      }
      else
      {
         if (pNode->pLeft == nullptr)
         {
            pNode->addLeft(t);
            pNew = pNode->pLeft;
            done = true;
         }
         else
         {
            pNode = pNode->pLeft;
         }
      }
   }
   numElements++;
   std::pair<iterator, bool> pairReturn(pNew, true);
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator& it)
{
   // If iterator is invalid, nothing to remove
   if (it == end())
      return end();

   BNode* p = it.pNode;

   // Save next iterator BEFORE we destroy anything
   iterator returnValue = it;
   ++returnValue;

   // No children case
   if (!p->pRight && !p->pLeft)
   {
      //Deleting root and no parent
      if (p->pParent == nullptr)
         root = nullptr;
      //Detach from parent (right)
      else if (p->pParent->pRight == p)
         p->pParent->pRight = nullptr;
      //Detach from parent (left)
      else if (p->pParent->pLeft == p)
         p->pParent->pLeft = nullptr;

      //Destroy node and update size
      delete p;
      --numElements;
   }

   // One child (left only)
   else if (!p->pRight && p->pLeft)
   {
      //If deleting root, move left child to root
      if (p->pParent == nullptr)
      {
         root = p->pLeft;
         root->pParent = nullptr;
      }
      //If right child, move left child up and update parent
      else if (p->pParent->pRight == p)
      {
         p->pParent->pRight = p->pLeft;
         p->pLeft->pParent = p->pParent;
      }
      //If left child, move left child up and update parent
      else if (p->pParent->pLeft == p)
      {
         p->pParent->pLeft = p->pLeft;
         p->pLeft->pParent = p->pParent;
      }

      //Destroy node and update size
      delete p;
      --numElements;
   }

   // One child (right only)
   else if (!p->pLeft && p->pRight)
   {
      //If deleting root, move right child to root
      if (p->pParent == nullptr)
      {
         root = p->pRight;
         root->pParent = nullptr;
      }
      //If right child, move right child up and update parent
      else if (p->pParent->pRight == p)
      {
         p->pParent->pRight = p->pRight;
         p->pRight->pParent = p->pParent;
      }
      //If left child, move right child up and update parent
      else if (p->pParent->pLeft == p)
      {
         p->pParent->pLeft = p->pRight;
         p->pRight->pParent = p->pParent;
      }

      //Destroy node and update size
      delete p;
      --numElements;
   }

   // Two children case
   else
   {
      //Store original node
      BNode* original = p;
      p = p->pRight;
      // Find successor (smallest node in right subtree)
      while (p->pLeft)
         p = p->pLeft;

      // Detach successor from its current location
      if (p->pParent->pLeft == p)
         p->pParent->pLeft = p->pRight;
      else
         p->pParent->pRight = p->pRight;

      if (p->pRight)
         p->pRight->pParent = p->pParent;

      // Replace original node with successor
      p->pParent = original->pParent;

      if (original->pParent == nullptr)
         root = p;
      else if (original->pParent->pLeft == original)
         original->pParent->pLeft = p;
      else
         original->pParent->pRight = p;

      // Update successor's children to original's children
      p->pLeft = original->pLeft;
      p->pRight = original->pRight;

      if (p->pLeft)
         p->pLeft->pParent = p;
      if (p->pRight)
         p->pRight->pParent = p;

      // Destroy original node and update size
      delete original;
      --numElements;
   }

   return returnValue;
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   while (!empty())
   {
      iterator it = begin(); // make an iterator for the current beginning node
      erase(it); // erase it
   }
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   // if it is empty, return end
   if (empty())
      return end();
   
   // set p to the root of the tree
   BNode * p = root;
   while (p->pLeft)
   {
      // while there is a left node, return the next left one
      p = p->pLeft;
   }
   
   // return an iterator to p
   return iterator(p);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   BNode* p = root;
   while (p)
   {
      if (p->data == t)
      {
         return iterator(p);
      }
      else if (t < p->data)
      {
         p = p->pLeft;
      }
      else
      {
         p = p->pRight;
      }
   }
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   pLeft = pNode;
   pNode->pParent = this;
   pNode->balance();
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   pRight = pNode;
   pNode->pParent = this;
   pNode->balance();
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   BNode* pNode = new BNode(t);
   pNode->isRed = true;
   pLeft = pNode;
   pNode->pParent = this;
   pNode->balance();
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   BNode* pNode = new BNode(t);
   pNode->isRed = true;
   pLeft = pNode;
   pNode->pParent = this;
   pNode->balance();
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   BNode* pNode = new BNode(t);
   pNode->isRed = true;
   pRight = pNode;
   pNode->pParent = this;
   pNode->balance();
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   BNode* pNode = new BNode(t);
   pNode->isRed = true;
   pRight = pNode;
   pNode->pParent = this;
   pNode->balance();
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> ::BNode::balance()
{
   // Case 1: if we are the root, then color ourselves black and call it a day.
   if (pParent == nullptr)
   {
      isRed = false;
   }

   // Case 2: if the parent is black, then there is nothing left to do
   else if (pParent->isRed == false)
   {

   }
   // Case 3: if the aunt is red, then just recolor

   //else if (pParent->pParent->pLeft->isRed == true)
   //{
   //   pParent->isRed = false;
   //   pParent->pParent->pLeft->isRed = false;
   //   pParent->pParent->isRed = true;
   //   pParent->pParent->balance();
   //}

   //else if (pParent->pParent->pRight->isRed == true)
   //{
   //   pParent->isRed = false;
   //   pParent->pParent->pRight->isRed = false;
   //   pParent->pParent->isRed = true;
   //   pParent->pParent->balance();
   //}

   // Case 4: if the aunt is black or non-existant, then we need to rotate
   //else if (pParent->pLeft == nullptr || pParent->pRight == nullptr)
   //{

   //}
   // Case 4a: We are mom's left and mom is granny's left
   // case 4b: We are mom's right and mom is granny's right
   // Case 4c: We are mom's right and mom is granny's left
   // case 4d: we are mom's left and mom is granny's right
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   // in case pNode is null
   if (pNode == nullptr)
      return *this;
   
   // case one: we have a right child
   else if (pNode->pRight)
   {
      pNode = pNode->pRight; // set the current to the child
      while (pNode->pLeft) {
         // while there is a left child, go down
         // and reassign current to that child
         pNode = pNode->pLeft;
      }
   }
   
   // case two: we have no right child and we are our parent's left child
   else if (pNode->pRight == nullptr &&
            pNode->pParent &&
            pNode->pParent->pLeft == pNode)
   {
      // return the parent
      pNode = pNode->pParent;
   }
   
   // case three: we have no right child and we are our parent's right child
   else if (pNode->pRight == nullptr &&
            pNode->pParent &&
            pNode->pParent->pRight == pNode)
   {
      while (pNode->pParent && pNode->pParent->pRight == pNode)
      {
         // while we have a parent and we are a right child,
         // set the node to the parent
         pNode = pNode->pParent;
      }
      
      // return one more
      pNode = pNode->pParent;
   }
   
   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   // in case pNode is null
   if (pNode == nullptr)
      return *this;
   
   // case one: we have a left child
   else if (pNode->pLeft)
   {
      pNode = pNode->pLeft; // set the current to the child
      while (pNode->pRight) {
         // while there is a right child, go down
         // and reassign current to that child
         pNode = pNode->pRight;
      }
   }
   
   // case two: we have no left child and we are our parent's right child
   else if (pNode->pLeft == nullptr &&
            pNode->pParent->pRight == pNode)
   {
      // return the parent
      pNode = pNode->pParent;
   }
   
   // case three: we have no left child and we are our parent's left child
   else if (pNode->pLeft == nullptr &&
            pNode->pParent->pLeft== pNode)
   {
      while (pNode->pParent && pNode->pParent->pLeft == pNode)
      {
         // while we have a parent and we are a left child,
         // set the node to the parent
         pNode = pNode->pParent;
      }
      
      // return one more
      pNode = pNode->pParent;
   }
   
   return *this;

}


} // namespace custom


