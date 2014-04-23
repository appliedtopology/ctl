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
#include "parallel_filtration.h"
#include "cell_set.h"
#include "io.h" 
#include "term.h"
#include "timer.h"
#include "thread_timer.h"
#include "point.h"
#include "point_vector.h"
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

#include <tbb/task_scheduler_init.h> 

namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex Cell;
typedef ctl::Simplex_boundary< Cell, ctl::Term_Z2> Simplex_boundary;
typedef ctl::Cell_set< Cell, Simplex_boundary, ctl::Nerve_data> Cover_complex;
typedef Cover_complex::iterator Cover_complex_iterator;
typedef ctl::Cover_data< Cover_complex_iterator > Cover_data;
typedef ctl::Cell_set< Cell, Simplex_boundary, Cover_data> Complex;
typedef Complex::iterator Complex_iterator;
typedef ctl::Cell_less< Complex_iterator> Complex_less;
typedef ctl::Parallel_filtration< Complex, Complex_less> Complex_filtration;
typedef Complex_filtration::iterator Complex_filtration_iterator;
typedef ctl::Thread_timer Timer;

template< typename Timer>
struct Dual_stats: ctl::Cover_stats< Timer>, 
	      ctl::Parallel_stats< Timer>
	     { Timer timer; };
typedef Dual_stats< Timer> Stats;

template<typename String, typename Complex>
void read_complex( String & complex_name, Complex & complex){
	std::ifstream in;
	std::cout << "File IO ..." << std::flush;
	ctl::open_file( in, complex_name.c_str());
	in >> complex;
	ctl::close_file( in);
	std::cout << "completed!" << std::endl;                     
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
  Cover_complex nerve;
  tbb::concurrent_vector< Complex_iterator> nearly_pure;
  Stats stats;

  // Read the cell_set in
  read_complex( full_complex_name, complex);

  stats.timer.start();
  Complex_filtration complex_filtration( complex);
  double orig_filtration_time = stats.timer.get();

  stats.timer.start();
  ctl::init_cover_complex( nerve, num_parts);
  ctl::graph_partition_open_cover( complex_filtration, nerve, nearly_pure);
  double cover_time = stats.timer.get();
  ctl::compute_homology( complex, num_parts, stats);

  double total_time = cover_time + stats.filtration_time + stats.get_iterators +
         	     stats.parallel_persistence;
                                                                                
  std::cout << std::setprecision( 2) << std::fixed;
                                                                                
  std::cout << "filter complex: " << orig_filtration_time << std::endl;
                                                                                
  std::cout << "build cover: " << cover_time 
            << " (" << (cover_time/total_time)*100 << "%)" << std::endl;
                                                                                
  std::cout << "re-filter complex: " << stats.filtration_time 
            << " (" << (stats.filtration_time/total_time)*100 << "%)" 
            << std::endl;
 /*
  std::cout << "compute parallel ranges: " << stats.get_iterators
            << " (" << (stats.get_iterators/total_time)*100 << "%)" 
	    << std::endl;
  */
                                                                             
  std::cout << "time for parallel_homology: " 
            << stats.parallel_persistence << " (" 
            << (stats.parallel_persistence/total_time)*100 << "%)" << std::endl;
  std::cout << "total time: " << total_time << " (100%)" << std::endl;

#ifdef TESTS_ON
  ctl::run_tests( complex, blowup_complex, nerve);
#endif
  init.terminate();
  return 0;
}
