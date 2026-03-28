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
      clear();

      if (data)
      {
         delete[] data;
         data = nullptr;
      }
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
      return iterator(0, this); 
   }
   iterator end()   
   { 
      return iterator((int)numElements, this); 
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
      assert (nullptr != data[ibFromID((int)numElements - 1)]);
      return data[ibFromID((int)numElements - 1)][icFromID((int)numElements - 1)];
   }
   const T & back() const
   {
      assert(numElements != 0);
      assert (nullptr != data[ibFromID((int)numElements - 1)]);
      return data[ibFromID((int)numElements - 1)][icFromID((int)numElements - 1)];
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
   size_t size()  const { return numElements; }
   bool   empty() const { return numElements == 0; }
   
private:
   // array index from deque index
   int iaFromID(int id) const
   {
      assert(0 <= id);
      assert(0 <= iaFront && iaFront < (numCells * numBlocks));
      int ia = (id + iaFront) % (numCells * numBlocks);
      assert((0 <= ia) && ia < numCells * numBlocks);
      return ia;
   }

   // block index from deque index
   int ibFromID(int id) const
   {
      int ib = iaFromID(id) / numCells;
      assert(0 <= ib && ib < numBlocks);
      return ib;
   }

   // cell index from deque index
   int icFromID(int id) const
   {
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
   iterator() :id(0), d(nullptr)
   {
   }
   iterator(int id, deque* d) : id(id), d(d)
   {
   }
   iterator(const iterator& rhs) : id(rhs.id), d(rhs.d)
   {
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      if (this != &rhs)
      {
         id = rhs.id;
         d = rhs.d;
      }
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const iterator& rhs) const { return !(*this == rhs); }
   bool operator == (const iterator& rhs) const { return id == rhs.id && d == rhs.d; }

   // 
   // Access
   //
   T& operator * ()
   {
      return (*d)[id];
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
   if (rhs.numElements == 0)
      return;

   // allocate enough blocks for all elements (contiguous)
   numBlocks = (rhs.numElements - 1) / numCells + 1;
   data = new T * [numBlocks];

   for (size_t i = 0; i < numBlocks; i++)
      data[i] = nullptr;

   // copy elements
   for (size_t i = 0; i < rhs.numElements; i++)
   {
      int ib = i / numCells;
      int ic = i % numCells;

      if (data[ib] == nullptr)
         data[ib] = alloc.allocate(numCells);

      alloc.construct(&data[ib][ic], rhs[i]); // COPY happens here
   }

   numElements = rhs.numElements;
}

/*****************************************
 * DEQUE :: COPY-ASSIGN
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> & deque <T, A> :: operator = (deque & rhs)
{
   if (this == &rhs)
      return *this;

   size_t i = 0;

   // assign into existing elements
   for (; i < numElements && i < rhs.numElements; i++)
   {
      (*this)[i] = rhs[i];
   }

   // if LHS has extra -> remove them
   while (numElements > rhs.numElements)
   {
      pop_back();
   }

   // if RHS has extra -> add them
   while (i < rhs.numElements)
   {
      push_back(rhs[i]);
      i++;
   }

   return *this;
}

/*****************************************
 * DEQUE :: PUSH_BACK
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(const T& t)
{
//    reallocate as needed
   if (numElements + iaFront >= numBlocks * numCells)
      reallocate(((int)numBlocks == 0) ? 1 : (int)numBlocks * 2);
      
   int ib = ibFromID((int)numElements); // get ib
   int ic = icFromID((int)numElements); // get ic
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);
   
   alloc.construct(&data[ib][ic], t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_BACK - move
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(T && t)
{
   // Check if there's a need to reallocate
   if (numElements + iaFront >= numBlocks * numCells)
      reallocate((numBlocks == 0) ? 1 : numBlocks * 2);

   // Set the IB and IC
   int ib = ibFromID((int)numElements);
   int ic = icFromID((int)numElements);

   // Checks and responds to not enough blocks
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);

   // Creates the new element, and adds to the total
   alloc.construct(&data[ib][ic], std::move(t));
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(const T& t)
{
   // reallocate as needed
   if (numElements == numBlocks * numCells ||
         (numElements > 0 &&
          ((iaFront == 0 ? numBlocks * numCells - 1 : iaFront - 1) / numCells) == ibFromID((int)numElements - 1) &&
          ((iaFront == 0 ? numBlocks * numCells - 1 : iaFront - 1) % numCells) > icFromID((int)numElements - 1)))
         reallocate(((int)numBlocks == 0) ? 1 : (int)numBlocks * 2);
   
   // adjust the front array index and wrap if needed
   if (iaFront != 0)
      iaFront--;
   else
      iaFront = (int)numBlocks * (int)numCells - 1;
   
   // allocate as needed
   int ib = ibFromID(0); // get ib
   int ic = icFromID(0); // get ic
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);
   
   // Creates the new element, and adds to the total
   alloc.construct(&data[ib][ic], t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT - move
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(T&& t)
{
   // reallocate as needed
   if (numElements == numBlocks * numCells ||
      (numElements > 0 &&
         ((iaFront == 0 ? numBlocks * numCells - 1 : iaFront - 1) / numCells) == ibFromID((int)numElements - 1) &&
         ((iaFront == 0 ? numBlocks * numCells - 1 : iaFront - 1) % numCells) > icFromID((int)numElements - 1)))
      reallocate(((int)numBlocks == 0) ? 1 : (int)numBlocks * 2);

   // adjust the front array index and wrap if needed
   if (iaFront != 0)
      iaFront--;
   else
      iaFront = (int)numBlocks * (int)numCells - 1;

   // allocate as needed
   int ib = ibFromID(0); // get ib
   int ic = icFromID(0); // get ic
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);

   // Creates the new element, and adds to the total
   alloc.construct(&data[ib][ic], std::move(t));
   numElements++;
}

/*****************************************
 * DEQUE :: CLEAR
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::clear()
{
   // destroy all elements
   for (size_t i = 0; i < numElements; i++)
   {
    // step by step throughout the Deque
      int ib = ibFromID((int)i);
      int ic = icFromID((int)i);

      alloc.destroy(&data[ib][ic]);  // calls destructor
   }

   // deallocate all blocks
   for (size_t ib = 0; ib < numBlocks; ib++)
   {
      if (data && data[ib])
      {
         alloc.deallocate(data[ib], numCells);
         data[ib] = nullptr;
      }
   }

   // sets apropriate member variables
   numElements = 0;
   iaFront = 0;
}

/*****************************************
 * DEQUE :: POP FRONT
 * Remove the front element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::pop_front()
{
   assert(numElements > 0);

   int ib = ibFromID(0);
   int ic = icFromID(0);

   // destroy front element
   alloc.destroy(&data[ib][ic]);

   numElements--;

   // CASE: last element removed
   if (numElements == 0)
   {
      alloc.deallocate(data[ib], numCells);
      data[ib] = nullptr;
      iaFront = 0;
      return;
   }

   // move front forward (wrap) 
   iaFront = (iaFront + 1) % (numBlocks * numCells);

   // now check if old block is still used
   bool blockStillUsed = false;
   for (size_t i = 0; i < numElements; i++)
   {
      if (ibFromID((int)i) == ib)
      {
         blockStillUsed = true;
         break;
      }
   }

   if (!blockStillUsed)
   {
      alloc.deallocate(data[ib], numCells);
      data[ib] = nullptr;
   }
}

/*****************************************
 * DEQUE :: POP BACK
 * Remove the back element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::pop_back()
{
   // Can't be an empty Deque
   assert(numElements > 0);

   // Go to the last element in the Deque
   int id = (int)numElements - 1;
   int ib = ibFromID(id);
   int ic = icFromID(id);

   // destroy back element
   alloc.destroy(&data[ib][ic]);

   numElements--;

   // CASE: last element removed
   if (numElements == 0)
   {
      alloc.deallocate(data[ib], numCells);
      data[ib] = nullptr;
      iaFront = 0;
      return;
   }

   // check if block is now empty
   bool blockStillUsed = false;
   for (size_t i = 0; i < numElements; i++)
   {
      if (ibFromID((int)i) == ib)
      {
         blockStillUsed = true;
         break;
      }
   }

   if (!blockStillUsed)
   {
      alloc.deallocate(data[ib], numCells);
      data[ib] = nullptr;
   }
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
       ibFromID(0) == ibFromID((int)numElements - 1) &&
       icFromID(0) > icFromID((int)numElements - 1))
   {
//      int ibFrontOld = ibFromID(0);
      int ibBackOld = ibFromID((int)numElements - 1);
      int ibBackNew = (int)numElements / (int)numCells;
      dataNew[ibBackNew] = new T[numCells];
      for (int ic = 0; ic <= icFromID((int)numElements - 1); ic++)
      {
         alloc.construct(&dataNew[ibBackNew][ic], std::move(data[ibBackOld][ic])); // in with the new
         alloc.destroy(&data[ibBackOld][ic]); // out with the old
      }
   }
      
   // change the deque's member variables with new values
   if (data) delete[] data;
   data = dataNew;
   numBlocks = numBlocksNew;
   iaFront = iaFront % numCells;
}


} // namespace custom
