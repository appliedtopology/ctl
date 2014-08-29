#ifndef CUBE_H
#define CUBE_H
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
* Notes: 
* August 27, 2014
* Instead of thinking of a cube as as a product of intervals -- the std. def.
* I use the approach of Hubert Wagner et. all in:
* Efficient Computation of Persistent Homology for Cubical Data
* In particular a cubical complex on $n$ vertices in $d$ dimensions is an array 
* of size m = (2n-1)^d
*******************************************************************************/
#include <vector> //std::vector
#include <utility> //std::pair
#include <initializer_list>  // std::initializer_list
#include <iostream> //cout (debug only)
#include <algorithm> //sort, unique

/*! \namespace ctl
Namespace where all library functionality resides
*/
namespace ctl {

namespace detail{ 
	template< typename T>
	using Interval = std::pair< T, T>;
} //end namespace detail

/**
* \class Cube< T>
* This class describes an cube for a cubical chain complex.
* While a cube is defined as a product of intervals, a cubical complex
* does not store a vector of these. A cube is infact stored implicitly
* as an address to its data, and we provide the mirage of the definition.
* This object does not itself have the facility for a boundary, this is handled
* by a separate boundary operator.
*/
template< typename T>
class Cube {
	public:
	typedef typename ctl::detail::Interval< T> Interval;
	private:
	typedef typename std::vector< Interval> Vector;
	typedef Cube< T> Self;
	typedef std::initializer_list< Interval> Init_list;
	public:
	//! \typedef  
	typedef std::size_t size_t;
	//! \typedef synonomous with vertex_type  
	typedef typename Vector::value_type value_type;
	//! \typedef The vertex type of the cube  
	typedef T vertex_type;
	//! \typedef const_iterator to the cube
	typedef typename Vector::const_iterator const_iterator;
	//! \typedef const_reverse_iterator to the cube
	typedef typename Vector::const_reverse_iterator const_reverse_iterator;
	//! \typedef an iterator to the cube
	typedef typename Vector::iterator iterator;
	//! \typedef reverse_iterator to the cube
	typedef typename Vector::reverse_iterator reverse_iterator;
	public:
	//! Default constructor
	Cube(): intervals(){};
	//! Reserves space for at least d intervals.
	Cube( size_t d) { intervals.reserve( d); }
	//! Reserves space for at least d intervals and initializes them to t
	Cube( size_t d, const T & t): intervals( d, t){}
	//! Initializer list constructor
	Cube( const Init_list & il) : intervals( il) {
		sort( intervals.begin(), intervals.end() );
		intervals.erase( unique( intervals.begin(), intervals.end() ), 
				intervals.end() );

	}
	//!  Range constructor 
	template< typename Iterator>
	Cube( const Iterator begin, const Iterator end): intervals( begin,end){}
	//! Copies the data from one cube to another
	Cube( const Self & from): intervals( from.intervals){}
	//! Moves the data from one cube to another
	Cube( Self && from): intervals( std::move( from.intervals)){}
	//! returns an iterator to the first vertex in the cube
	iterator       begin()	        { return intervals.begin(); }
	//! returns a const_iterator to the first vertex in the cube
	const_iterator begin()  const	{ return intervals.begin(); }
	
	/*!
	* @brief  returns an iterator to the past-the-end vertex in the cube
	* @return iterator
	*/
	iterator         end()	  	{ return intervals.end();   }
	/**
	* @brief returns a const_iterator to the past-the-end vertex in the cube
	* @return const_iterator
	*/
	const_iterator   end()  const	{ return intervals.end();   }
	/*!
	* @brief returns a reverse_iterator to the last vertex in the cube
	* @return reverse_iterator
	*/
	reverse_iterator        rbegin()	{ return intervals.rbegin(); }
	/*!
	* @brief returns a const_reverse_iterator to the last vertex in the cube
	* @return const_reverse_iterator
	*/
	const_reverse_iterator  rbegin()  const	{ return intervals.rbegin(); }
	/*!
	 * @brief Returns a reverse_iterator pointing to the theoretical element 
 	 * preceding the first element in the container 
	 * (which is considered its reverse end).
	 * @return reverse_iterator
	 */
	reverse_iterator         rend()	        { return intervals.rend(); }
	/*!
	 * @brief Returns a const_reverse_iterator 
	 * pointing to the theoretical element 
 	 * preceding the first element in the container 
	 * (which is considered its reverse end).
	 * @return const_reverse_iterator
	 */
	const_reverse_iterator   rend()  const	{ return intervals.rend(); }
	/*! Returns the size of the cube
	 * @return size_t
	 */
	size_t       size() const	{ return intervals.size(); 	}
	/*! Returns the dimension of the cube
	 * @return size_t
	 */
	size_t  dimension() const	{ return size()-1; 	  	}

