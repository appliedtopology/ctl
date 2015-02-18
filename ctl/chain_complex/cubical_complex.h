#ifndef CTL_CUBICAL_COMPLEX_H
#define CTL_CUBICAL_COMPLEX_H
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
* August 29, 2014
* Initial implementation of cubical complexes. 
* Based on PHAT/Hubert Wagners work.
*******************************************************************************/

//STL
#include <vector>
#include <numeric>
#include <sstream>
#include <fstream>
#include <functional>

//BOOST
#include <boost/serialization/base_object.hpp>
#include <boost/iterator/transform_iterator.hpp>

//CTL
#include <ctl/io/io.h>
#include <ctl/cube/cube.h>
#include <ctl/chain_complex/detail/cube_boundary_wrapper.h>

//BOOST
#include <boost/iterator/transform_iterator.hpp>

namespace ctl {
namespace detail {
//2*n-1
struct tnmo : public std::unary_function<std::size_t, std::size_t> {
	std::size_t operator()( const std::size_t & i) const { return 2*i-1; }
}; //end struct tnmo

template< typename Cell_,
	  typename Boundary_,
	  typename Data_,
	  typename Hash_, 
	  template< typename ...> class Storage_> 
class Cubical_complex  {
public: //Public types
   typedef detail::Cube_boundary_wrapper< Boundary_, 
					  Cubical_complex> Cell_boundary; 
   typedef Cell_ Cube; //Describes a fundamental object,
		       //e.g. Simplex, Cube, Polygon, etc..
   //Arbitrary data associated to space.
   typedef Data_ Data;
   typedef Hash_ Hash;
   typedef typename Cell_boundary::Cell Cell;

private: //Private types
   //Usually this is multi_array< Data> 
   typedef Storage_< Cell_, Data, Hash> Storage;
   typedef typename Storage::Coordinate  Vector;
public: //Public Types
   typedef typename std::size_t size_type;

   typedef typename Storage::iterator iterator;
   typedef typename Storage::const_iterator const_iterator;
   typedef typename Storage::reverse_iterator reverse_iterator;
   typedef typename Storage::const_reverse_iterator const_reverse_iterator;

   //Describes how to take Cell boundary
   //we overload the normal cubical boundary to make begin() and end() 
   //take as input the word we use to represent as a key and 
   //returns the word representation..
  template< typename X, typename Y>
  friend class Cube_boundary_wrapper;
public:
   //Constructors
   //!Default
   Cubical_complex(): cells(), index_data(), bd( *this) {}

public:
  /**
  * @brief 
  *
  * @tparam Vertex_extents
  * @param bd_
  * @param d_:
  */
   template< typename Vertex_extents>
   Cubical_complex( Boundary_ & bd_, Vertex_extents & d_):
    cells( boost::make_transform_iterator( d_.begin(), detail::tnmo()),  
	   boost::make_transform_iterator( d_.end(), detail::tnmo())), 
	   index_data( d_), bd( *this, bd_){ 
	   index_data.insert( index_data.begin(), 1);
   	   for( auto i = ++index_data.begin(); 
		     i != index_data.end(); ++i){ 
		     *i *= *(i-1); 
	   }
	   assign_keys();
  }
   
  /**
  * @brief Vertex_extents
  *
  * @tparam Vertex_extents
  * @param d_
  */
   template< typename Vertex_extents>
   Cubical_complex( const Vertex_extents& d_): 
     cells( boost::make_transform_iterator( d_.begin(), detail::tnmo()), 
	   boost::make_transform_iterator( d_.end(), detail::tnmo())), 
	   index_data( d_), bd( *this){
	  index_data.insert( index_data.begin(), 1);
   	   for( auto i = ++index_data.begin(); 
		     i != index_data.end(); ++i){ 
		     *i *= *(i-1); 
	  }
	 assign_keys();
  }

