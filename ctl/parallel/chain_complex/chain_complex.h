#ifndef CTLIB_CONCURRENT_CELL_MAP_H
#define CTLIB_CONCURRENT_CELL_MAP_H
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
*******************************************************************************
* ********** BSD-3 License ****************
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software without 
* specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
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
* A massive sort might have 5x more cache misses, for example, and
* the chains we store for persistence will have more stuff in them.
* We would save space by forcing a single hash table to hash cells of different
* dimensions to different places.
* This also makes iterators
* invalid less often then they were before.
* In particular, iterators are only invalidated in one dimension at a time,
* AND rehashing will be marginally less expensive.
* begin()/end() would be overloaded to take a dimension as well.
**********************/
//STL
#include <unordered_map>
#include <vector>
#include <sstream>

//CTL
#include <ctl/hash/hash.h>
#include <ctl/io/io.h>
#include <ctl/abstract_simplex/abstract_simplex.h>

#include <tbb/concurrent_unordered_map.h>
#include <tbb/atomic.h>

//forward declaration
namespace ctl{
namespace parallel{
struct Default_data {}; //class Default_data for complex.
template< typename Cell_, typename Boundary_,
	  typename Data_, typename Hash_>
class Chain_complex;
} //namespace parallel
} //namespace ct

//non-exported functionality
namespace {

template< typename Data_>
class Concurrent_data_wrapper : public Data_ {
   private:
   typedef Concurrent_data_wrapper< Data_> Self;
   public:
   typedef std::size_t Unsafe_id;
   typedef tbb::atomic< Unsafe_id> Safe_id;
   //default
   Concurrent_data_wrapper(): id_() {}
   
   Concurrent_data_wrapper( const std::size_t tid):
   Data_(), id_() { id_ = tid;}

   //copy
   Concurrent_data_wrapper( const Concurrent_data_wrapper & from) :
     id_( from.id_){}
   //move
   Concurrent_data_wrapper( const Concurrent_data_wrapper && from):
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


   Safe_id id() const { return id_; }
   //a bit akward.. probably should change this later.
   void id( const std::size_t p) { id_= p; }
   private:
   Safe_id id_;
   //(to be read in Millhouse Van Houten's voice)
   //This lets the chain_complex & boundary touch my privates ;)
   template< typename C, typename B, typename D, typename H>
   friend class ctl::parallel::Chain_complex;
}; // class Concurrent_data_wrapper
} //anonymous namespace


template< typename Stream>
Stream& operator<<( Stream & out, const ctl::parallel::Default_data & d){ 
	return out; 
}

//exported functionality
namespace ctl{
namespace parallel{
template< typename Cell_,
	  typename Boundary_,
	  typename Data_ = ctl::parallel::Default_data,
	  typename Hash_ = ctl::Hash< Cell_> >
class Chain_complex{
public:
   typedef Cell_ Cell; //Describes a fundamental object,
   		    //e.g. simplex, cube, etc
   typedef Boundary_ Cell_boundary; //Describes how to take its boundary
   //Arbitrary data associated to space.
   typedef Concurrent_data_wrapper< Data_> Data; 
   typedef Hash_ Hash;
private:
   typedef tbb::concurrent_unordered_map< Cell, Data, Hash>  Map;
public:
   typedef typename Map::size_type size_type;
   typedef typename Map::iterator iterator;
   typedef typename Map::const_iterator const_iterator;
   typedef typename Map::value_type value_type;

public:
   //Constructors
   //Default
   Chain_complex(): max_id(), max_dim() { 
	cells.max_load_factor( 1); 
   }
   Chain_complex( Cell_boundary & bd_, const std::size_t num_cells=1): 
   cells( num_cells), bd( bd_), max_id(), max_dim() {
	cells.max_load_factor( 1); 
   }
   //Copy
   Chain_complex( const Chain_complex & b): 
   cells( b.cells), bd( b.bd), max_id( b.max_id), max_dim( b.max_dim) { 
	cells.max_load_factor( 1); 
   }
   //Move
   Chain_complex( Chain_complex && b): 
			  cells( std::move( b.cells)),
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
     std::pair< iterator, bool> c =  cells.insert( std::make_pair( s, data));
     if( c.second) { 
	const std::size_t old_max_dim = max_dim.load();
	if( s.dimension() > old_max_dim) { 
 		max_dim.compare_and_swap( s.dimension(), old_max_dim);
        } 
	if( c.first->second.id_ == 0){
         c.first->second.id_ = max_id.fetch_and_add( 1);
        } else {
         const std::size_t old_max_id = max_id.load();
         if( c.first->second.id_ > old_max_id) {
         	max_id.compare_and_swap( c.first->second.id_, old_max_id);
         }
        }
     }
     return c;
   }

   std::pair< iterator, std::size_t>
   insert_closed_cell( const Cell & s, const bool closed=false,
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
   void write( Stream& out, const Functor & f) const {
   	out << "size " << cells.size() << std::endl;
   	for( auto cell: cells){
   	  out << f( cell.second) << " ";
   	  cell.first.write( out);
   	  out << std::endl;
   	}
   }
  
   template< typename Stream>
   void write( Stream& out) const {
   	out << "size " << cells.size() << std::endl;
   	for( auto cell: cells){
   	  out << cell.second.id() << " ";
   	  cell.first.write( out);
   	  out << std::endl;
   	}
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
private:
   Map cells;
   Cell_boundary bd;
   typename Data::Safe_id max_id;
   typename Data::Safe_id max_dim;
}; //cell_map
} //namespace parallel
} //namespace ctl

#endif //CTL_CONCURRENT_CHAIN_COMPLEX_MAP_H
