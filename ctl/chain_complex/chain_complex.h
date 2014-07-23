#ifndef CTLIB_CELL_MAP_H
#define CTLIB_CELL_MAP_H
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
/*********************
* April 5th, 2014
* Notes: presently we wrap the std::unordered_map a.k.a Map type.
* We fail to have perfect hashing, and we have collisions between
* cells of various different dimensions.
* A relatively easy optimization to explore is to  have something like
* vector< Map> where array element i stores cells of dimension i.
* Aside from certainly having less collisions,
* this has the benefit that the skeletal filtration will now be available with
* zero extra work after complex construction.
* We don't even need to worry about resizing time, since, copying a map is done
* via a swap of ~4 pointers, and in c++11 they are moves.
* Also this only happens at most O(log(d)) or something..
* the only thing to be careful of is the begin() and end() needs to be the
* original begin() and end() iterators concatenated together.
* This is somewhat frustrating because the naive implementation will require a
* 5x space bloat for each iterator, so less iterators fit on a cache line.
* A massive sort might have 5x more cache misses, for example.
* We would save space by forcing a single hash table to hash cells of different
* dimensions to different places, but, this seems overly optimistic.
* This also makes iterators
* invalid less often then they were before.
* In particular, iterators are only invalidated in one dimension at a time,
* AND rehashing will be marginally less expensive.
* begin()/end() would be overloaded to take a dimension as well.
* in particular this means that a filtration sorting could be implemented
* to avoid this craziness of pointers..  
**********************/

//STL
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>

//CTL
#include <ctl/hash/hash.h>
#include <ctl/io/io.h>
#include <ctl/abstract_simplex/abstract_simplex.h>

//forward declaration
namespace ctl{
template< typename Cell_, typename Boundary_,
	  typename Data_, typename Hash_>
class Chain_complex;
} //namespace ct

//non-exported functionality
namespace ctl {
namespace detail {

template< typename Data_>
class Data_wrapper : public Data_ {
   private:
   typedef Data_wrapper< Data_> Self;
   typedef std::size_t Id;
   public:
   //default
   Data_wrapper(): id_( 0) {}
   //copy
   Data_wrapper( const Id & tid):
   Data_(), id_( tid){}

   Data_wrapper( const Data_wrapper & from) : id_( from.id_){}
   //move
   Data_wrapper( const Data_wrapper && from):
   	id_( std::move( from.id_)){}

   Self& operator=( const Self & from){
   	Data_::operator=( from);
   	id_ = from.id_;
   	return *this;
   }

   Self& operator=( Self && from){
   	Data_::operator=( from);
   	id_ = std::move( from.id_);
   	return *this;
   }


   Id id() const { return id_; }
   void id( Id n){ id_ = n; }
   private:
   Id id_;
   //(to be read in Millhouse Van Houten's voice)
   //This lets the chain_complex & boundary touch my privates ;)
   template< typename C, typename B, typename D, typename H>
   friend class ctl::Chain_complex;
}; // class Data_wrapper

struct Default_data {}; //class Default_data for complex.

template< typename Stream>
Stream& operator<<( Stream & out, const Default_data & d){ 
	return out; 
}
template< typename Stream>
Stream& operator<<( Stream & out, const Default_data && d){ 
	return out; 
}
} //detail
} //ctl namespace


//exported functionality
namespace ctl{

/**
* @brief \class Chain_complex
* A structure which stores the standard cell basis for a Chain_complex
* Presently the internal data structure is a hash table. The keys are the
* cells themselves and the values are any data associated to a cell.
* We wrap the Data parameter transparently to associate a unique id to each cell.
* @tparam Cell
* @tparam Boundary
* @tparam Data
* @tparam Hash
*/
template< typename Cell_,
	  typename Boundary_,
	  typename Data_ = ctl::detail::Default_data,
	  typename Hash_ = ctl::Hash< Cell_> >
class Chain_complex{
public:
   typedef Cell_ Cell; //Describes a fundamental object,
   		    //e.g. simplex, cube, etc
   typedef Boundary_ Boundary; //Describes how to take its boundary
   //Arbitrary data associated to space.
   typedef ctl::detail::Data_wrapper< Data_> Data; 
   typedef Hash_ Hash;
private:
   typedef std::unordered_map< Cell, Data, Hash>  Map;
public:
   typedef typename Map::size_type size_type;
   typedef typename Map::iterator iterator;
   typedef typename Map::const_iterator const_iterator;
   typedef typename Map::value_type value_type;

public:
   //Constructors
   //Default
   Chain_complex(): max_id( 0), max_dim( 0) { cells.max_load_factor( 1); }
   Chain_complex( Boundary & bd_, const std::size_t num_cells=1): 
   cells( num_cells), bd( bd_), max_id( 0), max_dim( 0) {
	cells.max_load_factor( 1); 
   }