  /**
  * @brief 
  *
  * @tparam Vertex_extents
  * @param d_
  * @param offsets_
  */
   template< typename Vertex_extents, typename Offset>
   Cubical_complex( const Vertex_extents& d_,
		    const Offset& offset_): 
     cells( boost::make_transform_iterator( d_.begin(), detail::tnmo()), 
	    boost::make_transform_iterator( d_.end(), detail::tnmo()),
	    offset_),index_data( d_), bd( *this){ 
	  index_data.insert( index_data.begin(), 1);
 	   for( auto i = ++index_data.begin(); 
		     i != index_data.end(); ++i){ 
		     *i *= *(i-1); 
	  }
	  assign_keys();
   }
   
   /**
   * @brief boundary, length, and starting vertex constructor 
   * @tparam Vertex_extents
   * @param bd_
   * @param d_
   * @param offsets_
   */
   template< typename Vertex_extents, typename Function_values> 
   Cubical_complex( Cell_boundary & bd_, 
		    const Vertex_extents& d_,
		    Function_values & f,
		    const Vertex_extents& offsets_): 
     cells( boost::make_transform_iterator( d_.begin(), detail::tnmo()), 
	   boost::make_transform_iterator( d_.end(), detail::tnmo())), 
    bd( bd_), index_data( d_){ 
	  index_data.insert( index_data.begin(), 1);
 	   for( auto i = ++index_data.begin(); 
		     i != index_data.end(); ++i){ 
		     *i *= *(i-1); 
	  }
	  assign_keys();
   }

   //! Copy
   Cubical_complex( const Cubical_complex & b): 
   cells( b.cells), bd( b.bd), index_data( b.index_data) {}

   //! Move
   Cubical_complex( Cubical_complex && b): 
   cells( std::move( b.cells)), 
   bd( std::move( b.bd)),
   index_data( std::move( b.index_data))
   {}
   //! Assignment operator
   Cubical_complex& operator=( const Cubical_complex& b){
   	bd = b.bd;
   	cells = b.cells;
	index_data = b.index_data;
   	return *this;
   }

   //! Move assignment operator
   Cubical_complex& operator=( Cubical_complex&& b){
   	bd      = std::move( b.bd);
   	cells   = std::move( b.cells);
	index_data = std::move( b.index_data);
   	return *this;
   }
   bool operator==( const Cubical_complex & c) const {
	return (cells == c.cells) && (index_data == c.index_data);  
   }
   bool operator!=( const Cubical_complex & c) const {
	return !( this->operator==( c)); 
  } 
   /**
   * @brief find  a cell described as a product of intervals 
   *
   * @param s
   * @return 
   */
   iterator       find_cell( const Cell_ & s) {
	std::size_t linear_index = cell_to_word( s);
	return cells.begin()+linear_index;
   }

   /**
   * @brief find a cell described as a product of intervals 
   *
   * @param s
   * @return 
   */
   const_iterator find_cell( const Cell_ & s) const {
	std::size_t linear_index = cell_to_word( s);
	return cells.begin()+linear_index;
   }

   /**
   * @brief Look up cells based on a lattice indexing.  
   * @tparam Coordinate
   * @param s
   *
   * @return 
   */
   iterator find_cell( const Vector & s) {
	std::size_t linear_index = cells.coordinate_to_index( s);
	return cells.begin()+linear_index;
   }
   /**
   * @brief Look up cells based on a lattice indexing.
   * 
   * @tparam Coordinate
   * @param s
   * @return 
   */
   const_iterator find_cell( const Vector & s) const { 
	std::size_t linear_index = cells.coordinate_to_index( s);
	return cells.begin()+linear_index;
   }

   /**
   * @brief turns the bits + vertex encoding into an iterator
   * @param vertex_bits_index
   * @return 
   */
   const_iterator find_cell( std::size_t vertex_bits_index) const {
	std::size_t cell_index = vertex_bits_index >> cells.dimension();
	for( auto i = 0; i < cells.dimension(); ++i){
		std::size_t mask = vertex_bits_index & (1 << i);
		cell_index += (mask>0)*(offset( i));
	}
	return cells.begin()+cell_index; 
   }

