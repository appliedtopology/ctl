#ifndef CTL_DEFAULT_COMPLEX_STORAGE_H
#define CTL_DEFAULT_COMPLEX_STORAGE_H
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
* NOTES:
* August 29, 2014 -- We are refactoring our Chain_complex so that the underlying
* Storage is divorced from the interface. This way we can use different types 
* for different types of Cells. 
*
* We use Metaprogramming instead of inheritence so that the appropriate 
* structures are determined at compile time.
* 
* This type dispatching is done here.
*******************************************************************************/

//STL
#include <type_traits>
#include <unordered_map>

//CTL
#include <ctl/utility/recombine.hpp>
#include <ctl/chain_complex/detail/multi_array.hpp>
#include <ctl/cube/cube.hpp>
#include <ctl/cube/detail/cube.hpp>

namespace ctl {
namespace detail {

template< typename Cell, 
	  typename Data, 
	  typename Hash>
using Default_complex_storage = 
typename std::conditional<
//If C is a type of the form ctl::Cube< T> for any T, 
std::is_same< typename recombine< Cell, Dummy>::type, 
	      ctl::Cube< Dummy> >::value, 
	      multi_array< std::pair< ctl::detail::Cube< Cell>, Data> >,
	      std::unordered_map< Cell, Data, Hash>
	    >::type;
} //namespace detail  
} //namespace ctl


#endif //CTL_CHAIN_COMPLEX_MAP_H
