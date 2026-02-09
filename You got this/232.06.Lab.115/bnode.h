/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Katy's Favourites
 ************************************************************************/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

/*****************************************************************
 * BNODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <class T>
class BNode
{
public:
   // 
   // Construct
   //
   BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr)
   {
   }
   BNode(const T &  t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr)
   {
   }
   BNode(T && t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr)
   {
   }

   //
   // Data
   //
   BNode <T> * pLeft;
   BNode <T> * pRight;
   BNode <T> * pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T> * p)
{
   if (p == nullptr)
      return 0;

   return size(p->pLeft) + 1 + size(p->pRight);
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pAdd) // If the new Node isn't null, we set pNode to the parent Node
   {
      pAdd->pParent = pNode;
   }
   pNode->pLeft = pAdd; // Regardless, we set the pLeft of the parent node to the value at pAdd
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight (BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pAdd) // If the new Node isn't null, we set pNode to the parent Node
   {
      pAdd->pParent = pNode;
   }
   pNode->pRight = pAdd; // Regardless, we set the pRight of the parent node to the value at pAdd
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft (BNode <T> * pNode, const T & t) 
{
   BNode<T>* pAdd = new BNode<T>(t); // Create a pointer to the new Node
   pAdd->pParent = pNode; // Set the new Node parent to node in tree
   pNode->pLeft = pAdd; // Set the new Node to the left pointer of the Parent Node
}

template <class T>
inline void addLeft(BNode <T>* pNode, T && t)
{
   BNode<T>* pAdd = new BNode<T>(std::move(t)); // Create a pointer to the new Node
   pAdd->pParent = pNode; // Set the new Node parent to node in tree
   pNode->pLeft = pAdd; // Set the new Node to the left pointer of the Parent Node
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight (BNode <T> * pNode, const T & t)
{
   BNode<T>* pAdd = new BNode<T>(t); // Create a pointer to the new Node
   pAdd->pParent = pNode; // Set the new Node parent to node in tree
   pNode->pRight = pAdd; // Set the new Node to the right pointer of the Parent Node
}

template <class T>
void addRight(BNode <T>* pNode, T && t)
{
   BNode<T>* pAdd = new BNode<T>(std::move(t)); // Create a pointer to the new Node
   pAdd->pParent = pNode; // Set the new Node parent to node in tree
   pNode->pRight = pAdd; // Set the new Node to the right pointer of the Parent Node
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void clear(BNode <T> * & pThis)
{
   if (!pThis)
      return;
   clear(pThis->pLeft);
   clear(pThis->pRight);
   delete(pThis);
   pThis = nullptr;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
   BNode <T> * pTemp = pRHS; // create temporary data from RHS
   pRHS = pLHS; // set RHS to current LHS
   pLHS = pTemp; // set LHS to temp data
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T> * copy(const BNode <T> * pSrc) 
{
   if (!pSrc) return nullptr; // we don't need to do anything if it's null
      
   BNode<T> * pDest = new BNode<T> (pSrc->data); // create the destination
   
   // add the left side
   pDest->pLeft = copy(pSrc->pLeft);
   if (pDest->pLeft != nullptr)
      pDest->pLeft->pParent = pDest;
   
   // add the right side
   pDest->pRight = copy(pSrc->pRight);
   if (pDest->pRight != nullptr)
      pDest->pRight->pParent = pDest;
   
   return pDest;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T> * & pDest, const BNode <T>* pSrc)
{
   // if source is empty
   if (!pSrc)
   {
      clear(pDest); // if it doesn't, clear pDest
      return;
   }
   
   // if dest is empty and source exists
   if (!pDest && pSrc)
   {
      pDest = new BNode<T> (pSrc->data);
   }
   else
   {
      // if both exist
      pDest->data = pSrc->data;
   }
   
   // assign the left nodes
   assign(pDest->pLeft,  pSrc->pLeft);
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;

   // assign the right nodes
   assign(pDest->pRight, pSrc->pRight);
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;
}
