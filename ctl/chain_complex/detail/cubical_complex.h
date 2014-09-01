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
#include <ctl/cube/cube.h>
#include <numeric>
#include <sstream>
#include <fstream>

namespace ctl {
namespace detail {

template< typename Cell_,
	  typename Boundary_,
	  typename Data_,
	  typename Hash_> 
class Cubical_complex{
public: //Public types
   typedef Cell_ Cell; //Describes a fundamental object,
		       //e.g. Simplex, Cube, Polygon, etc..

   typedef Boundary_ Cell_boundary; //Describes how to take Cell boundary
   //Arbitrary data associated to space.
   typedef Data_ Data;
   typedef Hash_ Hash;

private: //Private types
   typedef std::vector< Data>  Map;
   typedef std::vector< std::size_t>  Vector;
    
public: //Public Types
   typedef typename Map::size_type size_type;
   typedef typename Map::iterator iterator;
   typedef typename Map::const_iterator const_iterator;
   typedef typename std::pair< Cell, Data> value_type;

public:
   //Constructors
   //Default
   Cubical_complex(): max_id( 0), max_dim( 0) {}

   Cubical_complex( Cell_boundary & bd_, const std::size_t num_cells): 
   cells( num_cells), bd( bd_), max_id( 0), max_dim( 0) {}

   //TODO: Expand complex structure to store cells of a fixed dimension
   //in different containers. glue together objects with a crazy iterator
   template< typename Size_by_dimension>
   Cubical_complex( Cell_boundary & bd_, const Size_by_dimension d): 
    cells( std::accumulate( d.begin(), d.end(), 0)),
    bd( bd_), max_id( 0), max_dim( 0) {
   }

   //Copy
   Cubical_complex( const Cubical_complex & b): cells( b.cells), bd( b.bd),
   				 max_id( b.max_id), max_dim( b.max_dim) {}

   //Move
   Cubical_complex( Cubical_complex && b): cells( std::move( b.cells)),
   			  bd( std::move( b.bd)),
   			  max_id( std::move(b.max_id)),
   			  max_dim( std::move( b.max_dim)) {}

   //Assignment operator
   Cubical_complex& operator=( const Cubical_complex& b){
   	bd = b.bd;
   	max_id = b.max_id;
   	max_dim = b.max_dim;
   	cells = b.cells;
   	return *this;
   }