   //Copy
   Chain_complex( const Chain_complex & b): cells( b.cells), bd( b.bd),
   				 max_id( b.max_id), max_dim( b.max_dim)
   { cells.max_load_factor( 1); }
   //Move
   Chain_complex( Chain_complex && b): cells( std::move( b.cells)),
   			  bd( std::move( b.bd)),
   			  max_id( std::move(b.max_id)),
   			  max_dim( std::move( b.max_dim)) {}

   // assignment operator
   Chain_complex& operator=( const Chain_complex& b){
   	bd = b.bd;
   	max_id = b.max_id;
   	max_dim = b.max_dim;
   	cells = b.cells;
   	return *this;
   }

   // move assignment operator
   Chain_complex& operator=( Chain_complex&& b){
   	bd      = std::move( b.bd);
   	max_id  = std::move( b.max_id);
   	max_dim = std::move( b.max_dim);
   	cells   = std::move( b.cells);
   	return *this;
   }

   iterator       find_cell( const Cell & s)       { return cells.find( s); }
   const_iterator find_cell( const Cell & s) const { return cells.find( s); }

   iterator       begin()       { return cells.begin(); }
   iterator         end()       { return cells.end();   }

   const_iterator begin() const { return cells.begin(); }
   const_iterator   end() const { return cells.end();   }

   std::pair< iterator, bool> insert_open_cell( const Cell & s,
   					     const Data& data=Data()){
     std::pair< iterator, bool> c =  cells.emplace( s, data);
     if( c.second) { //this outer if is probably unnecessary
       max_dim = std::max( max_dim, s.dimension());
       if( c.first->second.id_ == 0){
        c.first->second.id_ = ++max_id;
       } else{
        max_id=std::max( max_id, c.first->second.id_);
       }
     }
     return c;
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
 

	ctl::identity i;
	return write( out, i);
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
   Boundary& boundary() { return bd; }
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
private:
   Map cells;
   Boundary bd;
   std::size_t max_id;
   std::size_t max_dim;
}; //chain_complex


template< typename Stream, typename Cell, typename Boundary, 
	   typename Data, typename Hash>
Stream& operator<<( Stream& out, 
		    const Chain_complex< Cell, Boundary, Data, Hash> & c){ 
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
		    const Chain_complex< Cell, Boundary, Data, Hash> && c){ 
	out << c;
	return out;
}

} //namespace ctl

namespace ctl{

template< typename Stream, typename Cell, 
	  typename Boundary, typename Data_, typename Hash>
Stream& operator>>( Stream& in, 
		    Chain_complex< Cell, Boundary, Data_, Hash> & c){  return c.read( in); }

template<typename String, typename Complex, typename Functor>
void read_complex_and_data(String & complex_name, String & data_file, 
			   Complex & complex, Functor & f){
	std::ifstream in;
	std::cout << "File IO ..." << std::flush;
	//first read the complex in
	ctl::open_file( in, complex_name.c_str());
	complex.read( in);
	ctl::close_file( in);
	//then read the data file in, e.g. weights, cover, etc..
	ctl::open_file( in, data_file.c_str());
	complex.read_data( in, f);	
	ctl::close_file( in);
	std::cout << "completed!" << std::endl;
}

template<typename String, typename Complex>
void read_complex(String & complex_name, Complex & complex){
	std::ifstream in;
	std::cout << "File IO ..." << std::flush;
	ctl::open_file( in, complex_name.c_str());
	complex.read( in);
	ctl::close_file( in);
	std::cout << "completed!" << std::endl;
}



} //namespace ctl

#endif //CTL_CHAIN_COMPLEX_MAP_H
