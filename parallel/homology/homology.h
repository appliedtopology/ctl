#ifndef _CTL_PARALLEL_HOMOLOGY_H
#define _CTL_PARALLEL_HOMOLOGY_H
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
*******************************************************************************
* NOTES
*
*
*******************************************************************************/
// Global Project Deps
#include "abstract_simplex.h"
#include "simplex_boundary.h"
#include "complex_boundary.h"
#include "parallel_filtration.h"
#include "cell_set.h"
#include "io.h" 
#include "term.h"
#include "timer.h"
#include "thread_timer.h"
#include "point.h"
#include "point_vector.h"

//Persistence 
// Boost
#include <boost/property_map/property_map.hpp>
#include <boost/unordered_map.hpp>
#include "finite_field.h"
#include "term_z2.h"
#include "chain.h"
#include "persistence.h"

//Blowup
#include "product_boundary.h"
#include "iterator_product.h"
#include "blowup_io.h"

//Local Project Deps
#include "build_blowup_complex.h"
#include "parallel_persistence.h"

//TBB
#include <tbb/concurrent_vector.h>

namespace ctl {

template< typename Complex,
	  typename Stats>
void compute_homology( Complex & complex, 
		       size_t & num_parts,
		       Stats & stats){
	typedef typename Complex::iterator Complex_iterator;
	typedef typename ctl::Cover_less< Complex_iterator> Cell_less;
	typedef typename ctl::Parallel_filtration< Complex, 
						   Cell_less> Filtration;

	typedef typename Complex::Cell_boundary Cell_boundary;
	typedef typename ctl::Complex_boundary< Complex, 
						Complex_iterator> 
							  Complex_boundary;
	typedef typename  Filtration::iterator Filtration_iterator;
	typedef typename  std::pair< Filtration_iterator, 
				     Filtration_iterator> Filtration_pair;
	typedef typename  tbb::concurrent_vector< Filtration_pair> 
								Iterator_pairs;
	
	typedef typename  ctl::Term_Z2< Complex_iterator> Term_Z2;
	typedef typename  ctl::Term_less< Term_Z2, Cell_less> Term_less;
	typedef typename  ctl::Chain< Term_Z2, Term_less> Chain;
	typedef typename  tbb::concurrent_vector< Chain> Chains;
	typedef typename  Chains::iterator Chains_iterator;
	typedef typename  ctl::Id_property_map< Complex_iterator> Offset_map;
	typedef typename  boost::iterator_property_map< Chains_iterator,
				Offset_map, Chain, Chain&> Cell_chain_map;

	typedef typename tbb::concurrent_vector< int> Betti;

	stats.timer.start();
	Filtration filtration( complex);
	stats.filtration_time = stats.timer.get();


	Iterator_pairs ranges;
	stats.timer.start();
	ctl::get_cover_iterators( filtration.begin(),
			          filtration.end(),
				  ranges, true);
	stats.get_iterators = stats.timer.get();

	Chains cascades( complex.max_id()+1, Chain( Term_less() ));
	Cell_chain_map cascade_prop_map( cascades.begin(), Offset_map());
	Cell_boundary cell_boundary( complex.cell_boundary());
	Complex_boundary complex_boundary( complex, cell_boundary);

	stats.timer.start();
	ctl::parallel_persistence( ranges,
			           complex_boundary,
			           cascade_prop_map, 
			           num_parts );
	stats.parallel_persistence = stats.timer.get();
	#ifdef COMPUTE_BETTI
	        //betti numbers
	        Betti betti;
	        std::cout << "parallel betti (blowup): " << std::endl;
	        ctl::compute_betti( complex, cascade_prop_map, betti);
	        std::cout << std::endl;
	#endif
}
/*
template< typename Blowup, 
	  typename Cell_less,    
	  typename Stats>
void do_blowup_homology( Blowup & blowup_complex, 
			 Cell_less & less,
			 size_t num_local_pieces,
			 Stats & stats){

	typedef typename Blowup::iterator Blowup_iterator;
	typedef typename Blowup::Cell_boundary Cell_boundary;
	typedef typename ctl::Parallel_filtration< Blowup, 
						   Cell_less> Blowup_filtration;
	typedef typename ctl::Complex_boundary< Blowup, 
						Blowup_iterator> 
							  Blowup_boundary;
	typedef typename  Blowup_filtration::iterator Filtration_iterator;
	typedef typename  std::pair< Filtration_iterator, 
				     Filtration_iterator> Filtration_pair;
	typedef typename  std::vector< Filtration_pair> Iterator_pairs;
	
	typedef typename  ctl::Term_Z2< Blowup_iterator> Term_Z2;
	typedef typename  ctl::Term_less< Term_Z2, Cell_less> Term_less;
	typedef typename  ctl::Chain< Term_Z2, Term_less> Chain;
	typedef typename  std::vector< Chain> Chains;
	typedef typename  Chains::iterator Chains_iterator;
	typedef typename  ctl::Id_property_map< Blowup_iterator> Offset_map;
	typedef typename  boost::iterator_property_map< Chains_iterator,
				Offset_map, Chain, Chain&> Cell_chain_map;

	stats.timer.start();
	Blowup_filtration blowup_filtration( blowup_complex);
	stats.filtration_time = stats.timer.get();
	Iterator_pairs ranges;
	stats.timer.start();
	ctl::get_cover_iterators( blowup_filtration.begin(),
			          blowup_filtration.end(),
				  ranges);
  	Chains cascades( blowup_complex.max_id()+1, Chain( Term_less() ));
  	Cell_chain_map cascade_prop_map( cascades.begin(), Offset_map());
	Cell_boundary cell_boundary( blowup_complex.cell_boundary());
  	Blowup_boundary blowup_complex_boundary( blowup_complex, cell_boundary);
	stats.timer.start();
  	ctl::parallel_persistence( ranges,
			           blowup_complex_boundary,
			           cascade_prop_map, 
			           num_local_pieces );
	stats.parallel_persistence = stats.timer.get();
#ifdef COMPUTE_BETTI
	typedef typename tbb::concurrent_vector< int> Betti;
        //betti numbers
        Betti betti;
        std::cout << "parallel betti (blowup): " << std::endl;
        ctl::compute_betti( blowup_complex,cascade_prop_map,betti);
        std::cout << std::endl;
#endif
}
*/
template< typename Blowup, 
	  typename Blowup_filtration,
	  typename Nerve_filtration,
	  typename Stats>
void do_blowup_homology( Blowup & blowup_complex,
			 Blowup_filtration & blowup_filtration,
			 Nerve_filtration & nerve_filtration, 
			 size_t num_local_pieces,
			 Stats & stats){

	typedef typename Blowup::iterator Blowup_iterator;
	typedef typename Blowup::Cell_boundary Cell_boundary;
	typedef typename ctl::Complex_boundary< Blowup, 
						Blowup_iterator> 
						Blowup_boundary;
	typedef typename  Blowup_filtration::Cell_iterator_less Cell_less;	
	typedef typename  Blowup_filtration::iterator 
						Blowup_filtration_iterator;
	typedef typename  Nerve_filtration::iterator Nerve_filtration_iterator;
	
	typedef typename  ctl::Term_Z2< Blowup_iterator> Term_Z2;
	typedef typename  ctl::Term_less< Term_Z2, Cell_less> Term_less;
	typedef typename  ctl::Chain< Term_Z2, Term_less> Chain;
	typedef typename  std::vector< Chain> Chains;
	typedef typename  Chains::iterator Chains_iterator;
	typedef typename  ctl::Id_property_map< Blowup_iterator> Offset_map;
	typedef typename  boost::iterator_property_map< Chains_iterator,
				Offset_map, Chain, Chain&> Cell_chain_map;
	typedef typename std::pair< Blowup_filtration_iterator, 
				    Blowup_filtration_iterator> Filtration_pair;
	typedef typename tbb::concurrent_vector< Filtration_pair> 
							Iterator_pairs;
	stats.timer.start();
	Iterator_pairs ranges;
	Blowup_filtration_iterator current = blowup_filtration.begin(), beyond;
	Nerve_filtration_iterator _begin=nerve_filtration.begin();
	Nerve_filtration_iterator _end=_begin+num_local_pieces;
	for(Nerve_filtration_iterator i = _begin; 
				      i != _end; ++i){
			beyond = current + (*i)->second.count();
			ranges.push_back( make_pair( current, beyond));
			current = beyond;
	}
	ranges.push_back( make_pair( current, blowup_filtration.end()));
  	Chains cascades( blowup_complex.max_id()+1, Chain( Term_less() ));
  	Cell_chain_map cascade_prop_map( cascades.begin(), Offset_map()); 
	Cell_boundary cell_boundary( blowup_complex.cell_boundary());
  	Blowup_boundary blowup_complex_boundary( blowup_complex, cell_boundary);
  	std::cout << "calling parallel persistence: " << std::endl;
	ctl::parallel_persistence( ranges,
			           blowup_complex_boundary,
			           cascade_prop_map, 
			           num_local_pieces );
	stats.parallel_persistence = stats.timer.get();
#ifdef COMPUTE_BETTI
	typedef typename tbb::concurrent_vector< int> Betti;
        //betti numbers
        Betti betti;
        std::cout << "parallel betti (blowup): " << std::endl;
        ctl::compute_betti( blowup_complex,cascade_prop_map,betti);
        std::cout << std::endl;
#endif
}
} //namespace ctl
#endif //_CTL_PARALLEL_HOMOLOGY_H
