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
* \cite{hatcher, z-fcv-10a}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
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
#define COMPUTE_BETTI
//#define TESTS_ON
// Global Project Deps
#include "abstract_simplex.h"
#include "simplex_boundary.h"
#include "complex_boundary.h"
#include "filtration.h"
#include "parallel_filtration.h"
#include "cell_set.h"
#include "io.h" 
#include "term.h"
#include "timer.h"
#include "thread_timer.h"
#include "point.h"
#include "point_vector.h"
#include "concurrent_cell_set.h"
#include "compute_betti.h"

//Persistence 
// Boost
#include <boost/property_map/property_map.hpp>
#include <boost/unordered_map.hpp>
#include "finite_field.h"
#include "term_z2.h"
#include "chain.h"
#include "persistence.h"
#include "p2_helper.h"

//Blowup
#include "product_boundary.h"
#include "iterator_product.h"
#include "blowup_io.h"

//Local Project Deps
#include "covers.h"
#include "cover_tests.h"
#include "build_blowup_complex.h"
#include "parallel_homology.h"


//BOOST
#include <boost/program_options.hpp>

//STL
#include <sstream>
#include <vector>

#include <tbb/task_scheduler_init.h> 
#ifdef ZOOM
#include<zoom/zoom.h>
#endif
namespace po = boost::program_options;

// Complex type
typedef ajz::Abstract_simplex Cell;
typedef ajz::Simplex_boundary< Cell, ajz::Term_Z2> Simplex_boundary;
typedef ajz::Cell_set< Cell, Simplex_boundary, ajz::Nerve_data> Nerve;
typedef Nerve::iterator Nerve_iterator;
typedef ajz::Cover_data< Nerve_iterator > Cover_data;
typedef ajz::Cell_set< Cell, Simplex_boundary, Cover_data> Complex;
typedef Complex::iterator Complex_iterator;
typedef ajz::Cell_less< Complex_iterator> Complex_less;
typedef ajz::Cell_less< Nerve_iterator> Nerve_less;
typedef ajz::Parallel_filtration< Complex, Complex_less> Complex_filtration;
typedef ajz::Parallel_filtration< Nerve, Nerve_less> 
						Nerve_filtration;
typedef Complex_filtration::iterator Complex_filtration_iterator;
typedef Nerve_filtration::iterator Nerve_filtration_iterator;
typedef ajz::Filtration_property_map< Complex_filtration_iterator> 
					Complex_filtration_map;
typedef ajz::Thread_timer Timer;

template< typename Timer>
struct Tri_stats: ajz::Cover_stats< Timer>, 
	      ajz::Blowup_stats< Timer>,
	      ajz::Parallel_stats< Timer>
	     { Timer timer; };
typedef Tri_stats< Timer> Stats;

//Types to build a Blowup complex
typedef ajz::Complex_boundary< Complex, 
			       Complex::iterator> Complex_boundary;

typedef ajz::Complex_boundary< Nerve,
			       Nerve_iterator> Nerve_boundary;

typedef ajz::Iterator_product< Nerve_iterator, 
			       Complex::iterator> Product;

typedef ajz::Product_boundary< Product, Nerve_boundary, 
				        Complex_boundary> Product_boundary;

typedef ajz::Concurrent_cell_set< Product, 
	       	  Product_boundary, 
	       	  ajz::Default_cell_data, 
	       	  Product::Hash1> Blowup;
typedef Blowup::iterator Blowup_iterator;
typedef ajz::Complex_boundary< Blowup, Blowup_iterator> 
	       			Blowup_complex_boundary;
typedef ajz::Parallel_id_less< Blowup::iterator> Parallel_id_less;
typedef ajz::Id_less< Blowup::iterator> Blowup_id_less;
typedef ajz::Cell_less< Blowup::iterator> Blowup_cell_less;
typedef Blowup::iterator Blowup_iterator;
typedef Blowup::Cell_boundary Cell_boundary;
typedef ajz::Parallel_filtration< Blowup, 
	       		   Blowup_id_less> Blowup_filtration;
typedef Blowup_filtration::iterator Blowup_filtration_iterator;
typedef std::vector< int> Betti;

template<typename String, typename Complex>
void read_complex( String & complex_name, Complex & complex){
	std::ifstream in;
	std::cerr << "File IO ..." << std::flush;
	ajz::open_file( in, complex_name.c_str());
	in >> complex;
	ajz::close_file( in);
	std::cerr << "completed!" << std::endl;                     
}

