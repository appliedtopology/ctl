#ifndef CTLIB_PERSISTENCE_DATA_H
#define CTLIB_PERSISTENCE_DATA_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
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
	cascade(), cascade_boundary(), temporary_chain() {};

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