	/*! Returns the capacity of the cube
	* @return size_t
	*/
	size_t   capacity() const  { return intervals.capacity();   }
	/*! Removes all intervals (which are destroyed), 
	 * leaving the cell with a size of 0.*/
	void clear() { intervals.clear(); }

	/*!
	* @brief Inserts the vertex v if it doesn't already exist
	* @param const vertex_type & v
	* @return iterator to the vertex v in the cube
	*/
	iterator insert( const vertex_type & v){
	      iterator pos = std::lower_bound( begin(), end(), v);
	      if(pos != end() && *pos == v) { return pos; }
	      return intervals.insert( pos, v);
	}

	/*!
	* @brief Inserts the intervals in an initializer list 
	* @param std::initalizer_list< T>
	* @return None.
	*/
	void insert( const Init_list & il){
	        std::size_t offset = intervals.size();
		intervals.insert( intervals.end(), il.begin(), il.end());
	        std::inplace_merge( intervals.begin(), 
				    intervals.begin()+offset, intervals.end());
		intervals.erase( unique( intervals.begin(), intervals.end() ), 
				intervals.end() );
	
	}

	/**
	* Inserts the range [begin, end) into the cube 
	* @param Iterator begin, Iterator end
	* @return None.
	*/
	template< typename Iterator>
	void insert( Iterator first, Iterator last){
	        std::size_t offset = intervals.size();
		intervals.insert( intervals.end(), first, last);
		std::inplace_merge( intervals.begin(), 
				    intervals.begin()+offset, intervals.end());
		intervals.erase( unique( intervals.begin(), intervals.end() ), 
				intervals.end() );
	}
	
	/**
	* @brief Removes the vertex v from the cube
	* @param v
	* @return An iterator to the new location of the 
		  element that followed the element erased 
	*/
	iterator remove( const vertex_type v){
		iterator pos = std::lower_bound( begin(), end(), v);
		//element not in list
		if (pos == end()){ return pos; }
		return intervals.erase( pos);
	}
	/**
	* @brief Removes the range [first, last) from the cube
	* @param v
	* @return An iterator to the new location of the 
		  element that followed the last element erased 
	*/

	iterator remove( iterator first, iterator last){
		return intervals.erase( first, last);
	}

	/**
	* @brief Copy assignment operator
	* @param Simplex & b
	* @return Reference to the assigned to cube
	*/
	Self& operator=( const Self & b) { 
		intervals = b.intervals; 
		return *this;
	}

	/**
	* @brief Move assignment operator 
	* @param Simplex && b
	* @return Reference to the assigned to cube
	*/
	Self& operator=( Self && b) {
		intervals = std::move( b.intervals); 
		return *this;
	}

	/**
	* @brief Comparator
	* Compares two simplices first by size, breaking ties lexicographically.
	* @param const Simplex & b
	* @return true if a < b, false otherwise. 
	*/
	bool operator<( const Self & b) const {
		return (size() < b.size()) || 
		 ((size() == b.size()) && 
		std::lexicographical_compare( begin(), end(), 
					      b.begin(), b.end()));
	}

	/**
	* @brief Equality operator
	* @param const Simplex & b
	* @return is equivalent to !(a < b) && !(b < a)
	*/
	bool operator==( const Self & b) const { 
		const bool equal_size = (b.size() == size());
		return equal_size && std::equal( begin(), end(), b.begin()); 
	}
	
	/**
	* @brief Non equality operator
	* @param const Simplex & b
	* @return true iff (*this) == b is false
	*/
	bool operator!=( const Self & b) const{ 
		const bool equal_size = (b.size() == size());
		return !equal_size || !std::equal( begin(), end(), b.begin());
	}

	private:
	Vector intervals;
}; //Cube
} //namespace ctl

#endif // ABSTRACT_SIMPLEX_H
