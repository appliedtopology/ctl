#ifndef _CTL_PARALLEL_HOMOLOGY_H
#define _CTL_PARALLEL_HOMOLOGY_H
/*******************************************************************************
* -Academic Honesty-
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************
* NOTES
*******************************************************************************/

// Global Project Deps
#include <ctl/io/io.hpp> 
#include <ctl/term/term.hpp>
#include <ctl/utility/timer.hpp>
#include <ctl/parallel/utility/timer.hpp>
#include <ctl/graded_chain_complex/graded_boundary.hpp>

//Persistence 
// Boost
#include <boost/property_map/property_map.hpp>
#include <boost/unordered_map.hpp>
#include <ctl/finite_field/finite_field.hpp>
#include <ctl/chain/chain.hpp>
#include <ctl/persistence/persistence.hpp>
#include <ctl/matrix/offset_maps.hpp>
#include <ctl/matrix/iterator_property_map.hpp>
#include <ctl/matrix/matrix.hpp>

//Local Project Deps
//#include <ctl/parallel/build_blowup_complex/build_blowup_complex.h>
#include <ctl/parallel/homology/persistence.hpp>

//TBB
#include <tbb/concurrent_vector.h>

namespace ctl {
namespace parallel{
template< typename Complex,
	  typename Nerve,
	  typename Stats>
void compute_homology( Complex & complex,
		       const Nerve & nerve, 
		       size_t & num_parts,
		       Stats & stats){
	typedef typename Complex::iterator Complex_iterator;
	typedef typename ctl::parallel::Cover_less< Complex_iterator> Cell_less;
	typedef typename ctl::parallel::Filtration< Complex, Cell_less, Complex_iterator,
						    std::vector< Complex_iterator> > 
								Filtration;
	typedef typename Filtration::iterator Filtration_iterator;
	//typedef typename Complex::Boundary Cell_boundary;
	typedef typename ctl::Graded_boundary< Filtration> 
						Filtration_boundary;
	typedef typename Filtration_boundary::Coefficient Coefficient;

	typedef typename ctl::Sparse_matrix< Coefficient> Sparse_matrix;
        typedef typename ctl::Offset_map< Filtration_iterator> Offset_map;
        typedef typename ctl::Sparse_matrix_map< Coefficient, Offset_map> Chain_map;

	typedef typename std::pair< Filtration_iterator, Filtration_iterator> 
							     Filtration_pair;
	typedef typename std::vector< Filtration_pair> Iterator_pairs;
	//Change this to filtration term
	//typedef typename tbb::concurrent_vector< int> Betti;

	stats.timer.start();
	Filtration filtration( complex);
	stats.timer.stop();
	stats.filtration_time = stats.timer.elapsed();



	stats.timer.start();
	Iterator_pairs ranges( nerve.size(), 
		std::make_pair( filtration.begin(), filtration.begin()) );
	for( auto i = nerve.begin(); i != nerve.end(); ++i){
		//the vertex i has id i-1 by design.
		const std::size_t range_index = i->second.id()-1;
		ranges[ range_index].second += i->second.count();
	}
	for( std::size_t i = 1, offset=0; i < ranges.size(); ++i){
		offset += std::distance( ranges[ i-1].first, ranges[ i-1].second);
		ranges[ i].first += offset;
		ranges[ i].second += offset;
	}
	stats.timer.stop();
	stats.get_iterators = stats.timer.elapsed();

	Sparse_matrix R( complex.size());
	Offset_map offset_map( filtration.begin());
	Chain_map R_map( R.begin(), offset_map);
	Filtration_boundary filtration_boundary( filtration);

	auto times = ctl::parallel::persistence( ranges,
			           filtration_boundary,
			           R_map, 
			           num_parts, offset_map);
	stats.initialize_cascade_boundary = times.first;
	stats.parallel_persistence = times.second;
	#ifdef COMPUTE_BETTI
	        //betti numbers
	        Betti betti;
	        std::cout << "parallel betti (blowup): " << std::endl;
	        ctl::compute_betti( filtration, cascade_prop_map, betti, true);
	        std::cout << std::endl;
	#endif
}

template< typename Blowup, 
	  typename Blowup_filtration,
	  typename Nerve_filtration,
	  typename Stats>
void do_blowup_homology( Blowup & blowup_complex,
			 Blowup_filtration & blowup_filtration,
			 Nerve_filtration & nerve_filtration, 
			 size_t num_local_pieces,
			 Stats & stats){

	//typedef typename Blowup::iterator Blowup_iterator;
	typedef typename ctl::Graded_boundary< Blowup_filtration> 
						      Blowup_boundary;
	typedef typename  Blowup_filtration::iterator 
						Blowup_filtration_iterator;
	typedef typename  Nerve_filtration::iterator Nerve_filtration_iterator;
	
      	//typedef typename  Blowup_boundary::Term Blowup_term;
	typedef typename  Blowup_filtration::Term Blowup_filtration_term;	
	typedef typename  Blowup_filtration_term::Coefficient Coefficient;

        typedef typename ctl::Sparse_matrix< Coefficient> Sparse_matrix;
        typedef typename ctl::Offset_map< Blowup_filtration_iterator> Offset_map;
        typedef typename ctl::Sparse_matrix_map< Coefficient, Offset_map> Chain_map;

	typedef typename std::pair< Blowup_filtration_iterator, 
				    Blowup_filtration_iterator> Filtration_pair;
	typedef typename std::vector< Filtration_pair> Iterator_pairs;
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
  	Sparse_matrix R( blowup_complex.size()); 
	Offset_map offset_map( blowup_filtration.begin());
	Chain_map R_map( R.begin(), offset_map);
  	Blowup_boundary blowup_filtration_boundary( blowup_filtration); 
	auto p = ctl::parallel::persistence( ranges, blowup_filtration_boundary, 
				    	     R_map, num_local_pieces, offset_map);
	stats.initialize_cascade_boundary = p.first; 
	stats.parallel_persistence = p.second;
#ifdef COMPUTE_BETTI
	typedef typename tbb::concurrent_vector< int> Betti;
        //betti numbers
        Betti betti;
        std::cout << "parallel betti (blowup): " << std::endl;
        ctl::compute_betti( blowup_filtration, cascade_prop_map, betti, true);
        std::cout << std::endl;
#endif
}
} //namespace parallel
} //namespace ctl
#endif //_CTL_PARALLEL_HOMOLOGY_H
