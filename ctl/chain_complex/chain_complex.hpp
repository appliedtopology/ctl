#ifndef CTL_CHAIN_COMPLEX_H
#define CTL_CHAIN_COMPLEX_H
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
* Released under the BSD-3 License
*******************************************************************************/

//STL
#include <type_traits>

//CTL
#include <ctl/utility/recombine.hpp>
#include <ctl/hash/hash.hpp>
#include <ctl/chain_complex/detail/data_wrapper.hpp>
#include <ctl/chain_complex/detail/default_complex_storage.hpp>
#include <ctl/chain_complex/cubical_complex.hpp>
#include <ctl/chain_complex/simplicial_chain_complex.hpp>

namespace ctl {

/**
* @brief \class Complex_interface
* A structure which stores the standard cell basis for a Complex_interface
* The Cell type is specified via the template parameter Cell
* One must provide an operator which allows the boundary of a cell to be 
* enumerated. The type of this operator is the Boundary template type.
* 
* The library currently provides two cell types, ctl::Abstract_simplex< T> 
* and ctl::Cube< T>.
*
* The Data type can be anything, and is used to associate Data to a Cell.
*
* Presently, we wrap the Data parameter transparently to associate a unique id 
* to each cell. If the cells of the complex are inserted in a filtration order 
* Then sorting the cells of the complex according to these id's produces a 
* filtration ordering. 
*
* You may optionally provide a Hash function which might be used by an internal 
* container. The Hash function hashes Cells to a std::size_t. By default we use
* ctl::Hash.
*  
* The Storage parameter controls the internal data structure used for 
* storing the complex.
* This structure depends on the Cell type.
* At the time of writing, if Cell == ctl::Cube< T> we use 
* a std::vector< Data> the cube is stored implicitly as an offset into this
* structure. If a cell is not a cube, 
* then we use a std::unordered_map< Cell, Boundary, Data, Hash>
* 
* For those power users out there, one can roll their own internal
* storage container by modifying the Storage template parameter.
* This allows you to swap out hash tables, arrays, vectors, etc,
* without modifying the rest of the logic of th structure.
* 
* Further, one can just roll their own Storage container.
* 
* It must have an interface of Storage< Cell, Data, Hash>
*
* This is more flexible than you might imagine:, for example:
* To encode a std::vector< Data> as the container:
* template< typename Cell, typename Data, typename Hash>
* using Custom_vector_container = std::vector< Data>; 
* 
* In lieu of writing down the other requirements on the structure,
* we refer the reader to the implementation of the structures in 
* chain_complex/detail
* 
* In the future, one will simply "inherit" the interface for a 
* Storage container or complex.
* 
* At present the code below is not easily modifiable for adding 
* arbitrary complexes. This is intentional. Improvements will be added
* when necessary. 
*
* @tparam Cell
* @tparam Boundary
* @tparam Data
* @tparam Hash
* @tparam Storage
*/
template< typename Cell, 
	  typename Boundary, 
	  typename Data = detail::Default_data, 
	  typename Hash = ctl::Hash< Cell> 
	>
//meta code for the Chain_complex type.
//This metacode creates a type, Chain_complex< C, B, D, H, S>
using Chain_complex = 
typename std::conditional<
//If C is a type of the form ctl::Cube< T> for any T, 
std::is_same< typename detail::recombine< Cell, detail::Dummy>::type, 
	      ctl::Cube< detail::Dummy, 1> >::value, 
//then we create a Cubical_complex which optimizes storage.
 ctl::detail::Cubical_complex< Cell,
		       Boundary, 
		       detail::Data_wrapper< Data>, 
		       Hash>, 
//Otherwise all other cell types are stored generically
 ctl::detail::Simplicial_chain_complex< Cell, 
			        	Boundary, 
			        	detail::Data_wrapper< Data>, 
			        	Hash> 
	    >::type;


template< std::size_t N, typename T, typename Vertex_type=int>
using Simplicial_complex = 
ctl::Chain_complex< ctl::Abstract_simplex< Vertex_type>, 
		    ctl::Simplex_boundary< ctl::Abstract_simplex< Vertex_type>, 
				           ctl::Finite_field< N> >, T>;

template< std::size_t N, typename T, typename Vertex_type=int>
using Cubical_complex =
ctl::Chain_complex< ctl::Cube< Vertex_type>, 
		    ctl::Cube_boundary< ctl::Cube< Vertex_type>, 
					ctl::Finite_field< N> >, T>;

//! Utility to read a complex and any associated data 
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

//! Utility to read a complex 
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
