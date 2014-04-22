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

//Local Project Deps
#include "covers.h"
#include "cover_tests.h"

//BOOST
#include <boost/program_options.hpp>

//STL
#include <sstream>

//TBB
#include "tbb/task_scheduler_init.h"


namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex Cell;
typedef ctl::Simplex_boundary< Cell, ctl::Term_Z2> Simplex_boundary;
typedef ctl::Cell_set< Cell, Simplex_boundary, ctl::Nerve_data> Cover_complex;

typedef Cover_complex::iterator Cover_complex_iterator;
typedef ctl::Cover_data< Cover_complex_iterator > Cover_data;
typedef ctl::Cell_set< Cell, Simplex_boundary, Cover_data> Complex;
typedef Complex::iterator Complex_iterator;
typedef ctl::Cell_less< Complex_iterator> Cell_less;
typedef ctl::Cell_less< Cover_complex_iterator> Nerve_less;
typedef ctl::Parallel_filtration< Complex, Cell_less> Filtration;
typedef ctl::Parallel_filtration< Cover_complex, Nerve_less> Nerve_filtration;
typedef ctl::Thread_timer Timer;
typedef ctl::Cover_stats< Timer> Stats;

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
  ( "num-parts", "specify partition size");
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
  tbb::task_scheduler_init init;
  //setup some variables
  std::string full_complex_name = vm[ "input-file"].as< std::string>();
  std::string complex_name( full_complex_name);
  std::string binary_name( argv[ 0]);

  size_t found = complex_name.rfind( '/');
  if ( found != std::string::npos){
        complex_name.replace( 0, found+1, "");
  }

  Complex complex;
  Cover_complex nerve;

  // Read the cell_set in
  read_complex( full_complex_name, complex);
  if (!complex.is_closed()){
	  std::cout << "complex is not closed" << std::endl;
  }
  Filtration filtration( complex);
  Stats stats;
  Timer timer; 
  timer.start();
  ctl::init_cover_complex( nerve, num_parts);
  ctl::graph_partition_cover( filtration, nerve);
  double cover_time = timer.get();
  //display some timing info
  std::cout << "total time: " << cover_time <<  std::endl;
  Nerve_filtration ordered_nerve( nerve);
  typedef Nerve_filtration::iterator Iterator;
  for(Iterator i = ordered_nerve.begin(); i != ordered_nerve.end(); ++i){
 	std::cout << (*i)->first << ": " << (*i)->second.count() << std::endl;
  }
 //ctl::run_tests( complex);
  return 0;
}
