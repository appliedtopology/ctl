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
#include <ctl/utility/recombine.h>
#include <ctl/chain_complex/detail/multi_array.h>
#include <ctl/cube/cube.h>
#include <ctl/cube/detail/cube.h>

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
