#ifndef ABSTRACT_SIMPLEX_H
#define ABSTRACT_SIMPLEX_H
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
*******************************************************************************/
#include <vector> //std::vector
#include <initializer_list>  // std::initializer_list
#include <iostream> //cout (debug only)
#include <algorithm> //sort, unique
#include <ctl/abstract_simplex/simplex_boundary.h>
//! \namespace ctl
namespace ctl {

/**
* \class Abstract_simplex< T>
* @brief 
*/
template< typename T>
class Abstract_simplex {
	private:
	typedef typename std::vector< T> Vector;
	typedef Abstract_simplex< T> Self;
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
	Abstract_simplex( size_t d) { vertices.reserve( d); }
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
			  const Iterator end): vertices( begin,end){}
	//! Copies the data from one simplex to another
	Abstract_simplex( const Self & from): vertices( from.vertices){}
	//! Moves the data from one simplex to another
	Abstract_simplex( Self && from): vertices( std::move( from.vertices)){}
	//! returns an iterator to the first vertex in the simplex
	iterator       begin()	        { return vertices.begin(); }
	//! returns a const_iterator to the first vertex in the simplex
	const_iterator begin()  const	{ return vertices.begin(); }
	
	//! returns an iterator to the past-the-end vertex in the simplex
	iterator         end()	  	{ return vertices.end();   }
	//! returns a const_iterator to the past-the-end vertex in the simplex
	const_iterator   end()  const	{ return vertices.end();   }
	
	//! returns a reverse_iterator to the last element in the simplex
	reverse_iterator        rbegin()	{ return vertices.rbegin(); }
	//! returns a const_reverse_iterator to the last element in the simplex
	const_reverse_iterator  rbegin()  const	{ return vertices.rbegin(); }
	/**
	 * Returns a reverse_iterator pointing to the theoretical element 
 	 * preceding the first element in the container 
	 * (which is considered its reverse end).
	 * @param None.
	 * @return reverse_iterator
	 */
	reverse_iterator         rend()	        { return vertices.rend(); }
	/**
	 * Returns a const_reverse_iterator pointing to the theoretical element 
 	 * preceding the first element in the container 
	 * (which is considered its reverse end).
	 * @param None.
	 * @return const_reverse_iterator
	 */
	const_reverse_iterator   rend()  const	{ return vertices.rend(); }
	/** Returns the size of the simplex
	 * @param None.
	 * @return size_t
	 */
	size_t       size() const	{ return vertices.size(); 	}
	/** Returns the dimension of the simplex
	 * @param None.
	 * @return size_t
	 */
	size_t  dimension() const	{ return size()-1; 	  	}

	/** Returns the capacity of the simplex
	* @param None.
	* @return size_t
	*/
	size_t   capacity() const  { return vertices.capacity();   }

	/**
	* Inserts the vertex v if it doesn't already exist
	* @param const vertex_type & v
	* @return iterator to the vertex v in the simplex
	*/
	iterator insert( const vertex_type & v){
	      iterator pos = std::lower_bound( begin(), end(), v);
	      if(pos != end() && *pos == v) { return pos; }
	      return vertices.insert( pos, v);
	}

	/**
	* Inserts the vertices in an initializer list 
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
		vertices.erase( unique( vertices.begin(), vertices.end() ), 
				vertices.end() );
	}

	iterator remove( const vertex_type v){
		iterator pos = std::lower_bound( begin(), end(), v);
		//element not in list
		if (pos == end()){ return pos; }
		return vertices.erase( pos);
	}
	
	iterator remove( iterator first, iterator last){
		return vertices.erase( first, last);
	}

	Self& operator=( const Self & b) { 
		vertices = b.vertices; 
		return *this;
	}
	Self& operator=( Self && b) {
		vertices = std::move( b.vertices); 
		return *this;
	}
	bool operator<( const Self & b) const {
		
		return (size() < b.size()) || 
		 ((size() == b.size()) && 
		std::lexicographical_compare( begin(), end(), 
					      b.begin(), b.end()));
	}
	bool operator==( const Self & b) const { 
		const bool equal_size = (b.size() == size());
		return equal_size && std::equal( begin(), end(), b.begin()); 	    
	}
	bool operator!=( const Self & b) const{ 
		const bool equal_size = (b.size() == size());
		return !equal_size || !std::equal( begin(), end(), b.begin());
	}

	template< typename Stream>
	Stream& write( Stream & out) const {
		for( auto i : vertices){ 
			out << i;
			out << " "; 
		}
		return out;
	}

	template< typename Stream>
	Stream& read( Stream & in)  {
		T vertex;
		while( in.good()){
			in >> vertex;
			insert( vertex);
		}
		return in;
	}

	template< typename Stream>
	Stream& read( Stream & in, std::size_t size) {
		vertices.reserve( size);
		T vertex;
		while( in.good()){
			in >> vertex;
			insert( vertex);	
		}
		return in;
	}

	private:
	Vector vertices;

	//typename Self since the compiler complains
	template< typename Term> 
	friend class ctl::detail::const_simplex_boundary_iterator;
}; //Abstract_simplex

template< typename Stream, typename T>
inline Stream& operator>>( Stream & in, ctl::Abstract_simplex< T> & simplex){ return simplex.read( in); }
} // namespace ctl
	
template< typename Stream, typename T>
inline Stream& operator<<(Stream& out, const ctl::Abstract_simplex< T>& simplex){
	typedef typename ctl::Abstract_simplex< T>::const_iterator iterator;
	out << "[";
	for(iterator i = simplex.begin(); i != simplex.end(); ++i){
		out << *i;
		if ( i+1 != simplex.end()) { out << ", "; } else { out << "]";}
	}
	return out;
}
template< typename Stream, typename T>
Stream& operator<<(Stream& out, const ctl::Abstract_simplex< T>&& simplex){ out << simplex; }

#endif // ABSTRACT_SIMPLEX_H
