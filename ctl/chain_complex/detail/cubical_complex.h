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

//BOOST
#include <boost/iterator/transform_iterator.hpp>

//CTL
#include <ctl/io/io.h>
#include <ctl/cube/cube.h>

namespace ctl {
namespace detail {

template< typename Cell_,
	  typename Boundary_,
	  typename Data_,
	  typename Hash_, 
	  template< typename ...> class Storage_> 
class Cubical_complex  {
public: //Public types
   typedef Cell_ Cell; //Describes a fundamental object,
		       //e.g. Simplex, Cube, Polygon, etc..

   typedef Boundary_ Cell_boundary; //Describes how to take Cell boundary
   //Arbitrary data associated to space.
   typedef Data_ Data;
   typedef Hash_ Hash;

private: //Private types
   typedef Storage_< Cell, Data, Hash> Storage;
   typedef std::vector< std::size_t>  Vector;
    
public: //Public Types
   typedef typename std::size_t size_type;
   typedef typename Storage::iterator iterator;
   typedef typename Storage::const_iterator const_iterator;
   typedef typename std::pair< Cell, Data> value_type;

public:
   //Constructors
   //Default
   Cubical_complex() {}

   template< typename Vertex_extents>
   Cubical_complex( Cell_boundary & bd_, Vertex_extents & d_):
    cells( boost::make_transform_iterator( d_.begin(), 
					  [](const std::size_t & i){ return 2*i-1; }), 
	   boost::make_transform_iterator( d_.end(), 
					  [](const std::size_t & i){ return 2*i-1; })), 
	   bd( bd_), index_data( d_) {
   	   for( auto i = ++index_data.begin(); 
		     i != index_data.end(); ++i){ 
		     *i *= *(i-1); 
		}
	}

   template< typename Vertex_extents> 
   Cubical_complex( Cell_boundary & bd_, 
		    const Vertex_extents& d_,
		    const Vertex_extents& offsets_): 
     cells( boost::make_transform_iterator( d_.begin(), 
					  [](const std::size_t & i){ return 2*i-1; }), 
	   boost::make_transform_iterator( d_.end(), 
					  [](const std::size_t & i){ return 2*i-1; })), 
    bd( bd_), index_data( d_){}

   //Copy
   Cubical_complex( const Cubical_complex & b): 
   cells( b.cells), bd( b.bd), index_data( b.index_data) {}

   //Move
   Cubical_complex( Cubical_complex && b): 
   cells( std::move( b.cells)), bd( std::move( b.bd)),
   index_data( std::move( b.index_data)) {}

   //Assignment operator
   Cubical_complex& operator=( const Cubical_complex& b){
   	bd = b.bd;
   	cells = b.cells;
	index_data = b.index_data;
   	return *this;
   }

   //Move assignment operator
   Cubical_complex& operator=( Cubical_complex&& b){
   	bd      = std::move( b.bd);
   	cells   = std::move( b.cells);
	index_data = std::move( b.index_data);
   	return *this;
   }

   iterator       find_cell( const Cell & s) {
	std::size_t linear_index = cell_to_word( s);
	return cells.begin()+linear_index;
   }

   const_iterator find_cell( const Cell & s) const { 
	std::size_t linear_index = cell_to_word( s);
	return cells.begin()+linear_index;
   }

   iterator       begin()       { return cells.begin(); }
   iterator         end()       { return cells.end();   }

   const_iterator begin() const { return cells.begin(); }
   const_iterator   end() const { return cells.end();   }

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
	index_data.resize( max_dim);
	std::size_t length;
	for( std::size_t i = 0; i < max_dim; ++i){
		ss >> length;
		index_data[ i] =  length; 
	}

	//we think of a grid which is the 
	//cartesian product like this:
	//*-*-*-*-* x *-*-*-*
	
	cells.resize( boost::make_transform_iterator( index_data.begin(), 
		      [](const std::size_t & i){ return 2*i-1; }), 
	   	      boost::make_transform_iterator( index_data.end(), 
		      [](const std::size_t & i){ return 2*i-1; }));
 
	for( auto i = ++(index_data.begin()); 
		  i != index_data.end(); ++i){ *i *= *(i-1); }
	std::size_t vertex_id_number=0;
	while( ctl::get_line(in, line, line_num)){
	     std::istringstream ss( line);
	     //and it's id
	     Data d( vertex_id_number);
	     insert_open_cell( Cell( 1, vertex_id_number), d);
	}
	return in;
   }
   void reserve( const std::size_t n) { cells.reserve( n); }
   const std::size_t dimension() const { return cells.dimension(); }
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

//Private functions
private:

  template< typename Coordinate>
  void vertex_id_to_position( std::size_t index, Coordinate & c){
    c.resize( index_data.size(), 0);
    for( auto i = index_data.size()-1; i>0;  --i){ 
        c[ i] = index/ index_data[ i-1];
        index -= c[ i]*(index_data[ i-1]);
	c[ i] = 2*c[ i] + cells.base( i);
    }
    c[ 0] = 2*index + cells.base( 0);
    return c;
  }

//Private members
private:
   Storage cells;
   Vector index_data;
   Cell_boundary bd;
}; //chain_complex
} //namespace ctl

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
	for(auto i = c.begin(); i != c.end(); ++i){
		      const std::size_t id = i->second.id();
		      out << id; 
		      out << ": ";
		      out << i->first << std::endl; 
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

} //namespace ctl

#endif //CTL_CHAIN_COMPLEX_MAP_H
