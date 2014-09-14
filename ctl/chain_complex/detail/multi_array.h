#ifndef CTL_MULTI_ARRAY_H
#define CTL_MULTI_ARRAY_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the license below, but, please
* do not commit acts of academic dishonesty. We strongly encourage and request 
* that for any [academic] use of this source code one should cite one the 
* following works:
* 
* \cite{hatcher, z-ct-10}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program in a file entitled COPYING; if not, write to the 
* Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************************
*******************************************************************************
* NOTES
*
*
*******************************************************************************/
//STL
#include <vector>
#include <numeric>
#include <functional>
#include <cassert> //assert
#include <iterator> //std::distance

//non-exported functionality
namespace ctl {
namespace detail{

/**
* @brief A generic multidimensional array with runtime modifiable extents_ and 
* sizes.
*
*
* Unlike boost::multi_array the number of dimensions is not fixed at runtime.
* 
* @tparam T
*/
template< typename T>
class multi_array {
  typedef std::vector< T> Vector;
  public:
  typedef std::vector< std::size_t> Coordinate;
  typedef typename Vector::value_type value_type;
  typedef typename Vector::size_type size_type;
  typedef typename Vector::iterator iterator;
  typedef typename Vector::const_iterator const_iterator;
  typedef typename Vector::reverse_iterator reverse_iterator;
  typedef typename Vector::const_reverse_iterator const_reverse_iterator;
  
  /**
  * @brief Default constructor
  */
  multi_array(){}	
  
  /**
  * @brief Extents constructor, 
  * Tells us the size of the array in every direction
  *
  * @tparam Extents_iterator
  * @param begin
  * @param end
  */
  template< typename Extents_iterator>
  multi_array( Extents_iterator begin, Extents_iterator end):
   data( std::accumulate(begin, end, 1, std::multiplies< double>())),
   d_( begin, end), extents_( begin, end), base_( d_.size(), 0){
   for( auto i = ++d_.begin(); i != d_.end(); ++i){ *i *= *(i-1); }
  }

  /**
  * @brief Complete constructor, specifies extents_, data, 
  * and optionally, base coordinate
  * @tparam Extents_iterator
  * @param begin
  * @param end
  */
  template< typename Extents_iterator, typename Data_iterator>
  multi_array( Extents_iterator begin, Extents_iterator end,
	       Data_iterator begin1, Data_iterator end1): 
   data( begin1, end1), d_( begin, end), 
   extents_( begin, end), base_( d_.size(), 0){
   for( auto i = ++d_.begin(); i != d_.end(); ++i){ *i *= *(i-1); }
  }


  /**
  * @brief Complete constructor, specifies extents_, data, 
  * and optionally, base coordinate
  * @tparam Extents_iterator
  * @param begin
  * @param end
  */
  template< typename Extents_iterator, typename Data_iterator>
  multi_array( Extents_iterator begin, Extents_iterator end,
	       Data_iterator begin1, Data_iterator end1, 
	       const Coordinate & base__): 
   data( begin1, end1), d_( begin, end), extents_( begin, end), base_( base__){ 
   assert( base_.size() == d_.size());
   for( auto i = ++d_.begin(); i != d_.end(); ++i){ *i *= *(i-1); }
  }
  /**
  * @brief Complete constructor, specifies extents_, data, 
  * and optionally, base coordinate
  * @tparam Extents_iterator
  * @param begin
  * @param end
  */
  template< typename Extents_iterator>
  multi_array( Extents_iterator begin, Extents_iterator end, 
	       const Coordinate & base__): 
   d_( begin, end), extents_( begin, end), base_( base__){ 
   assert( base_.size() == d_.size());
   for( auto i = ++d_.begin(); i != d_.end(); ++i){ *i *= *(i-1); }
   data.resize( *(d_.rbegin()));
  }



   
  /**
  * @brief Reindexes the array to be "rooted" at position given by c 
  * Complexity: Runs in O(d) time where d is the dimension of the array.
  * @tparam Coordinate
  * @param c
  */
  void reindex( const Coordinate & c){ 
	assert( c.size() == d_.size()); 
	base_ = c; 
  }
  
  /**
  * @brief returns the element at logical index i
  * @param index
  * @return 
  */
  const T& operator[]( std::size_t index) const { return data[ index]; }
  /**
  * @brief returns the element at logical index i
  * @param index
  * @return 
  */
        T& operator[]( std::size_t index)       { return data[ index]; }

  /**
  * @brief retuns the element at the specified coordinates
  *
  * @tparam Coordinate
  * @param c
  *
  * @return 
  */
  const T& operator()( const Coordinate & c) const{ 
	return data[ coordinate_to_index( c)]; 
  }

  /**
  * @brief retuns the element at the specified coordinates
  *
  * @tparam Coordinate
  * @param c
  *
  * @return 
  */
  T& operator()( const Coordinate & c) { 
	return data[ coordinate_to_index( c)]; 
  }

