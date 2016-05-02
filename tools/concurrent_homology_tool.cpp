/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************/
//#define COMPUTE_BETTI
//#define TESTS_ON
//#define ZOOM
#ifdef ZOOM
#include "zoom.h"
#endif
//BOOST
#include <boost/program_options.hpp>

//STL
#include <sstream>
#include <vector>

//TBB 
#include <tbb/task_scheduler_init.h> 

#include <ctl/io/io.hpp>
#include <ctl/utility/timer.hpp>
//Chain Complex
#include <ctl/finite_field/finite_field.hpp>
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/cell_complex/complex_boundary.hpp>
#include <ctl/graded_chain_complex/graded_cell_complex.hpp>
#include <ctl/term/term.hpp>

//Parallel Chain Complex
#include <ctl/parallel/chain_complex/chain_complex.hpp>
#include <ctl/parallel/filtration/filtration.hpp>
#include <ctl/parallel/utility/timer.hpp>

//#include "point.h"
//#include "point_vector.h"

//Covers
#include <ctl/parallel/partition_covers/covers.hpp>
#include <ctl/parallel/partition_covers/cover_data.hpp>

//Blowup
#include <ctl/product_cell/iterator_product_boundary.hpp>
#include <ctl/product_cell/iterator_product_cell.hpp>
#include <ctl/product_cell/product_cell_less.hpp>
#include <ctl/parallel/build_blowup_complex/build_blowup_complex.hpp>

//Boundary matrix stuff
#include <ctl/matrix/iterator_property_map.hpp>
#include <ctl/matrix/matrix.hpp>
#include <ctl/matrix/offset_maps.hpp>

//Persistence
#include <ctl/chain/chain.hpp>
#include <ctl/persistence/persistence.hpp>
#include <ctl/persistence/compute_betti.hpp>

//Parallel Homology 
#include <ctl/parallel/homology/persistence.hpp>
#include <ctl/parallel/homology/homology.hpp>

namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex Cell;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary Simplex_boundary;
typedef ctl::Cell_complex< Simplex_boundary, ctl::parallel::Nerve_data> Nerve;
typedef Nerve::iterator Nerve_iterator;
typedef ctl::parallel::Cover_data< Nerve_iterator > Cover_data;
typedef ctl::Cell_complex< Simplex_boundary, Cover_data> Complex;
typedef Complex::iterator Complex_iterator;
typedef ctl::Cell_less Cell_less;
typedef ctl::parallel::Filtration< Complex, Cell_less> Complex_filtration;
typedef ctl::parallel::Filtration< Nerve, Cell_less> 
						Nerve_filtration;
typedef Complex_filtration::iterator Complex_filtration_iterator;
typedef Nerve_filtration::iterator Nerve_filtration_iterator;
typedef ctl::parallel::Filtration_property_map< Complex_filtration_iterator> 
					    Complex_filtration_map;
typedef ctl::parallel::Timer Timer;

template< typename Timer>
struct Tri_stats: ctl::parallel::Cover_stats< Timer>, 
	      ctl::parallel::Blowup_stats< Timer>,
	      ctl::parallel::Parallel_stats< Timer>
	     { Timer timer; };

typedef Tri_stats< Timer> Stats;

//Types to build a Blowup complex
typedef ctl::Complex_boundary< Complex> Complex_boundary;

typedef ctl::Complex_boundary< Nerve> Nerve_boundary;

typedef ctl::Iterator_product_cell< Nerve_iterator,  Complex_iterator> Product;


typedef ctl::Iterator_product_boundary< Product, Nerve_boundary, Complex_boundary> 
							 Product_boundary;

typedef ctl::parallel::Cell_complex< Product_boundary, 
				      ctl::detail::Default_data, 
				      Product::Hash> Blowup;
typedef Blowup::iterator Blowup_iterator;
typedef ctl::Complex_boundary< Blowup, Blowup_iterator> Blowup_complex_boundary;
typedef ctl::Product_first_less < Blowup_iterator> Parallel_id_less;
typedef Blowup::Cell_boundary Cell_boundary;

typedef ctl::Graded_cell_complex< Blowup, ctl::Id_less> Blowup_filtration;
typedef std::vector< int> Betti;


template<typename Variable_map>
void process_args( int & argc, char *argv[],Variable_map & vm){
  //parse command line options
  po::options_description desc( "Usage: concurrent_homology [options] input-file num-parts");
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
  //read_complex( full_complex_name, complex);

  stats.timer.start();
  Complex_filtration complex_filtration( complex);
  stats.timer.stop();
  double orig_filtration_time = stats.timer.elapsed();

  stats.timer.start();
  ctl::parallel::init_cover_complex( nerve, num_parts);
  bool is_edgecut = ctl::parallel::graph_partition_cover( complex_filtration, nerve);
  std::size_t num_covers = (is_edgecut) ? num_parts+1 : num_parts; 
  std::size_t blowup_size = 0;
  Nerve_filtration nerve_filtration( nerve);
  for( Nerve_iterator i = nerve.begin(); i != nerve.end(); ++i){
		blowup_size += (i->second.count());
  }
  stats.timer.stop();
  double cover_time = stats.timer.elapsed();
  stats.timer.start();
  Nerve_boundary nerve_boundary( nerve);
  Complex_boundary complex_boundary( complex);
  Product_boundary blowup_cell_boundary( nerve_boundary, complex_boundary);
  Blowup blowup_complex( blowup_cell_boundary, blowup_size);
  //We construct an empty filtration, which we fill out in parallel
  //When we build the complex, with Id's so that Id_less provides a valid
  //filtration order using this structure.
  //We introduce a single O(m) loop, which can be parallelized
  //and made to be O(m/p + p)
  Blowup_filtration blowup_filtration( blowup_complex, 
				       blowup_size, true);
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
 ctl::parallel::build_blowup_complex( complex_filtration.begin(), 
			              complex_filtration.end(),
			              nerve_filtration,
			              blowup_filtration,
			              get, stats);
 stats.timer.stop();
 double blowup_time = stats.timer.elapsed();

#ifdef ZOOM
  /* Stop profiling. */
  result = ZMStopSession();
  ZM_ASSERT("ZMStopSession", result);

  /* Disconnect from the profiling daemon process. */
 result = ZMDisconnect();
  ZM_ASSERT("ZMDisconnect", result);
#endif

  ctl::parallel::do_blowup_homology( blowup_complex,
			             blowup_filtration, 
			             nerve_filtration, 
			             num_covers,
			             stats);

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
		     stats.initialize_cascade_boundary + stats.parallel_persistence;

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
 std::cout << "initialize_cascade_boundary:  \t" << stats.initialize_cascade_boundary
	#ifdef PERCENTAGES 
	   << " (" << (stats.initialize_cascade_boundary/total_time)*100 << "%)" 
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
  ctl::run_tests( complex, blowup_complex, nerve);
#endif
  return 0;
}
