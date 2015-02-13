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
//CTL
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/chain_complex/complex_boundary.h>
#include <ctl/parallel/filtration/filtration.h>
#include <ctl/io/io.h>
#include <ctl/term/term.h>
#include <ctl/utility/timer.h>

#include <ctl/parallel/partition_covers/covers.h>
#include <ctl/parallel/partition_covers/cover_data.h>

//BOOST
#include <boost/program_options.hpp>

//STL
#include <sstream>

//TBB
#include <tbb/task_scheduler_init.h>


namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex<int> Cell;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Cell, Z2> Simplex_boundary;
typedef ctl::Chain_complex< Cell, 
			    Simplex_boundary, 
			    ctl::parallel::Nerve_data> Nerve;
typedef Nerve::iterator Nerve_iterator;
typedef ctl::parallel::Cover_data< Nerve_iterator > Cover_data;
typedef ctl::Chain_complex< Cell, Simplex_boundary, Cover_data> Complex;
typedef Complex::iterator Complex_iterator;
typedef ctl::Cell_less Cell_less;
typedef ctl::parallel::Filtration< Complex, Cell_less> Filtration;

typedef ctl::Timer Timer;
typedef ctl::parallel::Cover_stats< Timer> Stats;

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

  // Read the cell_set in
  read_complex( full_complex_name, complex);
  if (!complex.is_closed()){
	  std::cout << "complex is not closed" << std::endl;
  }
  Filtration filtration( complex);
  Stats stats;
  Timer timer; 
  timer.start();
  ctl::parallel::init_cover_complex( nerve, num_parts);
  ctl::parallel::graph_partition_cover( filtration, nerve);
  timer.stop();
  double cover_time = timer.elapsed();
  std::cout << "cover compute time: " << cover_time << std::endl;
  std::string cover_name ( full_complex_name);
  std::string nerve_name ( full_complex_name);

  found = cover_name.rfind( '.');
  if (found == std::string::npos){
	cover_name.append(".cov");
  }else{
	cover_name.replace(found,cover_name.size()-1,".cov");
  }

  found = nerve_name.rfind( '.');
  if (found == std::string::npos){
	nerve_name.append(".nrv");
  }else{
	nerve_name.replace(found,nerve_name.size()-1,".nrv");
  }

  std::ofstream out; 
  ctl::open_file( out, nerve_name.c_str());
  out << nerve;
  ctl::close_file( out);

  ctl::open_file( out, cover_name.c_str());
  typedef ctl::parallel::Get_cover< Complex::Data> Cover_functor;
  complex.write( out, Cover_functor());
  ctl::close_file( out);
  return 0;
}
