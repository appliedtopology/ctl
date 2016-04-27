#ifndef CUBE_H
#define CUBE_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
* Released under BSD-3 License. See LICENSE
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
#include <algorithm> //swap, sort, unique

/*! \namespace ctl
Namespace where all library functionality resides
*/
namespace ctl {


/**
* \class Cube
* This class describes an cube for a cubical chain complex.
* While a cube is defined as a product of intervals, a cubical complex
* does not store a vector of these. A cube is infact stored implicitly
* as an address to its data. This class exists as a mirage.  
* It can be used for "by hand" computations and examples. The cubical complex
* produces a "cube," for example when deferencing an iterator over the range
* of stored cubes, however, it is generated on the fly.
*
* This object does not itself have the facility for a boundary, this is handled
* by a separate boundary operator.
*/
class Cube {
	public:
	typedef typename std::pair< std::size_t, std::size_t> Interval;
	private:
	typedef typename std::vector< Interval> Vector;
	typedef Cube Self;
	typedef std::initializer_list< Interval> Init_list;
	public:
	//! \typedef  
	typedef std::size_t size_t;
	//! \typedef synonomous with vertex_type  
	typedef typename Vector::value_type value_type;
	//! \typedef The vertex type of the cube  
	typedef std::size_t vertex_type;
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
	Cube(): intervals(), dimension_( 0){};
	//! Reserves space for at least d intervals.
	Cube( size_t d) : dimension_(0){ intervals.reserve( d); }
	//! Creates a vertex [t,t]
	Cube( size_t d, const std::size_t & t): intervals( 1, Interval( t, t)), dimension_( 0){}
	//! Initializer list constructor
	Cube( const Init_list & il) : intervals( il), dimension_( 0) {
		sort( intervals.begin(), intervals.end() );
		intervals.erase( unique( intervals.begin(), intervals.end() ), 
				intervals.end() );
		for( auto& i: intervals){ dimension_ += (i.first == i.second); }
	}

	//!  Range constructor 
	template< typename Iterator>
	Cube( const Iterator begin, const Iterator end): 
	intervals( begin,end), dimension_( 0){
		for( auto& i: intervals){ dimension_ += (i.first == i.second); }
	}

	//! Copies the data from one cube to another
	Cube( const Self & from): intervals( from.intervals), dimension_( from.dimension_){}

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

	/*! 
	* Returns the total number of elements in the cube.
	* @return size_t 
	*/
	size_t  size() const	{ return intervals.size(); 	  	}
	/*! Returns the dimension of the cube
	 * @return size_t
	 */
	size_t  dimension() const	{ return dimension_; 	  	}

	/*! Returns the capacity of the cube
	* @return size_t
	*/
	size_t   capacity() const  { return intervals.capacity();   }
	/*! Removes all intervals (which are destroyed), 
	 * leaving the cell with a size of 0.*/
	void clear() { intervals.clear(); }

	/*!
	* @brief Inserts the interval v if it doesn't already exist
	* @param const vertex_type & v
	* @return iterator to the vertex v in the cube
	*/
	iterator insert( const value_type & v){
	      iterator pos = std::lower_bound( begin(), end(), v);
	      if(pos != end() && *pos == v) { return pos; }
	      dimension_ += (v.first == v.second);
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
		dimension_ = 0;
		for( auto&i: intervals){ dimension_ += (i.first == i.second); }	
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
		dimension_ = 0;
		for( auto&i: intervals){ dimension_ += (i.first == i.second); }	
	
	}
	
	/**
	* @brief Removes the vertex v from the cube
	* @param v
	* @return An iterator to the new location of the 
		  element that followed the element erased 
	*/
	iterator remove( const value_type v){
		iterator pos = std::lower_bound( begin(), end(), v);
		//element not in list
		if (pos == end()){ return pos; }
		dimension_ -= (pos->first == pos->second);
		return intervals.erase( pos);
	}
	/**
	* @brief Removes the range [first, last) from the cube
	* @param v
	* @return An iterator to the new location of the 
		  element that followed the last element erased 
	*/

	iterator remove( iterator first, iterator last){
		auto r= intervals.erase( first, last);
		dimension_ = 0;
		for( auto&i: intervals){ dimension_ += (i.first != i.second); }	
		return r;	
	}

	/**
	* @brief Copy assignment operator
	* @param Cube & b
	* @return Reference to the assigned to cube
	*/
	Self& operator=( const Self & b) { 
		intervals = b.intervals;
		dimension_ = b.dimension_; 
		return *this;
	}

	/**
	* @brief Move assignment operator 
	* @param Cube && b
	* @return Reference to the assigned to cube
	*/
	Self& operator=( Self && b) {
		intervals = std::move( b.intervals); 
		dimension_ = std::move( b.dimension_); 
		return *this;
	}

	/**
	* @brief Comparator
	* Compares two simplices first by size, breaking ties lexicographically.
	* @param const Cube & b
	* @return true if a < b, false otherwise. 
	*/
	bool operator<( const Self & b) const {
		return (size() < b.size()) || 
		 ((size() == b.size()) && 
		std::lexicographical_compare( begin(), end(), 
					      b.begin(), b.end()));
	}
	void reserve( std::size_t n){ intervals.reserve( n); }
	void resize( std::size_t n){ intervals.resize( n); }
	void swap( Cube & r){ intervals.swap( r.intervals); 
			      std::swap( dimension_, r.dimension_);} 
	/**
	* @brief Equality operator
	* @param const Cube & b
	* @return is equivalent to !(a < b) && !(b < a)
	*/
	bool operator==( const Self & b) const { 
		const bool equal_size = (b.size() == size());
		return equal_size && std::equal( begin(), end(), b.begin()); 
	}
	
	/**
	* @brief Non equality operator
	* @param const Cube & b
	* @return true iff (*this) == b is false
	*/
	bool operator!=( const Self & b) const{ 
		const bool equal_size = (b.size() == size());
		return !equal_size || !std::equal( begin(), end(), b.begin());
	}

	private:
	Vector intervals;
	size_t dimension_;

}; //Cube

std::ostream& operator<<( std::ostream & out, const ctl::Cube & c){
    for( auto i = c.begin(); i != c.end(); ++i){
	
    	out << "[" << i->first;
	if ( i->first != i->second){
		out  << "," << i->second ;
	}
	out << "]";  
    	if( i+1 != c.end()){ out << " x "; }		
    }
	return out;
} 
} //namespace ctl

#endif //CUBE_H