   //Move assignment operator
   Cubical_complex& operator=( Cubical_complex&& b){
   	bd      = std::move( b.bd);
   	max_id  = std::move( b.max_id);
   	max_dim = std::move( b.max_dim);
   	cells   = std::move( b.cells);
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

   std::pair< iterator, bool> insert_open_cell( const Cell & s,
   					        const Data& data){
     std::size_t linear_index = cell_to_word( s);
     if( cells.size() <= linear_index) { cells.resize( linear_index); }
     bool inserted_now = (cells[ linear_index].id_ == 0);
     if( inserted_now){ 
        cells[ linear_index] = data;  
        max_dim = std::max( max_dim, s.dimension());
        if( data.id_ == 0){ cells[ linear_index].id_ = ++max_id; } 
        else{ max_id=std::max( max_id, cells[ linear_index].id_); }
     }
     return std::make_pair( begin()+linear_index, inserted_now);
   }

   std::pair< iterator, std::size_t>
   insert_closed_cell( const Cell & s, 
		       const bool closed=false,
   		       const Data&  data = Data()){
   	typedef typename std::pair< iterator, std::size_t> Pair;
   	iterator iter = cells.find( s);
   	std::size_t num_faces_inserted=0;
   	//if cell exists, and we assume
   	//we are closed then we are done.
   	if( closed && iter != cells.end()){
   	 return std::make_pair( iter, num_faces_inserted);
   	}

   	//first you add the boundary
   	Data face_data( data);
   	face_data.id_ = 0;
   	for( auto face = bd.begin( s); face != bd.end( s); ++face){
   	 const Pair & p = insert_closed_cell( face->cell(),
   					      closed, face_data);
   	 num_faces_inserted+=p.second;
   	}

  	//ugly, but then you add yourself.
   	const std::pair< iterator, bool> p(insert_open_cell( s, data));
   	return std::make_pair( p.first, p.second+num_faces_inserted);
   }

   template< typename Stream, typename Functor>
   Stream& write( Stream& out, const Functor & f) const {
   	out << "size " << cells.size() << std::endl;
   	for( const auto & cell: cells){
   	  out << cell.first.size() << " " << std::flush;
	  cell.first.write( out);
	  out << std::flush;
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
	}
	//Read the header and reserve appropriately
        ctl::get_line( in, line, line_num);
        std::istringstream ss( line);
        std::string the_word_size;
        ss >> the_word_size;
        std::size_t the_number_of_cells;
        ss >> the_number_of_cells;
	reserve( the_number_of_cells);
	std::size_t size;
	while( ctl::get_line(in, line, line_num)){
	     std::istringstream ss( line);
	     Cell cell;
	     //header enabled so read size first
	     ss >> size;
	     //then get the cell
	     cell.read( ss, size);
	     //and it's id
	     ss >> id;
	     Data d( id);
	     insert_open_cell( cell, d);
	}
	return in;
   }
   template< typename Stream, typename Functor>
   Stream& read_data( Stream & in, Functor & f){
	typedef typename Data::Id Id;
	typedef typename Functor::result_type Value;
	typedef std::unordered_map< Id, Value> Value_map; 
	std::string line;
	std::size_t line_num = 0;
	Value_map values( size());
	Id id;
	Value value;
	while( ctl::get_line( in, line, line_num)){
		std::istringstream ss( line);
		ss >> id;
		ss >> value;
		values[ id] = value;
	}
	for (auto & sigma: cells) { 
		f( sigma.second) = values[ sigma.second.id()]; 
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
/*
int64_t tuple_to_index( const std::vector< int64_t >& tuple ) const
{
    int64_t index = tuple[ get_max_dim() - 1 ];
    for( int64_t cur_dim = get_max_dim() - 2; cur_dim >= 0; cur_dim-- ) {
        index *= cubical_complex_resolution[ cur_dim ];
        index += tuple[ cur_dim ];
    }
    return index;
}

int64_t vertex_tuple_to_lattice_index( const std::vector< int64_t >& tuple ) const
{
    int64_t index = tuple[ get_max_dim() - 1 ] / 2;
    for( int64_t cur_dim = get_max_dim() - 2; cur_dim >= 0; cur_dim-- ) {
        index *= lattice_resolution[ cur_dim ];
        index += tuple[ cur_dim ] / 2;
    }
    return index;
}

void index_to_tuple( int64_t idx, std::vector< int64_t >& tuple ) const
{
    tuple.resize( get_max_dim() );
    for( int64_t cur_dim = get_max_dim() - 1; cur_dim > 0; cur_dim-- ) {
        tuple[ cur_dim ] = idx / cum_resolution_product[ cur_dim - 1 ];
        idx -= tuple[ cur_dim ] * cum_resolution_product[ cur_dim - 1 ];
    }
    tuple[ 0 ] = idx;
}
*/
std::size_t cell_to_word( const Cell & cell) const {
	std::size_t index = 0;
	//TODO: implement this.
	return index;
}
void word_to_cell( const std::size_t word, Cell & cell){
	//TODO: implement this.
}
//Private members
private:
   Map cells;
  
   Cell_boundary bd;
   std::size_t max_id;
   std::size_t max_dim;
}; //chain_complex
} //namespace ctl


template< typename Stream, typename C, typename B, 
	   typename D, typename H>
Stream& operator<<( Stream& out, 
		    const typename ctl::detail::Cubical_complex< C, B, D, H>::
					iterator c){ 
	out << c->first;
	return out;	
}


template< typename Stream, typename C, typename B, 
	   typename D, typename H>
Stream& operator<<( Stream& out, 
		    const typename ctl::detail::Cubical_complex< C, B, D, H>::
					const_iterator c){ 
	out << c->first;
	return out;	
}

template< typename Stream, typename Cell, typename Boundary, 
	   typename Data, typename Hash>
Stream& operator<<( Stream& out, 
		    const ctl::detail::Cubical_complex< Cell, 
							Boundary, 
							Data, 
							Hash> & c){ 
	for(auto i = c.begin(); i != c.end(); ++i){
		      const std::size_t id = i->second.id();
		      out << id; 
		      out << ": ";
		      out << i->first << std::endl; 
	}
	return out;
}

template< typename Stream, typename Cell, typename Boundary, 
	   typename Data, typename Hash>
Stream& operator<<( Stream& out, 
   const ctl::detail::Cubical_complex< Cell, Boundary, Data, Hash> && c){ 
	out << c;
	return out;
}

template< typename Stream, typename Cell, 
	  typename Boundary, typename Data, typename Hash>
Stream& operator>>( Stream& in, 
		    ctl::detail::Cubical_complex< Cell, 
						  Boundary, 
						  Data, 
						  Hash> & c){  
return c.read( in); 
}

} //namespace ctl

#endif //CTL_CHAIN_COMPLEX_MAP_H
