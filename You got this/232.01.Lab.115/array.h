/***********************************************************************
 * Header:
 *    Array
 * Summary:
 *    Our custom implementation of std::array
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *    
 *    This will contain the class definition of:
 *       array             : similar to std::array
 *       array :: iterator : an iterator through the array
 * Author
 *    Jackson Aden
 ************************************************************************/

#pragma once

#include <stdexcept>

class TestArray; // forward declaration for unit tests

namespace custom
{

/************************************************
 * ARRAY
 * A class that holds stuff
 ***********************************************/
template <typename T, int N>
struct array
{
public:
   //
   // Construct
   //

   // No constructors or destructors needed here

   //
   // Assign
   //

   // No assignment operators needed here

   //
   // Iterator
   //

   // Forward declaration for the iterator class
   class iterator;

   // Begin and end methods
   iterator begin() 
   { 
      /* your code goes here; it should return an iterator referring to the first element in the array */
      return iterator(&__elems_[0]);
   }
   iterator end()   
   { 
      /* your code goes here; it should return an iterator referring to the first element off the end of the array */
      return iterator(&__elems_[N]);
   }

   // 
   // Access
   //

   // Subscript
   T& operator [] (size_t index)       
   { 
      /*your code goes here; it should return the i-th element in the array */
      return *(&__elems_[index]);
   }
   const T& operator [] (size_t index) const 
   { 
      /* your code goes here; it should return the i-th element in the array */
      return *(&__elems_[index]); 
   }

   // At
   T& at(size_t index)  
   { 
      /* your code goes here; it should return the i-th element in the array */
      if (index > N || index < 0)
      {
         throw std::out_of_range("index ain't in range");
      }
      return __elems_[index];
   }
   const T& at(size_t index) const 
   { 
      /* your code goes here; it should return the i-th element in the array */
      if (index > N || index < 0)
      {
         throw std::out_of_range("index ain't in range");
      }
      return __elems_[index];
   }

   // Front and Back
   T& front()       
   { 
      /* your code goes here; it should return the first element in the array */
      return *(&__elems_[0]);
   }
   T& back()        
   { 
      /* your code goes here; it should return the last element in the array */
      return *(&__elems_[N - 1]);
   }
   const T& front() const
   { 
      /* your code goes here; it should return the first element in the array */
      return *(&__elems_[0]);
   }
   const T& back()  const 
   { 
      /* your code goes here; it should return the last element in the array */
      return *(&__elems_[N - 1]);
   }

   //
   // Remove

   // No clear or erase here

   //
   // Status
   //

   // number of elements in the array
   size_t size()  const 
   { 
      return N;
   }
   bool empty() const 
   { 
      if (N == 0)
      {
         return true;
      }
      return false;
   }
   
   //
   // Member Variable
   //

   // statically allocated array of T
   T __elems_[N];        /* your code goes here */
};

/**************************************************
 * ARRAY ITERATOR
 * An iterator through array
 *************************************************/
template <typename T, int N>
class array <T, N> :: iterator
{
   friend class ::TestArray; // give unit tests access to the privates
public:
   // constructors, destructors, and assignment operator
   iterator()      
   { 
      /* your code goes here; it should initialize an iterator */
      p = nullptr;
   }
   iterator(T * p)  
   {
      /* your code goes here; it should point to an element in the array */
      this->p = p;
   }
   iterator(const iterator & rhs) 
   {
      /* your code goes here; it should copy an iterator */
      p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
      /* your code goes here; it should copy an iterator */
      p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      if (rhs.p == p)
      {
         return false;
      }
      else
      {
         return true;
      }
   }
   bool operator == (const iterator & rhs) const 
   { 
      if (rhs.p != p)
      {
         return false;
      }
      else
      {
         return true;
      }
   }
   
   // dereference operator
   T & operator * ()       
   { 
      /* your code goes here; it should fetch the element referred to by p */

      return *p; 
   }
   const T & operator * () const 
   { 
      /* your code goes here; it should fetch the element referred to by p */
      return *p; 
   }
   
   // prefix increment
   iterator & operator ++ ()
   {
      /* your code goes here; it should advance the iterator by one */
      ++p;
      return *this;
   }
   
   // postfix increment
   iterator operator ++ (int postfix)
   {
      return p++;
   }
   
private:
   T * p;
};

}; // namespace custom
