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
* Copyright (C) Ryan H. Lewis 2010 <me@ryanlewis.net>
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

//CTL
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>
#include <ctl/chain_complex/complex_boundary.h>
#include <ctl/parallel/filtration/filtration.h>
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/io/io.h>
#include <ctl/term/term.h>

//Persistence
#include <ctl/chain/chain.h>
#include <ctl/persistence/persistence.h>
#include <ctl/persistence/iterator_property_map.h>
#include <ctl/persistence/offset_maps.h>
#include <ctl/persistence/compute_betti.h>

//Blowup
#include <ctl/product_cell/product_boundary.h>
#include <ctl/product_cell/product_cell.h>
//#include <ctl/blowup_io.h>

//Local Project Deps
#include <ctl/parallel/partition_covers/covers.h>
#include <ctl/parallel/partition_covers/cover_tests.h>
#include <ctl/parallel/partition_covers/cover_data.h>
#include <ctl/parallel/chain_complex/chain_complex.h>
#include <ctl/parallel/build_blowup_complex/build_blowup_complex.h>

//BOOST
#include <boost/program_options.hpp>

//STL
#include <sstream>
#include <iostream>
#include <fstream>

//TBB
#include <tbb/task_scheduler_init.h>

namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex< int> Cell;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Cell, Z2> Simplex_boundary;
typedef ctl::Chain_complex< Cell, Simplex_boundary, 
			    ctl::parallel::Nerve_data> Nerve;
typedef ctl::parallel::Filtration< Nerve, ctl::Cell_less> Nerve_filtration;

typedef Nerve::iterator Nerve_iterator;
typedef ctl::parallel::Cover_data< Nerve_iterator > Cover_data;
typedef ctl::Chain_complex< Cell, Simplex_boundary, Cover_data> Complex;

typedef Complex::iterator Complex_iterator;
typedef Complex::const_iterator Complex_const_iterator;
typedef ctl::Cell_less Complex_less;

typedef ctl::parallel::Filtration< Complex, Complex_less> Complex_filtration;
typedef Complex_filtration::iterator Complex_filtration_iterator;
typedef ctl::parallel::Filtration_property_map< Complex_filtration_iterator> 
					Complex_filtration_map;
typedef ctl::Timer Timer;

template< typename Timer>
struct Dual_stats:
	ctl::parallel::Cover_stats< Timer>,
	ctl::parallel::Blowup_stats< Timer>
	{ Timer timer; };
typedef Dual_stats< Timer> Stats;

//Types to build a Blowup complex
typedef ctl::Complex_boundary< Complex> Complex_boundary;
typedef ctl::Complex_boundary< Nerve> Nerve_boundary;

typedef ctl::Product_cell< Nerve_iterator, 
			       Complex::iterator> Product;

typedef ctl::Product_boundary< Product, 
			       Nerve_boundary, 
			       Complex_boundary> Product_boundary;

typedef ctl::parallel::Chain_complex< Product, 
		       		      Product_boundary, 
		       		      ctl::parallel::Default_data, 
		       		      Product::Hash> Concurrent_blowup;
typedef ctl::Chain_complex< Product, 
		            Product_boundary, 
		            ctl::parallel::Default_data, 
		            Product::Hash> Blowup;

typedef Blowup::iterator Blowup_iterator;
typedef Concurrent_blowup::iterator Concurrent_blowup_iterator;

typedef ctl::Cell_less Blowup_cell_less;
typedef ctl::Cell_less Concurrent_blowup_cell_less;

typedef ctl::parallel::Filtration< Blowup, Blowup_cell_less> Blowup_filtration;
typedef ctl::parallel::Filtration< Concurrent_blowup,
 				  Concurrent_blowup_cell_less> 
				  Concurrent_blowup_filtration;

typedef Blowup_filtration::iterator Blowup_filtration_iterator;
typedef Concurrent_blowup_filtration::iterator 
				Concurrent_blowup_filtration_iterator;
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

