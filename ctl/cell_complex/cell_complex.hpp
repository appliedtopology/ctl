#ifndef CTL_CELL_COMPLEX_H
#define CTL_CELL_COMPLEX_H
/*******************************************************************************
* BSD-3
*******************************************************************************/
/*********************
* April 5th, 2014
* Notes: presently we wrap the std::unordered_map a.k.a Storage type.
* We fail to have perfect hashing, and we have collisions between
* cells of various different dimensions.
* A relatively easy optimization to explore is to  have something like
* vector< Storage> where array element i stores cells of dimension i.
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
#include <numeric>
#include <atomic>

//CTL
#include <ctl/abstract_simplex/simplex_boundary.hpp>
#include <ctl/cube/cube_boundary.hpp>
#include <ctl/product_cell/product_boundary.hpp>
#include <ctl/cell_complex/detail/data_wrapper.hpp>
#include <ctl/hash/hash.hpp>

//BOOST
#include <boost/serialization/base_object.hpp>

//TBB
#include <tbb/concurrent_unordered_map.h>

namespace ctl {

namespace detail {
template<typename T>
void update_maximum(T& maximum_value, const T & value) noexcept
{
  if(value > maximum_value) maximum_value = value;
}


template<typename T>
void update_maximum(std::atomic<T>& maximum_value, T const& value) noexcept
{
    T prev_value = maximum_value;
    while(prev_value < value &&
            !maximum_value.compare_exchange_weak(prev_value, value))
        ;
}
/***
 * This is a hack until we remove TBB dependency because not all versions of tbb support emplace(_)
 * We will move to use std::experimental where they hopefully have concurrent containers by now
 */
template< typename Cell, typename Data, typename Hash>
decltype(auto) emplace_cell( tbb::concurrent_unordered_map< Cell, Data, Hash>& cells, const Cell& cell, const Data& d){ return cells.insert( std::make_pair( cell, d)); }
template< typename Cell, typename Data, typename Hash>
decltype(auto) emplace_cell( std::unordered_map< Cell, Data, Hash>& cells, const Cell& cell, const Data& d){ return cells.emplace( cell, d); }

template< typename C, 
	  typename D,
	  typename H,
	  bool is_concurrent
	>
//meta code for the Chain_complex type.
//This metacode creates a type, Chain_complex< C, B, D, H, S>
using Storage = typename std::conditional< is_concurrent, 
	tbb::concurrent_unordered_map<C,D,H>, 
	std::unordered_map< C,D,H>
	>::type; 
} //end namespace detail

template< typename Boundary_,
	  typename Data_ = detail::Default_data,
	  typename Hash_ = ctl::Hash< typename Boundary_::Cell>,
	  bool Concurrent= false> 
class Cell_complex{
public:
   typedef Boundary_ Cell_boundary; //Describes how to take Cell boundary
   //Arbitrary data associated to space.
   typedef detail::Data_wrapper< Data_> Data;
   typedef Hash_ Hash;
   typedef typename Cell_boundary::Cell Cell; //e.g. Simplex, Cube, Polygon, etc..
private:
   typedef detail::Storage<Cell, Data, Hash, Concurrent> Storage;
   
public:
   typedef typename Storage::size_type size_type;
   typedef typename Storage::iterator iterator;
   typedef typename Storage::const_iterator const_iterator;
   typedef typename Storage::value_type value_type;

public:
   //Constructors
   //Default
   //template<typename std::enable_if< std::is_default_constructible<Cell_boundary>::value , int>::type = 0>
   Cell_complex(): max_id( 0), max_dim( 0) { 
	cells.max_load_factor( 1); 
   }
   template< typename Iterator>
   //template< typename Iterator, typename std::enable_if< std::is_default_constructible<Cell_boundary>::value , int>::type = 0>
   Cell_complex( Iterator begin, Iterator end, bool construct_closed=true): cells(), max_id(0), max_dim( 0){
      if( construct_closed){
        for(Iterator i = begin; i != end; ++i){
          insert_closed_cell( *i);
        }
      } else {
        for(Iterator i = begin; i != end; ++i){
          insert_open_cell( *i);
        }
      }
   }

   Cell_complex( Cell_boundary & bd_, const std::size_t num_cells): 
   cells( num_cells), bd( bd_), max_id( 0), max_dim( 0) {
	cells.max_load_factor( 1); 
   }

   //TODO: Expand complex structure to store cells of a fixed dimension
   //in different containers. glue together objects with a crazy iterator
   template< typename Size_by_dimension>
   Cell_complex( Cell_boundary & bd_, const Size_by_dimension d): 
    cells( std::accumulate( d.begin(), d.end(), 0)),
    bd( bd_), max_id( 0), max_dim( 0) {
	cells.max_load_factor( 1); 
   }