template<typename Variable_map>
void process_args( int & argc, char *argv[],Variable_map & vm){
  //parse command line options
  po::options_description desc( "Usage: tp [options] input-file num-parts");
  desc.add_options()
  ( "help", "Display this message")
  ( "input-file", "input .asc file to parse")
  ( "num-parts", "specify partition size")
  ( "num-threads", po::value<int>()->default_value(-1),
			 "specify number of threads (Defaults to num_covers)");
  po::positional_options_description p;
  p.add( "input-file",1);
  p.add( "num-parts",1);
  po::store( po::command_line_parser( argc,argv)
	    .options( desc)
	    .positional( p)
	    .run(),
	     vm);
  po::notify( vm);
  if ( vm.count( "help")){
	std::cout << desc << std::endl;
	std::exit( 0);
  }
  if ( vm.count( "input-file") != 1 || vm.count( "num-parts") != 1){
	std::cout << desc << std::endl;	
	std::exit( -1);
  }
}

int main( int argc, char *argv[]){
#ifdef ZOOM 
/* Connect to profiling daemon process. */
 ZMError result; int active;
 result = ZMConnect();
 ZM_ASSERT("ZMConnect", result);
#endif
  po::variables_map vm;
  process_args( argc, argv, vm);
  size_t num_parts = atoi( vm[ "num-parts"].as< std::string>().c_str());

  //setup some variables
  std::string full_complex_name = vm[ "input-file"].as< std::string>();
  std::string complex_name( full_complex_name);
  std::string binary_name( argv[ 0]);

  int num_threads = vm[ "num-threads"].as< int>();
  size_t found = complex_name.rfind( '/');
  if ( found != std::string::npos){
        complex_name.replace( 0, found+1, "");
  }
  tbb::task_scheduler_init init( num_threads);
  
  Complex complex;
  Nerve nerve;
  Complex_filtration_map get;
  Stats stats;

  // Read the cell_set in
  read_complex( full_complex_name, complex);

  stats.timer.start();
  Complex_filtration complex_filtration( complex);
  double orig_filtration_time = stats.timer.get();
  stats.timer.start();
  ajz::init_cover_complex( nerve, num_parts);
  bool is_edgecut = ajz::graph_partition_cover( complex_filtration, nerve);
  int num_covers = (is_edgecut) ? num_parts+1 : num_parts; 
  std::size_t blowup_size = 0;
  Nerve_filtration nerve_filtration( nerve);
  for( Nerve_iterator i = nerve.begin(); i != nerve.end(); ++i){
		blowup_size += (i->second.count());
  }
  double cover_time = stats.timer.get();
  std::cerr << "cover computed" << std::endl;
  stats.timer.start();
  Nerve_boundary nerve_boundary( nerve);
  Complex_boundary complex_boundary( complex);
  Product_boundary blowup_cell_boundary( nerve_boundary, complex_boundary);
  Blowup blowup_complex( blowup_cell_boundary);
  //We construct an empty filtration, which we fill out in parallel
  //When we build the complex, with Id's so that Id_less provides a valid
  //filtration order using this structure.
  //We introduce a single O(m) loop, which can be parallelized
  //and made to be O(m/p + p)
  Blowup_filtration blowup_filtration( blowup_complex, 
				       Blowup_id_less(),
				       false, blowup_size);
#ifdef ZOOM
  /*Check if zoom is already sampling - if true, 
  the following ZMStartSession call will fail.*/ 
 result = ZMIsActive(&active);
 ZM_ASSERT("ZMIsActive", result);
 printf("ZMIsActive: %s\n", (active ? "true" : "false"));

 /* Start profiling. */
 result = ZMStartSession();
 ZM_ASSERT("ZMStartSession", result);
#endif
 stats.timer.start();
 ajz::build_blowup_complex( complex_filtration.begin(), 
			    complex_filtration.end(),
			    nerve_filtration,
			    blowup_filtration,
			    get, stats);
 double blowup_time = stats.timer.get();
 std::cerr << "blowup built" << std::endl;

#ifdef ZOOM
  /* Stop profiling. */
  result = ZMStopSession();
  ZM_ASSERT("ZMStopSession", result);

  /* Disconnect from the profiling daemon process. */
 result = ZMDisconnect();
  ZM_ASSERT("ZMDisconnect", result);
#endif

  ajz::do_blowup_homology( blowup_complex,
			   blowup_filtration, 
			   nerve_filtration, 
			   num_covers,
			   stats);

  std::cerr << "homology computed " << std::endl; 
  //END REAL COMPUTATION
  typedef std::vector< double> Size_vector;
  typedef Size_vector::iterator Size_iterator;
  Size_vector sizes( num_covers, 0);
  for( Nerve_filtration_iterator i = nerve_filtration.begin();
				 i < nerve_filtration.begin()+num_covers;
				 ++i){
	sizes[ std::distance( nerve_filtration.begin(), i)] 
						= (*i)->second.count();
  }
  Size_iterator largest = max_element( sizes.begin(), 
       			       	       sizes.begin()+num_covers);
  Size_iterator smallest = min_element( sizes.begin(), 
       					sizes.begin()+num_covers);
  Size_iterator largest_pure = max_element( sizes.begin(), 
       				    	    sizes.begin()+num_parts);
  Size_iterator smallest_pure = min_element( sizes.begin(), 
					     sizes.begin()+num_parts);

 Size_vector part_sizes( num_parts, 0);
 std::size_t edgecut=0;
 std::size_t num_edges=0;
 std::size_t num_vertices=0;
 for(Complex_filtration_iterator i = complex_filtration.begin(); 
				 i != complex_filtration.end(); ++i){
	//count edges
	if ((*i)->first.dimension() == 1) { 
		++num_edges; 
		//are you an impure edge? if so, you are a cut -edge
		if ( (*i)->second.data()->first.dimension() == 0 && 
		     *((*i)->second.data()->first.begin()) == num_parts){
			++edgecut;
		}
	}
	//count vertices
	if( (*i)->first.dimension() == 0){
			++num_vertices;
			++part_sizes[ *((*i)->second.data()->first.begin())];
	}
 }
 Size_iterator largest_partition = max_element( part_sizes.begin(), 
						part_sizes.end());
  

 std::cout.width( 20);
 std::cout << " " << std::flush << std::endl;
 std::cout << "complex_size:\t" << complex.size() << std::endl;
 std::cout << "blowup_size:\t" << blowup_complex.size() << std::endl;
 std::cout << "num_edges:\t" << num_edges << std::endl;
 std::cout << "edgecut:\t" << edgecut;
	#ifdef PERCENTAGES 
	   << " (" << (edgecut/num_edges)*100 << "%)" 
	#endif
 std::cout << std::endl;
 std::cout << "graph_balance_ratio:\t" << (*largest_partition)/num_vertices 
	   << std::endl;
 std::cout << "cover_balance_ratio:\t" 
           << (*largest)/complex.size() << std::endl;
  std::cout << "closed_cover_balance:\t" 
	    << (*largest)/(*smallest) << std::endl;
  std::cout << "pure_cover_balance:  \t" 
	    << (*largest_pure)/(*smallest_pure) << std::endl;
  std::cout << "smallest_cover:      \t" 
	    << std::distance( sizes.begin(), smallest) 
	    << std::endl;
  std::cout << "largest_cover:       \t" << std::right
	    << std::distance( sizes.begin(), largest) 
	    << std::endl;
  std::cout << "blowup_factor:       \t" 
	    << ((double) blowup_complex.size())/ ((double) complex.size())  
	    << std::endl;
 stats.filtration_time = 0;
 double total_time = orig_filtration_time + cover_time + blowup_time + 
		     stats.parallel_persistence;

 std::cout << "sort_complex:       \t" << std::right << orig_filtration_time
	#ifdef PERCENTAGES 
	   << " (" << (orig_filtration_time/total_time)*100 << "%)" 
	#endif
	   << std::endl;
 std::cout << "build_cover:        \t" << cover_time 
	#ifdef PERCENTAGES 
	   << " (" << (cover_time/total_time)*100 << "%)" 
	#endif
	   << std::endl;
 std::cout << "build_blowup:       \t" << blowup_time
	#ifdef PERCENTAGES 
	   << " (" << (blowup_time/total_time)*100 << "%)" 
	#endif
	   << std::endl;
 std::cout << "parallel_homology:  \t" << stats.parallel_persistence 
	#ifdef PERCENTAGES 
	   << " (" << (stats.parallel_persistence/total_time)*100 << "%)" 
	#endif
	   << std::endl;
 std::cout << "total_time:         \t" << total_time 
	#ifdef PERCENTAGES 
	   << " (100%)" 
	#endif
	   << std::endl;

#ifdef TESTS_ON
  ajz::run_tests( complex, blowup_complex, nerve);
#endif
  return 0;
}