   /**
   * @brief turns the bits + vertex encoding into an iterator
   * @param vertex_bits_index
   * @return 
   */
   iterator find_cell( std::size_t vertex_bits_index){
	std::size_t cell_index = vertex_bits_index >> cells.dimension();
	for( auto i = 0; i < cells.dimension(); ++i){
		std::size_t mask = vertex_bits_index & (1 << i);
		cell_index += (mask>0)*(offset( i));
	}
	return cells.begin()+cell_index; 
   }
   
   iterator       		begin()       	{ return cells.begin(); } 
   iterator         		  end()       	{ return cells.end();   }
 
   const_iterator       	begin()  const 	{ return cells.begin(); } 
   const_iterator         	  end()  const 	{ return cells.end();   }
  
   reverse_iterator            rbegin()       	{ return cells.rbegin(); } 
   reverse_iterator         	 rend()       	{ return cells.rend();   }
 
   const_reverse_iterator       rbegin() const  { return cells.rbegin(); } 
   const_reverse_iterator        rend()  const  { return cells.rend();   }

   template< typename Stream, typename Functor>
   Stream& write( Stream& out, const Functor & f) const {
   	out << "size " << cells.size() << std::endl;
   	for( const auto & cell: cells){
   	  out << f( cell.second)  << std::flush;
   	  out << std::endl;
   	}
	return out;
   }

   template< typename Stream>
   inline Stream& write( Stream& out) const { 
	out << "size " << cells.size() << std::endl;
   	for( const auto & cell: cells){
   	  out << cell.first.size() << " " << std::flush;
	  cell.first.write( out);
	  out << std::flush;
   	  out << cell.second.id()  << std::flush;
	  out << std::endl;  
 	}
	return out;
   }
   template< typename Stream> 
   Stream& read( Stream & in){
    std::size_t line_num = 0;
    std::string line;
    char the_first_character = in.peek();
    const bool headers_enabled = (the_first_character == 's');
    if( !headers_enabled) { 
    	std::cerr << "Error: reading a file without appropriate header."
    		  << "Bailing out." << std::endl;
    	std::exit( -1);
    }
    //Read the header and reserve appropriately
    ctl::get_line( in, line, line_num);
    std::istringstream ss( line);
    std::string the_word_size;
    std::size_t max_dim;  
    ss >> the_word_size;
    ss >> max_dim; 
    //keep track of the number of index_data.
    index_data.resize( max_dim+1, 1);
    std::size_t length;
    for( std::size_t i = 0; i < max_dim; ++i){
    	ss >> length;
    	index_data[ i+1] =  length; 
    }
    
    //we think of a grid which is the 
    //cartesian product like this:
    //*-*-*-*-* x *-*-*-*
    detail::tnmo t;
    cells.resize( boost::make_transform_iterator( ++index_data.begin(), t),
       	          boost::make_transform_iterator( index_data.end(), t));
    
    for( auto i = ++(index_data.begin()); 
    	      i != index_data.end(); ++i){ *i *= *(i-1); }
    std::size_t vertex_id=0;
    while( ctl::get_line(in, line, line_num)){
         std::istringstream ss( line);
         //and it's id
         Data d( vertex_id);
	 ss >> d;
	 std::size_t vertex_index = vertex_id_to_index( vertex_id); 
	 auto& p = cells[ vertex_index];
	 p.first = vertex_id; 
	 p.second = d;
	 vertex_id++;
    }
    assign_keys(); 
    return in;
   }

    
   std::size_t id_and_bits_to_index( std::size_t id_and_bits) const {
	//By linearity the id_and_bits_encoding can be immediately turned into
	//the array_index	
	std::size_t vertex_id = id_and_bits >> cells.dimension();
	for( auto i = 0; i < cells.dimension(); ++i){ 
		std::size_t mask = ((id_and_bits&(1 <<i)) > 0);
		vertex_id += mask*index_data[ i]; 
	}
	return vertex_id;
   }

   template< typename Coordinate>
   Coordinate& id_and_bits_to_coordinate( std::size_t id_and_bits, const Coordinate & c) const{
	return index_to_coordinate( id_and_bits_to_index( id_and_bits), c);
   }
 