   //Copy
   Cell_complex( const Cell_complex & b): 
   cells( b.cells), bd( b.bd), max_id( b.max_id), max_dim( b.max_dim)
   { cells.max_load_factor( 1); }

   //Move
   Cell_complex( Cell_complex && b): 
   cells( std::move( b.cells)), bd( std::move( b.bd)),
   max_id( std::move(b.max_id)), max_dim( std::move( b.max_dim)) {}

   // assignment operator
   Cell_complex& operator=( const Cell_complex& b){
   	bd = b.bd;
   	max_id = b.max_id;
   	max_dim = b.max_dim;
   	cells = b.cells;
   	return *this;
   }

   // move assignment operator
   Cell_complex& operator=( Cell_complex&& b){
   	bd      = std::move( b.bd);
   	max_id  = std::move( b.max_id);
   	max_dim = std::move( b.max_dim);
   	cells   = std::move( b.cells);
   	return *this;
   }

   // move assignment operator
   bool operator==( const Cell_complex& b) const {
        return (b.max_dim == max_dim) && (cells == b.cells);
   }
  // move assignment operator
   bool operator!=( const Cell_complex& b) const {
   	return !(b == *this);
   }


   iterator       find_cell( const Cell & s)       { return cells.find( s); }
   const_iterator find_cell( const Cell & s) const { return cells.find( s); }

   iterator       begin()       { return cells.begin(); }
   iterator         end()       { return cells.end();   }

   const_iterator begin() const { return cells.begin(); }
   const_iterator   end() const { return cells.end();   }

   std::pair< iterator, bool> insert_open_cell( const Cell & s,
   					     const Data& data=Data()){
     std::pair< iterator, bool> c =  emplace_cell( cells, s, data);
     if( c.second) { //this outer if is probably unnecessary
       detail::update_maximum( max_dim, s.dimension());
       if( c.first->second.id() == 0){
        c.first->second.id( ++max_id) ;
       } else{
        max_id=std::max( max_id, c.first->second.id());
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
   	face_data.id(  0);
   	for( auto face = bd.begin( s); face != bd.end( s); ++face){
   	 const Pair & p = insert_closed_cell( face->cell(),
   					      closed, face_data);
   	 num_faces_inserted+=p.second;
   	}

  	//ugly, but then you add yourself.
   	const std::pair< iterator, bool> p(insert_open_cell( s, data));
   	return std::make_pair( p.first, p.second+num_faces_inserted);
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
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const std::size_t version){
     ar & cells;
     ar & max_id;
     ar & max_dim;
  }

   Storage cells;
   Cell_boundary bd;
   std::size_t max_id;
   std::size_t max_dim;
}; //end class Cell_complex

template< typename Stream, 
          typename Boundary_,
          typename Data_,
          typename Hash_>
Stream& operator<<( Stream& out, 
   const typename ctl::Cell_complex< Boundary_, Data_, Hash_>::iterator c){ 
	out << c->first;
	return out;	
}

template< typename Stream, 
          typename Boundary_,
          typename Data_,
          typename Hash_> 
Stream& operator<<( Stream& out, 
   const typename ctl::Cell_complex< Boundary_, Data_, Hash_>::const_iterator c){ 
	out << c->first;
	return out;	
}

template< typename Stream, 
          typename Boundary_,
          typename Data_,
          typename Hash_,
	  bool Concurrent> 
Stream& operator<<( Stream& out, 
   const ctl::Cell_complex< Boundary_, Data_, Hash_, Concurrent> & c){ 
	for(auto i = c.begin(); i != c.end(); ++i){
		out << i->second.id() << ": " << i->first << std::endl; 
	}
	return out;
}

template< typename Stream, 
          typename Boundary_,
          typename Data_,
          typename Hash_,
	  bool Concurrent> 
Stream& operator<<( Stream& out, 
		    const ctl::Cell_complex< Boundary_, Data_, Hash_, Concurrent>&& c){
	out << c;
	return out;
}

template< typename D=ctl::detail::Default_data>
using Simplicial_complex = ctl::Cell_complex< ctl::Simplex_boundary, D>;

template< typename D=ctl::detail::Default_data>
using Cubical_complex = ctl::Cell_complex< ctl::Cube_boundary, D>;
template< typename Bd1, typename Bd2>
using Product_complex = ctl::Cell_complex< ctl::Product_boundary< Bd1, Bd2>, 
      					   ctl::detail::Default_data, 
					   typename ctl::Product_boundary< Bd1, Bd2>::Hash >;
using Prod_simplicial_complex = Product_complex< ctl::Simplex_boundary, ctl::Simplex_boundary>;
using Cube_simplicial_product_complex = Product_complex< ctl::Cube_boundary, ctl::Simplex_boundary>;

} //namespace ctl

#endif //CTL_CELL_COMPLEX_H
