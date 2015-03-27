#ifndef CTL_SIMPLICIAL_CHAIN_COMPLEX_H
#define CTL_SIMPLICIAL_CHAIN_COMPLEX_H
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
* Released under BSD. See LICENSE
*******************************************************************************/

//STL
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>

//CTL
#include <ctl/utility/flattening_iterator.hpp>

namespace ctl {
namespace detail{

template< typename Cell_, typename Boundary_,
	  typename Data_, typename Hash_> 
class Simplicial_chain_complex{
public:
   typedef Cell_ Cell; //Describes a fundamental object,
		       //e.g. Simplex, Cube, Polygon, etc..

   typedef Boundary_ Cell_boundary; //Describes how to take Cell boundary
   //Arbitrary data associated to space.
   typedef Data_ Data;
   typedef Hash_ Hash;

private:
   typedef std::unordered_map< Cell, Data, Hash> Map;
   typedef std::vector< Map> Storage;
   typedef typename Storage::iterator outer_iterator;
   typedef typename Storage::const_iterator const_outer_iterator;
   
public:
   typedef typename Storage::size_type size_type;
   typedef typename Map::value_type value_type;
   typedef typename ctl::detail::flattening_iterator< outer_iterator> iterator;
   typedef typename ctl::detail::const_flattening_iterator< const_outer_iterator> const_iterator;

public:

    
   /**
   * @brief Default constructor
   */
   Simplicial_chain_complex(): max_id( 0), max_dim( 0) {}


   /**
   * @brief Initialized Cell_boundary constructor
   * If the second parameter is given the complex will present with 
   * dimension = sizes.size()-1 and space for dimension k cells will
   * be reserved up to the amount specified
   * @param Cell_boundary boundary
   * @param Size_by_dimension sizes (e.g. std::vector< int> sizes = {3, 3, 1})
   */
   template< typename Size_by_dimension>
   Simplicial_chain_complex( Cell_boundary & bd_, 
			     const Size_by_dimension sizes=Size_by_dimension()): 
    cells( sizes.size()),
    bd( bd_), max_id( 0), max_dim( 0) {
	cells.max_load_factor( 1);
	for( auto i = 0; i < sizes.size(); ++i){cells[ i].reserve( sizes[ i]);} 
   }

   
   /**
   * @brief Copy constructor
   */
   Simplicial_chain_complex( const Simplicial_chain_complex & b): 
   cells( b.cells), bd( b.bd), max_id( b.max_id), max_dim( b.max_dim)
   { cells.max_load_factor( 1); }

   /**
   * @brief Move constructor
   */
   Simplicial_chain_complex( Simplicial_chain_complex && b): 
   cells( std::move( b.cells)), bd( std::move( b.bd)),
   max_id( std::move(b.max_id)), max_dim( std::move( b.max_dim)) {}


   /**
   * @brief Assignment operator 
   */
   Simplicial_chain_complex& operator=( const Simplicial_chain_complex& b){
   	bd = b.bd;
   	max_id = b.max_id;
   	max_dim = b.max_dim;
   	cells = b.cells;
   	return *this;
   }

   /**
   * @brief Move assignment operator 
   */
   Simplicial_chain_complex& operator=( Simplicial_chain_complex&& b){
   	bd      = std::move( b.bd);
   	max_id  = std::move( b.max_id);
   	max_dim = std::move( b.max_dim);
   	cells   = std::move( b.cells);
   	return *this;
   }

   /**
   * @brief Searches for a Cell s and returns an iterator to it.
   * @param Cell s 
   * @return [const_]iterator
   */
   iterator       find_cell( const Cell & s)       { return iterator( cells[ s.dimension()].find( s), cells.begin(), cells.end()); }
   const_iterator find_cell( const Cell & s) const { return const_iterator( cells[ s.dimension()].find( s), cells.begin(), cells.end()); }

   iterator       begin()       { return iterator( cells.begin(), cells.end()); }
   iterator         end()       { return iterator( cells.end(), cells.end());   }

   const_iterator begin() const { return const_iterator( cells.begin(), cells.end()); }
   const_iterator   end() const { return const_iterator( cells.end(), cells.end());   }