   template< typename Coordinate>
   std::size_t coordinate_to_id_and_bits( const Coordinate & c) const {
	Coordinate vertex_coords( c);
	std::size_t pos=0;
	//in 0 based systems go from coordinates to vertex coordinates
	//by making each coordinate even.
	//here we do
	// i -= starting_index_in_this_dimension
	// i -= i%2
	// i += starting_index_in_this_dimension
	//this is equivalent
	std::size_t mask = 0;
	for( auto & i : vertex_coords){ 
		i -= cells.base( pos);
		mask ^= ((i%2) << pos);
		i -= i%2;
		i += cells.base( pos); 
		++pos;
	}
	std::size_t t = cells.coordinate_to_index( vertex_coords);
	t <<= c.size();
	t ^= mask;
	return t;
   }

   void reserve( const std::size_t n) { cells.reserve( n); }
   const std::size_t dimension() const { return cells.dimension(); }
   const std::size_t dimension( const Cell & c) const { 
	std::size_t mask = (1 << (dimension()+1)) - 1;
	return __builtin_popcount( c & mask);
   }
   const std::size_t size() const { return cells.size(); }
   const std::size_t size( std::size_t i) const { return cells.extents( i); }
   Cell_boundary& cell_boundary() { return bd; }
   bool is_closed() const{
   	for( auto sigma : cells){
   		for( auto tau = bd.begin( sigma.first);
   			  tau != bd.end( sigma.first); ++tau){
   			if( find_cell( tau->cell()) == end()){
   				return false;
   			}
   		}
   	}
   	return true;
   }

   std::size_t offset( std::size_t i) const { if( i){ return cells.offsets( i-1); } return 1; }
//Private functions
private:
  void assign_keys(){
    Vector c;
    std::size_t p=0;
    for( auto i = cells.begin(); i != cells.end(); ++i, ++p){
	cells.index_to_coordinate( p, c);
        i->first = coordinate_to_id_and_bits( c);
    }  
  }


/**
* @brief This method is used to convert vertex_id's 
* 0 ... #vertex --> coordinates in the larger complex.
* This is _not_ to be used to convert the vertex id in 
* the high bits of a key to a coordinate. That will result
* in a bug!
* 
* @tparam Coordinate
* @param index
* @param c
*/
  template< typename Coordinate>
  void vertex_id_to_coordinate( std::size_t index, Coordinate & c) const{
    for( auto i = dimension(); i>0;  --i){ 
       c[i-1] = index/ index_data[ i];
       index -= c[i-1]*(index_data[ i]);
       c[i-1] = 2*c[i-1]+ cells.base( i-1);
    }
  }


/**
* @brief This method is used to convert vertex_id's 
* 0 ... #vertex --> coordinates in the larger complex.
* This is _not_ to be used to convert the vertex id in 
* the high bits of a key to a coordinate. That will result
* in a bug!
* 
* @param index
* @param c
*/
  std::size_t vertex_id_to_index( std::size_t index) const{
    Vector c( dimension(), 0);
    vertex_id_to_coordinate( index, c);
    return cells.coordinate_to_index( c);
  }

public:

/**
* @brief Given a cell encoded as a product of intervals
* (each interval a product of vertex ids)
* we return a unique id associate to this cell.
* The id encodes the lower left vertex of the cube
* and then in the lowest d bits the sum of the standard 
* d-dimensional basis vectors which encodes the difference from this point.
* E.g.  |=   this picture encodes a cubical complex where . is a vertex - and | 
        ._   are edges and = is a face.
* where . is say vertex 3 = 11b
* we encode the edge | as 1110b 
*  the edge - as 1101b 
* and the face = as  1111b
* In case its not clear to you 
* the highest two bits in this example correspond to writing 3 in binary 
* @param cell
*
* @return 
*/
  Cell cube_to_key( const Cube & cube) const{
    Cell key=0;
    Cell set_bits = 0;
    auto begin = cells.offsets().begin();
    auto end = cells.offsets.end();
    for( auto & i : cube){
      key = std::min( i.first, i.second);
      set_bits &= (1 << (std::find( begin, end, i.second - i.first) - begin));
    }
    key <<= cells.dimension();
    key &= set_bits;
    return key;
  }

