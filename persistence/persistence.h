#ifndef CTLIB_PERSISTENCE_H
#define CTLIB_PERSISTENCE_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the below liscence, but please
* do not commit acts of academic dishonesty. We encourage and request that 
* for any academic use of this source code one should cite the following 
* papers:
* 
* \cite{$bibtex_names_here}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) $NAMES_OF_AUTHORS $YEARS_FIRST_CREATED <$emails>
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

#include "persistence/persistence_data.h"

//exported functionality
namespace ctl{

//two different outputs
class partner {};
class partner_and_cascade {};

template< typename Persistence_data>
void eliminate_boundaries( Persistence_data & persistence_data){
   while( !data.cascade_boundary.size()){
	auto tau = cascade_boundary.youngest();
	auto bd_cascade_tau = data.cascade_boundary_map[ tau];
	//tau is the partner
	if( bd_cascade_tau.empty()){ return; }
	//otherwise tau has a partner
	auto tau_partner_term = tau_boundary.youngest();
	auto tau_partner = tau_parter_term.cell();
	auto bd_cascade_tau_partner = data.cascade_boundary_map[ tau_partner];
	auto scalar = tau_partner_term.coefficient().inverse();
	//TODO: Make sure terms here are correct
  	data.cascade_boundary.scaled_add( scalar, bd_cascade_tau_partner);
  }
}

template< typename Cell, typename Chain_map>
bool is_creator( const Cell & cell, Chain_map & cascade_boundary_map){
	typedef typename Chain_map::value_type Chain;
	typedef typename Chain::Less Term_less;
	const Chain& bd = cascade_boundary_map[ cell];
	Term_less term_less;
	return  bd.empty() || term_less( cell, boundary.youngest());
}

template< typename Cell, typename Persistence_data>
void initialize_cascade_data( const Cell & sigma, const std::size_t pos, 
			      Persistence_data & data, partner){
	typedef typename Persistence_data::Chain Chain;
	Chain& cascade_boundary = data.cascade_boundary;
	cascade_boundary.reserve( data.bd.max_length( sigma));
	for( auto i = data.boundary_operator.begin( sigma, pos);
		  i != data.boundary_operator.end( sigma);
		  ++i){
	    //the += here maintains that the chain is sorted
	    if( is_creator( i->cell(), data.cascade_boundary_map)){
		cascade_boundary+=(*i);    
	    }
	}
}

template< typename Cell, typename Persistence_data>
void initialize_cascade_data( const Cell & cell, const std::size_t pos, 
			      Persistence_data & data, partner_and_cascade){
	data.cascade += Term( cell, 1); 
	initialize_cascade_data( cell, data, partner);
}

//nothing to store when we don't store cascades.
template< typename Persistence_data, typename Cell>
void store_cascade( Persistence_data & data, const Cell & cell, partner){}	

//nothing to store when we don't store cascades.
template< typename Persistence_data, typename Cell>
void store_scaled_cascade( Persistence_data & data, 
			   const Cell & cell, partner){}	

template< typename Persistence_data, typename Cell>
void store_cascade( Persistence_data & data, const Cell & cell, 
		    partner_and_cascade){
	data.cascade_map[ cell] = data.cascade;
}

template< typename Persistence_data, typename Cell>
void store_scaled_cascade( Persistence_data & data, const Cell & cell, 
		    partner_and_cascade){
	auto scalar = data.cascade_boundary.normalize();
	data.cascade_map[ cell] = scalar*data.cascade;
}

template< typename Filtration_iterator,
	  typename Boundary_operator,
	  typename Chain_map,
	  typename Output_policy>
void pair_cells( Filtration_iterator & begin,
		 Filtration_iterator & end,
		 Boundary_operator & bd,
		 Chain_map & cascade_boundary_map,
		 Chain_map & cascade_map,
		 Output_policy policy){
	
	typedef Persistence_data< Term_less, 
				  Boundary_operator, 
				  Chain_map, 
				  Output_policy> Data;
	typedef typename Filtration_iterator::value_type Cell_iterator; 
	Data persistence_data( term_less, bd, 
			       cascade_boundary_map, cascade_map); 
	//TODO: pos should not start at zero, 
	//Filtration_iterator should know its position
	for(std::size_t pos = 0; begin != end; ++begin, ++pos){	
		Cell_iterator sigma = *begin;
		initialize_cascade_data( sigma, pos, data, output_policy);
		eliminate_boundaries( data);
		if( !data.cascade_boundary.empty() ){
			auto tau = youngest( data.cascade_boundary).cell();
		 	//make tau sigma's partner
			cascade_boundary_map[ sigma] = data.cascade_boundary;
			store_scaled_cascade( data, sigma, output_policy);  
		        //make sigma tau's partner
			Term sigma_term( sigma, 1);	
			Chain tau_boundary_chain( sigma_term);  
			cascade_boundary_map[ tau] = 
					std::move( tau_boundary_chain); 
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
			 cascade, partner_and_cascade());
}
template< typename Filtration_iterator, 
	  typename Boundary_operator,
	  typename Chain_map>
void persistence( Filtration_iterator begin,
		  Filtration_iterator end,
		  Boundary_operator & bd,
		  Chain_map & cascade_boundary_map){
	Chain_map not_going_to_be_used; 
	ctl::pair_cells( begin, end, bd, cascade_boundary_map, 
			 not_going_to_be_used, partner());
}		  
} //namespace ctl

//non-exported functionality
namespace {} //anonymous namespace

#endif //CTLIB_PERSISTENCE_H
