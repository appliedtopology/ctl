#ifndef ABSTRACT_SIMPLEX_H
#define ABSTRACT_SIMPLEX_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* Code release under BSD-3 License. See LICENSE.
*******************************************************************************/
//STL
#include <vector> //std::vector
#include <initializer_list>  // std::initializer_list
#include <iostream> //cout (debug only)
#include <algorithm> //sort, unique


namespace ctl {

/*!
* \class Abstract_simplex
* This class describes an abstract simplex for a simplicial chain complex.
* It is implemented as a sorted std::vector< T> since 
* maintaining a sorted vector is faster than a std::set 
* for a fixed large number.
*
* This object does not itself have the facility for a boundary, this is handled
* by a separate boundary operator.
* @tparam T the underlying vertex type. T must be totally ordered via <
*/
class Abstract_simplex {
	private:
	typedef std::size_t T;
	typedef typename std::vector< T> Vector;
	typedef Abstract_simplex Self;
	typedef std::initializer_list< T> Init_list;
	public:
	//! \typedef  
	typedef std::size_t size_t;
	//! \typedef synonomous with vertex_type  
	typedef typename Vector::value_type value_type;
	//! \typedef The vertex type of the simplex  
	typedef typename Vector::value_type vertex_type;
	//! \typedef const_iterator to the simplex
	typedef typename Vector::const_iterator const_iterator;
	//! \typedef const_reverse_iterator to the simplex
	typedef typename Vector::const_reverse_iterator const_reverse_iterator;
	//! \typedef an iterator to the simplex
	typedef typename Vector::iterator iterator;
	//! \typedef reverse_iterator to the simplex
	typedef typename Vector::reverse_iterator reverse_iterator;
	public:
	//! Default constructor
	Abstract_simplex(): vertices(){};
	//! Reserves space for at least d vertices.
	Abstract_simplex( size_t d): vertices() { vertices.reserve( d); }
	//! Reserves space for at least d vertices and initializes them to t
	Abstract_simplex( size_t d, const T & t): vertices( d, t){}
	//! Initializer list constructor
	Abstract_simplex( const Init_list & il) : vertices( il) {
		sort( vertices.begin(), vertices.end() );
		vertices.erase( unique( vertices.begin(), vertices.end() ), 
				vertices.end() );

	}
	//!  Range constructor 
	template< typename Iterator>
	Abstract_simplex( const Iterator begin, 
			  const Iterator end): vertices( begin, end){
		sort( vertices.begin(), vertices.end() );
		vertices.erase( unique( vertices.begin(), vertices.end() ), 
				vertices.end() );
	}
	//! Copies the data from one simplex to another
	Abstract_simplex( const Self & from): vertices( from.vertices){}
	//! Moves the data from one simplex to another
	Abstract_simplex( Self && from): vertices( std::move( from.vertices)){}
	//! returns an iterator to the first vertex in the simplex
	iterator       begin()	        { return vertices.begin(); }
	//! returns a const_iterator to the first vertex in the simplex
	const_iterator begin()  const	{ return vertices.begin(); }
	T front() const { return vertices.front(); }
	T back() const { return vertices.back(); }
	/*!
	* @brief  returns an iterator to the past-the-end vertex in the simplex
	* @return iterator
	*/
	iterator         end()	  	{ return vertices.end();   }
	/**
	* @brief returns a const_iterator to the past-the-end vertex in the simplex
	* @return const_iterator
	*/
	const_iterator   end()  const	{ return vertices.end();   }
	/*!
	* @brief returns a reverse_iterator to the last vertex in the simplex
	* @return reverse_iterator
	*/
	reverse_iterator        rbegin()	{ return vertices.rbegin(); }
	/*!
	* @brief returns a const_reverse_iterator to the last vertex in the simplex
	* @return const_reverse_iterator
	*/
	const_reverse_iterator  rbegin()  const	{ return vertices.rbegin(); }
	/*!
	 * @brief Returns a reverse_iterator pointing to the theoretical element 
 	 * preceding the first element in the container 
	 * (which is considered its reverse end).
	 * @return reverse_iterator
	 */
	reverse_iterator         rend()	        { return vertices.rend(); }
	/*!
	 * @brief Returns a const_reverse_iterator pointing to the theoretical element 
 	 * preceding the first element in the container 
	 * (which is considered its reverse end).
	 * @return const_reverse_iterator
	 */
	const_reverse_iterator   rend()  const	{ return vertices.rend(); }
	/*! Returns the size of the simplex
	 * @return size_t
	 */
	size_t       size() const	{ return vertices.size(); 	}
	/*! Returns the dimension of the simplex
	 * @return size_t
	 */
	size_t  dimension() const	{ return size()-1; 	  	}
	/*!
	 * Reserves as much capacity as is necessary to store
	 * at least d elements.
 	 */
	void reserve( std::size_t d){ vertices.reserve(d); }
	/*! Returns the capacity of the simplex
	* @return size_t
	*/
	size_t   capacity() const  { return vertices.capacity();   }
	/*! Removes all vertices (which are destroyed), 
	 * leaving the cell with a size of 0.*/
	void clear() { vertices.clear(); }

