#ifndef CTLIB_PERSISTENCE_H
#define CTLIB_PERSISTENCE_H
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
#include <ctl/persistence/persistence_data.h>
#include <ctl/io/io.h>

//exported functionality
namespace ctl{

//two different outputs
class partner {};
class partner_and_cascade {};

template< typename Persistence_data>
void eliminate_boundaries( Persistence_data & data){
   //Cell here is really a pointer into the complex
   typedef typename Persistence_data::Chain Chain;
   typedef typename Chain::value_type Term;
   //typedef typename Term::Cell Cell;
   typedef typename Term::Coefficient Coefficient;
   while( !data.cascade_boundary.empty()){
	//instead of directly accessing tau, we just get tau's position
	const Term& tau_term = data.cascade_boundary.youngest();
	const Chain& bd_cascade_tau = data.cascade_boundary_map[ tau_term];
	//tau is the partner
	if( bd_cascade_tau.empty()){ return; }
	//otherwise tau has a partner
	const Term& tau_partner_term = bd_cascade_tau.youngest();
	const Chain& bd_cascade_tau_partner = 
				data.cascade_boundary_map[ tau_partner_term];
	const Coefficient scalar = tau_partner_term.coefficient().inverse();
  	data.cascade_boundary.scaled_add( scalar, bd_cascade_tau_partner,
					  data.temporary_chain);
  }
}

template< typename Term, typename Chain_map>
bool is_creator( const Term & term, Chain_map & cascade_boundary_map){
	typedef typename Chain_map::value_type Chain;
	typedef typename Chain::Less Term_less;
	const Chain& bd = cascade_boundary_map[ term];
	Term_less term_less;
	return  bd.empty() || term_less( term, bd.youngest());
}

template< typename Filtration_iterator, 
	  typename Persistence_data>
void initialize_cascade_data( const Filtration_iterator sigma_iterator, 
			      const Filtration_iterator begin,
			      Persistence_data & data, partner){
     typedef typename Filtration_iterator::value_type Cell;
     typedef typename Persistence_data::Chain Chain;
     typedef typename Chain::value_type Term;
     const Cell & sigma = *sigma_iterator;
     Chain& cascade_boundary = data.cascade_boundary;
     cascade_boundary.reserve( data.bd.length( sigma));
     //using pos here stores the pos in the data section of the complex
     const std::size_t pos = std::distance( begin, sigma_iterator); 
     for( auto i = data.bd.begin( sigma, pos); i != data.bd.end( sigma); ++i){
         if( is_creator( *i, data.cascade_boundary_map)){
		//What we did was rebind the term in Filtration so that terms 
		//contain filtration iterators. These are in order so 
		//set_symmetric_difference
		cascade_boundary.emplace( begin + i->cell()->second.pos(), 
					  i->coefficient());
	}
     }
     cascade_boundary.sort();
}


template< typename Cell, typename Persistence_data>
void initialize_cascade_data( const Cell & cell, const std::size_t pos, 
			      Persistence_data & data, partner_and_cascade){
	typedef typename Persistence_data::Chain::Term Term;
	//TODO: fix cascade to avoid this
	data.cascade.add( Term( cell, 1, pos)); 
	initialize_cascade_data( cell, data, partner());
}

//nothing to store when we don't store cascades.
template< typename Persistence_data, typename Cell>
void store_cascade( Persistence_data & data, const Cell & cell, partner){}	

//nothing to store when we don't store cascades.
template< typename Persistence_data, typename Filtration_iterator>
void store_scaled_cascade( Persistence_data & data, 
			   const Filtration_iterator sigma, partner){}	

template< typename Persistence_data, typename Filtration_iterator>
void store_cascade( Persistence_data & data, 
		    const Filtration_iterator sigma, 
		    partner_and_cascade){
	data.cascade_map[ sigma].swap( data.cascade);
}

template< typename Persistence_data, typename Filtration_iterator>
void store_scaled_cascade( Persistence_data & data, 
			   const Filtration_iterator sigma, 
		           partner_and_cascade){
	const auto scalar = data.cascade_boundary.normalize();
	data.cascade *= scalar;
	data.cascade_map[ sigma].swap( data.cascade);
}

template< typename Filtration_iterator,
	  typename Boundary_operator,
	  typename Chain_map,
	  typename Output_policy>
void pair_cells( Filtration_iterator begin, Filtration_iterator end,
		 Boundary_operator & bd, 
		 Chain_map & cascade_boundary_map, Chain_map & cascade_map,
		 Output_policy output_policy){

	typedef typename boost::property_traits< Chain_map>::value_type Chain;
	typedef typename Chain::Less Term_less;
	//this should now operator on filtration pointers, so it should be fast.	
	typedef ctl::detail::Persistence_data< Term_less, Boundary_operator, 
				  	Chain_map, Output_policy> 
					            Persistence_data;
	typedef typename Filtration_iterator::value_type Cell_iterator; 
	typedef typename Chain::Term Term;
	Term_less term_less; 
	Persistence_data data( term_less, bd, cascade_boundary_map, cascade_map, 
			       output_policy);
	for(Filtration_iterator sigma = begin ; sigma != end; ++sigma){
		initialize_cascade_data( sigma, begin, data, output_policy);
		eliminate_boundaries( data);
		if( !data.cascade_boundary.empty() ){
		     //make tau sigma's partner
		     const Term& tau = data.cascade_boundary.youngest();
		     cascade_boundary_map[ sigma].swap( data.cascade_boundary);
		     store_scaled_cascade( data, sigma, output_policy);  
		     //make sigma tau's partner
		     cascade_boundary_map[ tau].emplace( sigma, 1); 
		} else{
		     store_cascade( data, sigma, output_policy); 
		}
	}
}

template< typename Filtration_iterator, 
	  typename Boundary_operator,
	  typename Chain_map>
void persistence( Filtration_iterator begin,
		  Filtration_iterator end,
		  Boundary_operator & bd,
		  Chain_map & cascade_boundary_map,
		  Chain_map & cascade_map){
	ctl::pair_cells( begin, end, bd, cascade_boundary_map, 
			 cascade_map, partner_and_cascade());
}
template< typename Filtration_iterator, 
	  typename Boundary_operator,
	  typename Chain_map>
void persistence( Filtration_iterator begin,
		  Filtration_iterator end,
		  Boundary_operator & bd,
		  Chain_map & cascade_boundary_map){
	Chain_map not_going_to_be_used = cascade_boundary_map; 
	ctl::pair_cells( begin, end, bd, cascade_boundary_map, 
			 not_going_to_be_used, partner());
}		  
} //namespace ctl

#endif //CTLIB_PERSISTENCE_H