 /**
 * @brief given the bit encoding of a cube as vertex_id_in_binary|b_1...b_d
 * where bi specifies the displacement from the lower left vertex_id
 *
 * This function produces the cell representation as a product of intervals
 * each interval is a product of vertices. 
 *
 * @param word
 * @param cell
 */
  Cube& key_to_cube( const Cell word1, Cube & r) const{
   //typedef typename Cube::Interval Interval;
   Cube cube;
   Cell word = word1;
   std::size_t lower_left_vertex_id = word >> dimension();
   word ^= (lower_left_vertex_id << dimension());
   std::vector< std::size_t> set_bits;
   set_bits.reserve( dimension());
    std::size_t vid = lower_left_vertex_id;
   if (word == 0){
     	cube.insert( {{vid, vid}});
	r.swap( cube);
   	return r;
   }
   for( auto i = 0; i < dimension(); ++i){ 
     if((word1 & (1 << i)) != 0){ set_bits.push_back( i); } 
   }
   cube.reserve( set_bits.size());
   for( auto & i : set_bits){
     cube.insert( {{vid, vid + 2*offset( i)}});
   } 
   r.swap( cube);
   return r;
  }

//Private members
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const std::size_t version){
     ar & cells;
     ar & index_data; 
  }
  Storage cells;
  //precomputed products for offsets into the array
  Vector index_data;
  Cell_boundary bd;
}; //chain_complex


/**
* @brief Helper function for converting the key representation of a cube to a 
*        product of intervals.
* @tparam Key
* @tparam Args
* @param complex
* @param key
*
* @return 
*/
template< typename Cubical_complex> 
typename Cubical_complex::Cube 
key_to_cube( const Cubical_complex & complex, 
	     const typename Cubical_complex::Cell & key){
	typedef typename Cubical_complex::Cube  Cube;
	Cube cube;	
	complex.key_to_cube( key, cube);
	return cube;
}

template< typename ... Args>
typename ctl::detail::Cubical_complex< Args ...>::Cell 
cube_to_key( const ctl::detail::Cubical_complex< Args ...>& complex, 
	  const typename ctl::detail::Cubical_complex< Args ...>::Cube & cube){ 
	return complex.cube_to_key( complex, cube);
}

template< typename Cubical_complex>
std::size_t cube_dimension( const Cubical_complex& complex, 
			    const typename Cubical_complex::Cell & cell){ 
	std::size_t mask = (1 << (complex.dimension()+1)) - 1;
	return __builtin_popcount( cell & mask);
}


template< typename Stream, typename ... Args>
Stream& operator<<( Stream& out, 
		    const typename ctl::detail::Cubical_complex< Args...>::
					iterator c){ 
	out << c->first;
	return out;	
}


template< typename Stream, typename ... Args>
Stream& operator<<( Stream& out, 
		    const typename ctl::detail::Cubical_complex< Args...>::
					const_iterator c){ 
	out << c->first;
	return out;	
}

template< typename Stream, typename ... Args>
Stream& operator<<( Stream& out, 
		    const ctl::detail::Cubical_complex< Args...> & c){ 
	typedef ctl::detail::Cubical_complex< Args...> Cubical_complex;
	typedef typename Cubical_complex::Cube Cube;
   	Cube cube; 
	for(auto i = c.begin(); i != c.end(); ++i){
		      const std::size_t id = i->second.id();
		      out << id; 
		      out << ": ";
		      out << c.key_to_cube( i->first, cube) << std::endl; 
	}
	return out;
}

template< typename Stream, typename ... Args>
Stream& operator<<( Stream& out, 
   const ctl::detail::Cubical_complex< Args...>  && c){ 
	out << c;
	return out;
}

template< typename Stream, typename ... Args>
Stream& operator>>( Stream& in, 
		    ctl::detail::Cubical_complex< Args...> & c){  
return c.read( in); 
}
} //end namespace detail

} //namespace ctl

#endif //CTL_CHAIN_COMPLEX_MAP_H
