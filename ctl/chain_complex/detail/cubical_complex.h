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
   Cubical_complex():  max_dim( 0) {}

   template< typename Range>
   Cubical_complex( Cell_boundary & bd_, Range r1, Range r2, Range r3):
    cells(), bd( bd_), max_dim( d_.size()), dimensions( d_), 
    offsets( d_.size(), 0) {
	Vector d( d_);
	for( auto & i: d) { i = 2*i-1; }
 	cells.resize( d);
    }

   Cubical_complex( Cell_boundary & bd_, 
		    const Vector& d_,
		    const Vector& offsets_): 
    cells( d_), bd( bd_), max_dim( d_.size()), 
    dimensions( d_), offsets( offsets_) {
    cells.reserve( std::accumulate( d_.begin(), d_.end(), 0));
   }

   //Copy
   Cubical_complex( const Cubical_complex & b): cells( b.cells), bd( b.bd),
   		    max_dim( b.max_dim), 
		    dimensions( b.dimensions),
		    offsets( b.offsets) {}

   //Move
   Cubical_complex( Cubical_complex && b): cells( std::move( b.cells)),
   			  bd( std::move( b.bd)),
   			  max_dim( std::move( b.max_dim)), 
			  dimensions( std::move( b.dimensions)), 
			  offsets( std::move( b.offsets)) {}

   //Assignment operator
   Cubical_complex& operator=( const Cubical_complex& b){
   	bd = b.bd;
   	max_dim = b.max_dim;
   	cells = b.cells;
	dimensions = b.dimensions;
        offsets = b.offsets;
   	return *this;
   }

   //Move assignment operator
   Cubical_complex& operator=( Cubical_complex&& b){
   	bd      = std::move( b.bd);
   	max_dim = std::move( b.max_dim);
   	cells   = std::move( b.cells);
	dimensions = std::move( b.dimensions);
        offsets = std::move( b.offsets);
   	return *this;
   }

   iterator       find_cell( const Cell & s) {
	std::size_t linear_index = cell_to_word( s);
        if( cells.size() <= linear_index) { return cells.end(); }
	return cells.begin()+linear_index;
   }

   const_iterator find_cell( const Cell & s) const { 
	std::size_t linear_index = cell_to_word( s);
        if( cells.size() <= linear_index) { return cells.end(); }
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
	std::size_t id=0;
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
        ss >> the_word_size;
        ss >> max_dim; 
	//keep track of the number of dimensions.
	dimensions.resize( max_dim);
	std::size_t length;
	for( std::size_t i = 0; i < max_dim; ++i){
		ss >> length;
		//we think of a grid which is the 
		//cartesian product like this:
		//*-*-*-*-* x *-*-*-*
		dimensions[ i] =  2*length-1;
	}
	//we create an array with this number of entries
	cells.resize( dimensions);
	std::size_t vertex_id_number=0;
	while( ctl::get_line(in, line, line_num)){
	     std::istringstream ss( line);
	     //and it's id
	     ss >> id;
	     Data d( id);
	     //TODO: vertex_id_number --> vertex_position
	     //insert_open_cell( cell, d);
	     ++vertex_id_number;
	}
	return in;
   }
   void reserve( const std::size_t n) { cells.reserve( n); }
   const std::size_t dimension() const { return max_dim; }
   const std::size_t size() const { return cells.size(); }
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

//Private members
private:
   Storage cells;
   Vector dimensions;
   Vector offsets;
   Cell_boundary bd;
   std::size_t max_dim;
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
