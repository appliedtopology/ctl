/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************/
//#define COMPUTE_BETTI
//#define TESTS_ON
#define CTL_USE_CITY
//#define ZOOM_PROFILE
#ifdef ZOOM_PROFILE
#include "zoom.h"
#endif

//#define TESTS_ON
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

//Persistence 
#include <ctl/chain/chain.hpp>
#include <ctl/persistence/compute_betti.hpp>
#include <ctl/persistence/persistence.hpp>
#include <ctl/matrix/iterator_property_map.hpp>
#include <ctl/matrix/offset_maps.hpp>
#include <ctl/persistence/compute_betti.hpp>

//Parallel Homology 
#include <ctl/parallel/homology/persistence.hpp>
#include <ctl/parallel/homology/homology.hpp>

#include <ctl/parallel/build_blowup_complex/build_blowup_complex.hpp>

namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex Cell;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary Simplex_boundary;
typedef ctl::Cell_complex< Simplex_boundary,  ctl::parallel::Nerve_data> Nerve;
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
struct Dual_stats: ctl::parallel::Cover_stats< Timer>, 
	      ctl::parallel::Parallel_stats< Timer>
	     { Timer timer; };
typedef Dual_stats< Timer> Stats;

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
  #ifdef ZOOM_PROFILE
  std::cout << "Connect" <<  ZMConnect() << std::endl;
  #endif
 
  po::variables_map vm;
  process_args( argc, argv, vm);
  size_t num_parts = atoi( vm[ "num-parts"].as< std::string>().c_str());

  //setup some variables
  std::string full_complex_name = vm[ "input-file"].as< std::string>();
  std::string complex_name( full_complex_name);
  std::string binary_name( argv[ 0]);

  size_t found = complex_name.rfind( '/');
  if ( found != std::string::npos){
        complex_name.replace( 0, found+1, "");
  }
  tbb::task_scheduler_init init;
  
  Complex complex;
  Nerve nerve;
  tbb::concurrent_vector< Complex_iterator> nearly_pure;
  Stats stats;

  // Read the cell_set in
  //read_complex( full_complex_name, complex);
  stats.timer.start();
  Complex_filtration complex_filtration( complex);
  stats.timer.stop();
  double orig_filtration_time = stats.timer.elapsed();
  std::cout << "filtered complex " << orig_filtration_time << std::endl;
  stats.timer.start();
  ctl::parallel::init_cover_complex( nerve, num_parts);
  ctl::parallel::graph_partition_open_cover( complex_filtration, nerve, nearly_pure);
  stats.timer.stop();
  double cover_time = stats.timer.elapsed();
  std::cout << "built cover." << std::endl;
  #ifdef ZOOM_PROFILE
  std::cout << "Profiling Begin";
  ZMError start_error = ZMStartSession();
  std::cout << start_error << std::endl;
  #endif 
 
  ctl::parallel::compute_homology( complex, nerve, num_parts, stats);
  #ifdef ZOOM_PROFILE
  ZMError end_error = ZMStopSession();
  std::cout << "Profiling End" << end_error << std::endl;
  #endif
  #ifdef ZOOM_PROFILE
   std::cout << "Disconnect" <<  ZMDisconnect() << std::endl;
  #endif
  double total_time = cover_time + stats.filtration_time + stats.get_iterators +
         	     stats.parallel_persistence + stats.initialize_cascade_boundary;
                  
  std::cout << std::setprecision( 2) << std::fixed;
                                                                                
  std::cout << "build cover: " << cover_time 
            << " (" << (cover_time/total_time)*100 << "%)" << std::endl;
                                                                                
  std::cout << "re-filter complex: " << stats.filtration_time 
            << " (" << (stats.filtration_time/total_time)*100 << "%)" 
            << std::endl;
  std::cout << "compute parallel ranges: " << stats.get_iterators
            << " (" << (stats.get_iterators/total_time)*100 << "%)" 
	    << std::endl;
  std::cout << "initialize_cascade_boundary: " 
            << stats.initialize_cascade_boundary << " (" 
            << (stats.initialize_cascade_boundary/total_time)*100 << "%)" << std::endl;
  std::cout << "parallel_homology: " 
            << stats.parallel_persistence << " (" 
            << (stats.parallel_persistence/total_time)*100 << "%)" << std::endl;
  std::cout << "total time: " << total_time << " (100%)" << std::endl;

#ifdef TESTS_ON
  ctl::run_tests( complex, blowup_complex, nerve);
#endif
  init.terminate();
  return 0;
}
