#ifndef CTLIB_PERSISTENCE_DATA_H
#define CTLIB_PERSISTENCE_DATA_H
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
* \cite{zc-cph-04, z-ct-10}
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

#include "boost/property_map/property_map.hpp"

namespace _ctl {

template< typename Term_less_, 
	  typename Boundary_operator_, 
	  typename Cell_chain_map_,
	  typename Output_policy_ > 
//hold onto persistence data
struct Persistence_data {

	typedef Term_less_ Term_less; 
	typedef Boundary_operator_ Boundary_operator;
	typedef Cell_chain_map_ Cell_chain_map;
	typedef Output_policy_ Output_policy;
 	typedef typename boost::property_traits< Cell_chain_map>::value_type Chain;
	typedef typename Chain::value_type Term;
 
	Persistence_data( Term_less t, 
			  Boundary_operator & bd_,
			  Cell_chain_map & bd_map_,	
			  Cell_chain_map & map_,
			  Output_policy p):
	term_less( t), bd( bd_), cascade_boundary_map( bd_map_), 
	cascade_map( map_), policy( p), 
	cascade( 1), cascade_boundary(), temporary_chain() {};

	Term_less term_less;
	Boundary_operator bd;
	Cell_chain_map& cascade_boundary_map; 
	Cell_chain_map& cascade_map; 
	Output_policy policy;

	//at each iteration we use these temporaries and then
	//we swap them into place in the vector. 
	Chain cascade;
	Chain cascade_boundary;
	//when we do the x <- x+y in persistence we 
	//end up creating a temporary:
	// z <- x+y
	// x.swap( z)
	// by storing this temporary here, we avoid the reallocation 
	// at every iteration
	Chain temporary_chain;
}; //struct Persistence_data 

} //end namespace _ctl

#endif //CTLIB_PERSISTENCE_DATA_H