  /**
  * @brief returns an iterator to the first element in the array
  * @return 
  */
  iterator begin() 	       { return data.begin(); }
  const_iterator begin() const { return data.begin(); }

  /**
  * @brief returns an iterator to the past the end element of the array
  * @return 
  */
  iterator end() 	       { return data.end(); }
  const_iterator end() 	 const { return data.end(); }

  /**
  * @brief returns an reverse_iterator to the last element in the array
  * @return 
  */
 
  iterator rbegin() 	        { return data.rbegin(); }
  const_iterator rbegin() const { return data.rbegin(); }

  /**
  * @brief returns a reverse_iterator to the before-the-first element of the array
  * @return 
  */
 
  iterator rend() 	       { return data.rend(); }
  const_iterator rend()  const { return data.rend(); }
  
  /**
  * @brief Reserves internal storage to hold at least n elements sizeof(Data)
  * @param n
  */
  void reserve( std::size_t n){ data.reserve( n); }

  /**
  * @brief Grows the container to size at least given
  * by the product of the extents_
  * @tparam Extents_iterator
  * @param begin
  * @param end
  */
  template< typename Extents_iterator>
  void resize(  Extents_iterator begin, Extents_iterator end){
	d_.assign( begin, end);
	extents_.assign( begin, end);
        for( auto i = ++d_.begin(); i != d_.end(); ++i){ *i *= *(i-1); }
	data.resize( *(d_.rbegin()));	
  }
  
  /**
  * @brief Returns the number of elements in the entire multiarray
  * @return 
  */
  std::size_t size() const { return data.size(); }
  
  /**
  * @brief Returns the largest number of elements storable before a reallocation is necessary
  * @return 
  */
  std::size_t capacity() const { return data.capacity(); }

  /**
  * @brief returns the length of the array in dimension i 
  * @return 
  */
  std::size_t extents( std::size_t i) const { return extents_[ i]; }

  /**
  * @brief Returns the length of the array in each dimension
  */
  Coordinate extents() const { return extents_; }
  Coordinate base() const    { return base_; } 
  std::size_t base( std::size_t i) const    { return base_[ i]; }
  std::size_t dimension() const { return extents_.size(); } 
  /**
  * @brief Coverts linear index into coordinate vector c
  *
  * @tparam Coordinate
  * @param index
  * @param c
  *
  * @return 
  */
  Coordinate& index_to_coordinate( std::size_t index, Coordinate & c) const {
    c.resize( d_.size(), 0);
    for( auto i = d_.size()-1; i>0;  --i){ 
        c[ i] = index/ d_[ i-1];
        index -= c[ i]*(d_[ i-1]);
	c[ i] += base_[ i]; //write down the coordinate in the correct offset
    }
    c[ 0] = index + base_[ 0];
    return c;
  }
  Coordinate& iterator_to_coordinate( iterator i, Coordinate & c) const {
	return index_to_coordinate( std::distance( data.begin(), i), c);
  }
  Coordinate& iterator_to_coordinate( const_iterator i, Coordinate & c) const {
	return index_to_coordinate( std::distance( data.cbegin(), i), c);
  }
  Coordinate& iterator_to_coordinate( reverse_iterator i, Coordinate & c) const {
	return index_to_coordinate( std::distance( data.rbegin(), i), c);
  }
  Coordinate& iterator_to_coordinate( const_reverse_iterator i, Coordinate & c) const {
	return index_to_coordinate( std::distance( data.crbegin(), i), c);
  }
  /**
  * @brief Converts a coordinate vector into a linear index 
  *
  * @tparam Coordinate
  * @param c
  *
  * @return 
  */
  std::size_t coordinate_to_index( Coordinate & c) const{
     assert( c.size() == d_.size());
     std::size_t index = (c[0]-base_[ 0]);
     for( auto i = 1; i != d_.size(); ++i){ index += (c[ i]-base_[ i])*d_[ i-1];}
     return index;
  }
  bool operator==( const multi_array & o) const {
	return (data == o.data) && (d_ == o.d_) && (extents_ == o.extents_) &&
		(base_ == o.base_);
  }
  bool operator!= ( const multi_array & o) const {
	return !((*this)==o);
  }
  multi_array& operator=( const multi_array&& o) {
	data = std::move( o.data);
	d_ = std::move( o.d_);
	extents_ = std::move( o.extents_);
	base_ = std::move( o.base_);
	return *this;	
  }
 
  multi_array& operator=( const multi_array& o) {
	data = o.data;
	d_ =  o.d_;
	extents_ = o.extents_;
	base_ = o.base_;
	return *this;	
  } 

  private:
  Vector data; //the actual data
  //TODO: Optimize this with libdivide.
  Coordinate d_; //entry i stores the product if extents_[ 0]*...*extents_[ i]
  Coordinate extents_; //length of the array
  Coordinate base_; //root the indices at places other than (0,0,0);
}; //end class multi_array

} // end namespace detail
} //ctl namespace

#endif //CTL_MULTI_ARRAY_H