   std::pair< iterator, bool> insert_open_cell( const Cell & s,
   					     const Data& data=Data()){
     if( cells.size() < s.size()){ cells.resize( s.size()); }
     auto c =  cells[ s.dimension()].emplace( s, data);
     if( c.second) { //this outer if is probably unnecessary
       max_dim = std::max( max_dim, s.dimension());
       if( c.first->second.id() == 0){
        c.first->second.id( ++max_id);
       } else{
        max_id=std::max( max_id, c.first->second.id());
       }
     }
     return std::make_pair( iterator(c.first, cells.begin(), cells.end()), c.second);
   }
   private:
   std::pair< iterator, std::size_t>
   insert_closed_cell_( const Cell & s, 
		       const bool closed=false,
   		       const Data&  data = Data()){
   	typedef typename std::pair< iterator, std::size_t> Pair;
   	iterator iter = find_cell( s);
   	std::size_t num_faces_inserted=0;
   	//if cell exists, and we assume
   	//we are closed then we are done.
   	if( closed && iter != end()){
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

  	//then you add yourself.
   	std::pair< iterator, bool> p = insert_open_cell( s, data);
	p.second += num_faces_inserted;
	return p;
   }

   unsigned nChoosek( unsigned n, unsigned k )
   {
       if (k > n) return 0;
       if (k * 2 > n){ k = n-k ; }
       if (k == 0) return 1;
   
       int result = n;
       for( int i = 2; i <= k; ++i ) {
           result *= (n-i+1);
           result /= i;
       }
       return result;
   }

   public:
   std::pair< iterator, std::size_t>
   insert_closed_cell( const Cell & s, 
		       const bool closed=false,
   		       const Data&  data = Data()){
	//before we begin we resize the outer table
	if( cells.size() < s.size()){ cells.resize( s.size()); }
	//and reserve all the necessary new space
	for( auto i = 0; i < s.size(); ++i){
		auto& map = cells[ i];
		map.reserve( map.size() + nChoosek( s.size(), i+1));
	}
	return insert_closed_cell_( s, closed, data);
   }
   template< typename Stream, typename Functor>
   Stream& write( Stream& out, const Functor & f) const {
   	out << "size " << cells.size() << std::endl;
   	for( auto & cell: *this){
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
   	for( auto & cell: *this){
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
	std::vector< std::size_t> the_number_of_cells;
	std::size_t cells_in_dimension_i;
	std::size_t i = 0;
	do{
        ss >> cells_in_dimension_i;
        reserve( i, cells_in_dimension_i);
	++i;
	}while( ss.good());

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
	for (auto & sigma: *this) { 
		f( sigma.second) = values[ sigma.second.id()]; 
	}
	return in;
   }
   
  /**
  * @brief Reserves memory for n cells in dimension d
  *
  * @param d
  * @param n
  */
   void reserve( const std::size_t d, const std::size_t n) { 
	cells[ d].reserve(n); 
   }
   
  /**
  * @brief Returns the dimension of the simplicial complex
  *
  * @return the dimension of the complex 
  */
   const std::size_t dimension() const { return (cells.size() > 0)? cells.size()-1:0; }

  /**
  * @brief Returns the size of the simplicial complex
  * @return std::size_t holding the number of simplices 
  */
   std::size_t size() const { 
	std::size_t size=0;
	for( auto& i : cells){ size+= i.size(); }
	std::cerr << size << std::endl;
	return size; 
   }

  /**
  * @brief Returns the number of d simplices of the simplicial complex
  * @return std::size_t holding the number of simplices 
  */
   const std::size_t size( std::size_t d) const { return cells[ d].size(); }

   /**
   * @brief Returns a reference to the underlying cell boundary
   * @return Cell_boundary 
   */
   Cell_boundary& cell_boundary() { return bd; }
 
   /**
   * @brief Checks if the complex is closed under the face relation
   * @return true if closed false otherwise 
   */
   bool is_closed() const{
    for( auto sigma : *this){
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
   Storage cells;
   Cell_boundary bd;
   std::size_t max_id;
   std::size_t max_dim;
}; //end class Simplicial_chain_complex

template< typename Cell_,
          typename Boundary_,
          typename Data_,
          typename Hash_> 
std::ostream& operator<<( std::ostream& out, 
   const ctl::detail::Simplicial_chain_complex< Cell_, Boundary_, Data_, Hash_> & c){ 
	for(auto i = c.begin(); i != c.end(); ++i){
		      const std::size_t id = i->second.id();
		      out << id; 
		      out << ": ";
		      out << i->first << std::endl; 
	}
	return out;
}

template< typename Cell_,
          typename Boundary_,
          typename Data_,
          typename Hash_> 
std::ostream& operator<<( std::ostream& out, 
		    const ctl::detail::Simplicial_chain_complex< Cell_, Boundary_, Data_, Hash_>&& c){
	out << c;
	return out;
}
template< typename Stream, typename Cell_,
          typename Boundary_,
          typename Data_,
          typename Hash_>
Stream& operator>>( Stream& in, ctl::detail::Simplicial_chain_complex< Cell_, Boundary_, Data_, Hash_> & c){  
	return c.read( in); 
}

} //end namespace detail
} //end namespace ctl

#endif //CTL_SIMPLICIAL_CHAIN_COMPLEX_H
