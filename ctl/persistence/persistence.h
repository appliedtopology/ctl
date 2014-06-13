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
#include <ctl/parallel/utility/timer.h>

//exported functionality
namespace ctl{

//two different outputs
class partner {};
class partner_and_cascade {};

template< typename Persistence_data>
inline void 
scaled_add( Persistence_data & data, 
	    const typename Persistence_data::Chain& bd_cascade_tau_partner){
   //Cell here is really a pointer into the complex
   typedef typename Persistence_data::Chain Chain;
   typedef typename Chain::value_type Term;
   typedef typename Term::Coefficient Coefficient;
 
   const Coefficient& scalar = tau_partner_term.coefficient().inverse();
   data.cascade_boundary.scaled_add( scalar, bd_cascade_tau_partner, 
				     data.temporary_chain);	
}

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
	scaled_add( data, data.cascade_boundary_map[ tau_partner_term]);
  }
}

template< typename Term, typename Chain_map>
bool is_creator( const Term & term, Chain_map & cascade_boundary_map){
	typedef typename boost::property_traits< Chain_map>::value_type Chain;
	typedef typename Chain::Less Term_less;
	const Chain& bd = cascade_boundary_map[ term];
	Term_less term_less;
	return  bd.empty() || term_less( term, bd.youngest());
}

template< typename Term, typename Chain_map>
struct Is_not_creator{
   Is_not_creator( Chain_map & cbp): cascade_boundary_map( cbp) {}
   bool operator()( const Term & term) const { 
	return !is_creator( term, cascade_boundary_map);
   }
   Chain_map & cascade_boundary_map;
}; //struct Is_not_creator

template< typename Filtration_iterator, 
	  typename Persistence_data>
void remove_destroyers( const Filtration_iterator sigma, Persistence_data & data){
     typedef typename Persistence_data::Chain Chain;
     typedef typename Persistence_data::Cell_chain_map Chain_map;
     typedef typename Chain::value_type Term;
     typedef typename Chain::reverse_iterator reverse_iterator;
     typedef Is_not_creator< Term, Chain_map> Is_not_creator;
     Is_not_creator is_not_creator( data.cascade_boundary_map);
     Chain& boundary = data.cascade_boundary;
     reverse_iterator begin = std::remove_if( boundary.rbegin(), boundary.rend(), is_not_creator);
     boundary.erase( begin, boundary.rend());
}


template< typename Filtration_iterator, 
	  typename Persistence_data>
void initialize_cascade_data( const Filtration_iterator sigma, 
			      Persistence_data & data, partner){
     //typedef typename Filtration_iterator::value_type Cell;
     typedef typename Persistence_data::Chain Chain;
     typedef typename Chain::value_type Term;
     Chain& cascade_boundary = data.cascade_boundary;
     cascade_boundary.reserve( data.bd.length( sigma));
     for( auto i = data.bd.begin( sigma); i != data.bd.end( sigma); ++i){
         //if( is_creator( *i, data.cascade_boundary_map)){
		//cascade_boundary.add( Term( i->cell(), i->coefficient()));
		cascade_boundary.emplace( i->cell(), i->coefficient());
	//}
     }
     cascade_boundary.sort();
}


template< typename Filtration_iterator, typename Persistence_data>
void initialize_cascade_data( const Filtration_iterator & cell, 
			      Persistence_data & data, partner_and_cascade){
	typedef typename Persistence_data::Chain::Term Term;
	//TODO: Replace me!
	//data.cascade.emplace( cell, Coefficient( 1));
	data.cascade.add( Term( cell, 1)); 
	initialize_cascade_data( cell, data, partner());
}

//nothing to store when we don't store cascades.
template< typename Persistence_data, typename Filtration_iterator>
void store_cascade( const Persistence_data & data, 
		    const Filtration_iterator & cell, partner){}	

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
std::pair< double, double> 
pair_cells( Filtration_iterator begin, Filtration_iterator end,
		 Boundary_operator & bd, 
		 Chain_map & cascade_boundary_map, Chain_map & cascade_map,
		 Output_policy output_policy){

	typedef typename boost::property_traits< Chain_map>::value_type Chain;
	typedef typename Chain::Less Term_less;
	//this should now operator on filtration pointers, so it should be fast.	
	typedef ctl::detail::Persistence_data< Term_less, Boundary_operator, 
				  		Chain_map, Output_policy> 
					            Persistence_data;
	//typedef typename Filtration_iterator::value_type Cell_iterator; 
	typedef typename Chain::Term Term;
	ctl::parallel::Timer timer;
	timer.start();
	Term_less term_less; 
	Persistence_data data( term_less, bd, cascade_boundary_map, cascade_map, 
			       output_policy);
	for(Filtration_iterator sigma = begin; sigma != end; ++sigma){
		cascade_boundary_map[ sigma].swap( data.cascade_boundary);
		initialize_cascade_data( sigma, data, output_policy);
		cascade_boundary_map[ sigma].swap( data.cascade_boundary);
	}
	timer.stop();
	double init_cascade_time = timer.elapsed();
	timer.start();
	for(Filtration_iterator sigma = begin; sigma != end; ++sigma){
		//hand the column we want to operate on to our temporary.
		cascade_boundary_map[ sigma].swap( data.cascade_boundary);
		remove_destroyers( sigma, data); // since we didnt do this earlier.	
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
	timer.stop();
	double persistence_algorithm = timer.elapsed();
	return std::make_pair( init_cascade_time, persistence_algorithm);
}

template< typename Filtration_iterator, 
	  typename Boundary_operator,
	  typename Chain_map>
std::pair< double,double> 
persistence( Filtration_iterator begin,
		  Filtration_iterator end,
		  Boundary_operator & bd,
		  Chain_map & cascade_boundary_map,
		  Chain_map & cascade_map){
	return ctl::pair_cells( begin, end, bd, cascade_boundary_map, 
			 cascade_map, partner_and_cascade());
}
template< typename Filtration_iterator, 
	  typename Boundary_operator,
	  typename Chain_map>
std::pair< double, double> 
persistence( Filtration_iterator begin,
	     Filtration_iterator end,
	     Boundary_operator & bd,
	     Chain_map & cascade_boundary_map){
	Chain_map not_going_to_be_used = cascade_boundary_map; 
	return ctl::pair_cells( begin, end, bd, cascade_boundary_map, 
			 not_going_to_be_used, partner());
}		  
} //namespace ctl

#endif //CTLIB_PERSISTENCE_H
