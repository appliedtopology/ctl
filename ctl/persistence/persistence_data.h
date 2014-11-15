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

#include <boost/property_map/property_map.hpp>

namespace ctl {
namespace detail{
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
 	typedef typename boost::property_traits< Cell_chain_map>::value_type 
								       Chain;
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

} //end namespace detail
} //end namespace ctl

#endif //CTLIB_PERSISTENCE_DATA_H