/*
template<typename Stats>
void print_out1(Stats & stats){
  //display some timing info
  std::cout << "init time: " << stats.init_time <<  std::endl;
  std::cout << "partition time: " << stats.partition_time <<  std::endl;
  if(stats.open_time != 0){
  std::cout << "\t open time: " << stats.open_time <<  std::endl;
  }
  if(stats.close_time != 0){
  std::cout << "\t close time: " << stats.close_time <<  std::endl;
  }
  std::cout << "covering time: " << stats.cover_time <<  std::endl;
  std::cout << "total cover time: " << 
		stats.init_time + stats.partition_time + stats.cover_time
	    << std::endl;
}

template<typename Stats>
void print_out2(Stats & stats){
  //std::cout << "time to build blowup: " << stats.blowup_time << std::endl;
  std::cout << "blowup ratio: " << stats.blowup_ratio << std::endl;
  std::cout << "complex size: " << stats.complex_size << std::endl;
  std::cout << "blowup size: " << stats.blowup_size << std::endl;
}

template<typename Out, typename Map>
void hash_stats(Out & out, Map & dist, double & load){
	typedef typename ctl::Cell_less< typename Map::iterator> Less;
 	typedef typename ctl::Parallel_filtration< Map, Less> Ordering;
	typedef typename Ordering::iterator Iterator;
	std::cout << "Load Factor: " << load << std::endl;
	std::cout << "Num Buckets: " << dist.size() << std::endl;
	Ordering order( dist);
	for(Iterator i = order.begin(); i != order.end(); ++i){
		      out << (*i)->first 
			  << " \t " 
			  << (*i)->second 
			  << std::endl;
	}
}*/



int main( int argc, char *argv[]){
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

  Complex complex;
  Nerve nerve;
  Stats stats;
  // Read the chain_complex in
  ctl::read_complex( full_complex_name, complex);
  if (num_threads != -1){
  	tbb::task_scheduler_init init( num_threads);
  }else{
  	tbb::task_scheduler_init init;
  }
  
  stats.timer.start();
  Complex_filtration complex_filtration( complex);
  stats.timer.stop();
  double filtration_time = stats.timer.elapsed();
  std::cout << "time to filter complex: " << filtration_time << std::endl;

  stats.timer.start();
  ctl::parallel::init_cover_complex( nerve, num_parts);
  ctl::parallel::graph_partition_cover( complex_filtration, nerve);
  std::size_t blowup_size = 0;
  for( Nerve_iterator i = nerve.begin(); i != nerve.end(); ++i){
                blowup_size += (i->second.count());
  }
  stats.timer.stop();
  double cover_time = stats.timer.elapsed();
  std::cout << "cover time: " << cover_time << std::endl;

  Complex_boundary complex_boundary( complex);
  Nerve_boundary nerve_boundary( nerve);
  Nerve_filtration nerve_filtration( nerve);
  Product_boundary blowup_cell_boundary( nerve_boundary, complex_boundary);

  Complex_filtration_iterator filtration_begin = complex_filtration.begin();
  Complex_filtration_iterator filtration_end = complex_filtration.end();
  Complex_filtration_map get;
  Concurrent_blowup blowup_complex( blowup_cell_boundary, blowup_size);
  Concurrent_blowup_filtration blowup_filtration( blowup_complex, blowup_size, true);
  std::cout << "begin parallel build blowup" << std::endl;
  
  stats.timer.start();
  ctl::parallel::build_blowup_complex( filtration_begin, filtration_end, 
				       nerve_filtration, blowup_filtration, get);
  stats.timer.stop();
  double blowup_time = stats.timer.elapsed();

  std::cout << "parallel_build_blowup_complex " << blowup_time << std::endl;
  std::cout << "-----------------" << std::endl;
  std::cout << "-----------------" << std::endl;
  /*
  std::cout << "Serial complex" << std::endl;
  stats.timer.start();
  ctl::serial_build_blowup_complex( filtration_begin, 
			     	    filtration_end, 
			     	    serial_complex,
			    	    get);
  double serial_blowup_time = stats.timer.get(); 
  std::cout << "serial_build_blowup_complex " 
	    << serial_blowup_time << std::endl;
  Concurrent_blowup_filtration f1( blowup_complex);
  Blowup_filtration f2( serial_complex);
  Blowup_filtration_iterator j;
  j = f2.begin();
*/
#ifdef HASH_TESTS
  typedef std::map< size_t, size_t> Dist;
  Dist serial_count;
  Dist parallel_count;
  double serial_load = serial_complex.bucket_distribution( serial_count); 
  double parallel_load = blowup_complex.bucket_distribution( parallel_count); 
  std::ofstream boostf( "boost.hash");
  std::ofstream tbbf( "tbb.hash");
  hash_stats( boostf, serial_count, serial_load);
  hash_stats( tbbf, parallel_count, parallel_load); 
#endif
  //ctl::run_tests( complex, blowup_complex, nerve);
  return 0;
}
