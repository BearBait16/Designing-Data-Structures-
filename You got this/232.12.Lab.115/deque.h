/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque 
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    Katy's Favourites
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <memory>   // for std::allocator
#include <iostream>

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *****************************************************/
template <typename T, typename A = std::allocator<T>>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque(const A & a = A()) : numBlocks(0), numCells(16), numElements(0), iaFront(0), data(nullptr)
   {
   }
   deque(deque & rhs);
   ~deque()
   {
   }

   //
   // Assign
   //
   deque & operator = (deque& rhs);

   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(); 
   }
   iterator end()   
   { 
      return iterator(); 
   }

   // 
   // Access
   //
   T & front()       
   {
      assert(numElements != 0);
      assert (nullptr != data[ibFromID(0)]);
      return data[ibFromID(0)][icFromID(0)];
   }
   const T & front() const 
   {
      assert(numElements != 0);
      assert (nullptr != data[ibFromID(0)]);
      return data[ibFromID(0)][icFromID(0)];
   }
   T & back()
   {
      assert(numElements != 0);
      assert (nullptr != data[ibFromID(numElements - 1)]);
      return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
   }
   const T & back() const
   {
      assert(numElements != 0);
      assert (nullptr != data[ibFromID(numElements - 1)]);
      return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
   }
   T & operator[](int id)
   {
      assert(0 <= id && id < numElements);
      assert(nullptr != data[ibFromID(id)]);
      return data[ibFromID(id)][icFromID(id)];
   }
   const T & operator[](int id) const
   {
      assert(0 <= id && id < numElements);
      assert(nullptr != data[ibFromID(id)]);
      return data[ibFromID(id)][icFromID(id)];
   }

   //
   // Insert
   //
   void push_back(const T & t);
   void push_back(T && t);
   void push_front(const T& t);
   void push_front(T&& t);

   //
   // Remove
   //
   void pop_front();
   void pop_back();
   void clear();

   //
   // Status
   //
   size_t size()  const { return 99; }
   bool   empty() const { return false; }
   
private:
   // array index from deque index
   int iaFromID(int id) const
   {
      std::cout << "checking id: " << id << std::endl;
      std::cout << "numElements: " << numElements << std::endl;
      assert(0 <= id && id <= numElements);
      assert(0 <= iaFront && iaFront < (numCells * numBlocks));
      int ia = (id + iaFront) % (numCells * numBlocks);
      assert((0 <= ia) && ia < numCells * numBlocks);
      return ia;
   }

   // block index from deque index
   int ibFromID(int id) const
   {
      std::cout << "in ibFromID" << std::endl;
      int ib = iaFromID(id) / numCells;
      assert(0 <= ib && ib < numBlocks);
      return ib;
   }

   // cell index from deque index
   int icFromID(int id) const
   {
      std::cout << "in icFromID" << std::endl;
      int ic = iaFromID(id) % numCells;
      assert(0 <= ic && ic < numCells);
      return ic;
   }

   // reallocate
   void reallocate(int numBlocksNew);

   A    alloc;                // use alloacator for memory allocation
   size_t numCells;           // number of cells in a block
   size_t numBlocks;          // number of blocks in the data array
   size_t numElements;        // number of elements in the deque
   int iaFront;               // array-centered index of the front of the deque
   T ** data;                 // array of arrays
};

/**************************************************
 * DEQUE ITERATOR
 * An iterator through deque.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T, typename A>
class deque <T, A> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   // 
   // Construct
   //
   iterator() 
   {
   }
   iterator(int id, deque* d) 
   {
   }
   iterator(const iterator& rhs) 
   { 
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const iterator& rhs) const { return true; }
   bool operator == (const iterator& rhs) const { return true; }

   // 
   // Access
   //
   T& operator * ()
   {
      return *(new T);
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
      return this->id - it.id;
   }
   iterator& operator += (int offset)
   {
      id += offset;
      return *this;
   }
   iterator& operator ++ ()
   {
      ++id;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      this->id++;
      return temp;
   }
   iterator& operator -- ()
   {
      --id;
      return *this;
   }
   iterator operator -- (int postfix)
   {
      iterator temp = *this;
      this->id--;
      return temp;
   }

private:
   int id;
   deque* d;
};

/*****************************************
 * DEQUE :: COPY CONSTRUCTOR
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> ::deque(deque& rhs) :
data(nullptr),
    numBlocks(0),
    numCells(16),
    numElements(0),
    iaFront(0)
{
   *this = rhs;
}

/*****************************************
 * DEQUE :: COPY-ASSIGN
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> & deque <T, A> :: operator = (deque & rhs)
{
   this->clear();
//   for (auto it = rhs.begin(); it != rhs.end(); ++it)
//      this->push_back(*it);
   return *this;
}

/*****************************************
 * DEQUE :: PUSH_BACK
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(const T& t)
{
//   int numCapacity = numBlocks * numCells;
//   if (numElements == numCapacity)
//      reallocate(numCapacity * 2);
//   std::cout << "numElements before iaFromID call: " << numElements << std::endl;
//   data[iaFromID(numElements++)];
}

/*****************************************
 * DEQUE :: PUSH_BACK - move
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(T && t)
{
}

/*****************************************
 * DEQUE :: PUSH_FRONT
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(const T& t)
{
   int numCapacity = numBlocks * numCells;
      if (numElements == numCapacity)
         reallocate(numCapacity * 2);

      iaFront--;
      if (iaFront < 0)
         iaFront = numCapacity - 1;
      
      // 1. Calculate the 2D coordinates
      int block = iaFront / numCells;
      int cell = iaFront % numCells;

      // 2. Assign the value to the exact cell within the block
      data[block][cell] = t;
      
      numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT - move
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(T&& t)
{
}

/*****************************************
 * DEQUE :: CLEAR
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::clear()
{
}

/*****************************************
 * DEQUE :: POP FRONT
 * Remove the front element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: pop_front()
{
}

/*****************************************
 * DEQUE :: POP BACK
 * Remove the back element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::pop_back()
{
}

/*****************************************
 * DEQUE :: REALLOCATE
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: reallocate(int numBlocksNew)
{
   // allocate new array of pointers
   T** dataNew = new T*[numBlocksNew];
   
   // copy over pointers and unwrap as we go
   int ibNew = 0;
   for (int ibOld = 0; ibOld < numElements; ibOld += numCells)
   {
      dataNew[ibNew] = data[ibFromID(ibOld)];
      ibNew++;
   }
   
   // set block pointers to null if there are no blokcs
   while (ibNew < numBlocksNew)
   {
      dataNew[ibNew] = nullptr;
      ibNew++;
   }
   
   // if the back element is on the front element's block, move it
   if (numElements > 0 &&
       ibFromID(0) == ibFromID(numElements - 1) &&
       icFromID(0) > icFromID(numElements - 1))
   {
//      int ibFrontOld = ibFromID(0);
      int ibBackOld = ibFromID(numElements - 1);
      int ibBackNew = numElements / numCells;
      dataNew[ibBackNew] = new T[numCells];
      for (int ic = 0; ic <= icFromID(numElements - 1); ic++)
         dataNew[ibBackNew][ic] = std::move(data[ibBackOld][ic]);
   }
      
   // change the deque's member variables with new values
   if (data) delete[] data;
   data = dataNew;
   numBlocks = numBlocksNew;
   iaFront = iaFront % numCells;
}




} // namespace custom