	/*!
	* @brief Inserts the vertex v if it doesn't already exist
	* @param const vertex_type & v
	* @return iterator to the vertex v in the simplex
	*/
	iterator insert( const vertex_type & v){
	      iterator pos = std::lower_bound( begin(), end(), v);
	      if(pos != end() && *pos == v) { return pos; }
	      return vertices.insert( pos, v);
	}

	/*!
	* @brief Inserts the vertices in an initializer list 
	* @param std::initalizer_list< T>
	* @return None.
	*/
	void insert( const Init_list & il){
	        std::size_t offset = vertices.size();
		vertices.insert( vertices.end(), il.begin(), il.end());
	        std::inplace_merge( vertices.begin(), 
				    vertices.begin()+offset, vertices.end());
		vertices.erase( unique( vertices.begin(), vertices.end() ), 
				vertices.end() );
	
	}

	/**
	* Inserts the range [begin, end) into the simplex 
	* @param Iterator begin, Iterator end
	* @return None.
	*/
	template< typename Iterator>
	void insert( Iterator first, Iterator last){
	        std::size_t offset = vertices.size();
		vertices.insert( vertices.end(), first, last);
		std::inplace_merge( vertices.begin(), 
				    vertices.begin()+offset, vertices.end());
		vertices.erase( std::unique( vertices.begin(), vertices.end() ), 
				vertices.end() );
	}
	
	/**
	* @brief Removes the vertex v from the simplex
	* @param v
	* @return An iterator to the new location of the 
		  element that followed the element erased 
	*/
	iterator remove( const vertex_type v){
		iterator pos = std::lower_bound( begin(), end(), v);
		//element not in list
		if (pos == end()){ return pos; }
		return vertices.erase( pos);
	}
	/**
	* @brief Removes the range [first, last) from the simplex
	* @param v
	* @return An iterator to the new location of the 
		  element that followed the last element erased 
	*/

	iterator remove( iterator first, iterator last){
		return vertices.erase( first, last);
	}

	/**
	* @brief Copy assignment operator
	* @param Simplex & b
	* @return Reference to the assigned to simplex
	*/
	Self& operator=( const Self & b) { 
		vertices = b.vertices; 
		return *this;
	}

	/**
	* @brief Move assignment operator 
	* @param Simplex && b
	* @return Reference to the assigned to simplex
	*/
	Self& operator=( Self && b) {
		vertices = std::move( b.vertices); 
		return *this;
	}
	
	/**
	* @brief Move assignment operator 
	* @param Vector< Vertex_type> && b
	* @return Reference to the assigned to simplex
	*/
	Self& operator=( Vector && b) {
		vertices = std::move( b);
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

	/**
	* @brief write function
	* writes the simplex to this output stream
	* @tparam Stream
	* @param out
	* @return Stream&
	*/
	template< typename Stream>
	Stream& write( Stream & out) const {
		for( auto i : vertices){ 
			out << i;
			out << " "; 
		}
		return out;
	}

	/**
	* @brief read function
	* Reads the simplex from an output stream
	* @tparam Stream
	* @param out
	* @return Stream&
	*/
	template< typename Stream>
	Stream& read( Stream & in)  {
		T vertex;
		while( in.good()){
			in >> vertex;
			insert( vertex);
		}
		return in;
	}

	/**
	* @brief read function
	* Reads the simplex from an output stream
	* assumes 
	* @pre size of the resulting simplex is given by the size parameter
	* @tparam Stream
	* @param std::size_t
	* @param out
	* @return Stream&
	*/
	template< typename Stream>
	Stream& read( Stream & in, std::size_t size) {
		vertices.reserve( size);
                T vertex;
                for( std::size_t i = 0; in.good() && i < size; ++i){
                        in >> vertex;
                        insert( vertex);    
                }   
                return in; 
	}

	private:
	Vector vertices;

}; //Abstract_simplex


/**
* @brief Input stream operator for a simplex
*
* @tparam Stream
* @tparam T
* @param in
* @param simplex
*  Reads a simplex from the input stream
* @return in
*/
template< typename Stream>
Stream& operator>>( Stream & in, ctl::Abstract_simplex & simplex){ return simplex.read( in); }
	
/**
* @brief Output stream operator for a simplex
*
* @tparam Stream
* @tparam T
* @param out
* @param simplex
* Writes a simplex to this output stream
* @return Stream&
*/
std::ostream& operator<<(std::ostream& out, const ctl::Abstract_simplex& simplex){
	out << "[" << std::flush;
	if( simplex.size() != 0) {
		for(auto i = simplex.begin(); i != simplex.begin()+simplex.dimension(); ++i){
			out << *i << ", ";
		}
		if( simplex.rbegin() != simplex.rend()){
			out << *(simplex.rbegin()); 
		}
	}
	out << "]" << std::flush;
	return out;
}

/*template< typename Stream>
Stream& operator<<(Stream& out, ctl::Abstract_simplex&& simplex){ 
	out << simplex; 
	return out;
}*/
} //namespace ctl

#endif // ABSTRACT_SIMPLEX_H
