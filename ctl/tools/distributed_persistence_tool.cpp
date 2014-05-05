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
//BOOST
#include <boost/program_options.hpp>
#include <boost/mpi.hpp>

//STL
#include <sstream>
#include <vector>

//CTL
//Utility
/*
#include <ctl/distributed/io/io.h>
#include <ctl/distributed/utility/timer.h>
*/

//Chain Complex
#include <ctl/finite_field/finite_field.h>
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/chain_complex/complex_boundary.h>
#include <ctl/filtration/filtration.h>
#include <ctl/term/term.h>

//Distributed Data
#include <ctl/distributed/cover_data.h>

/*
//Blowup
#include <ctl/product_cell/product_boundary.h>
#include <ctl/product_cell/product_cell.h>
#include <ctl/parallel/build_blowup_complex/build_blowup_complex.h>

//Persistence 
#include <ctl/chain/chain.h>
#include <ctl/persistence/persistence.h>
#include <ctl/persistence/iterator_property_map.h>
#include <ctl/persistence/offset_maps.h>
#include <ctl/persistence/compute_betti.h>
*/

//Namespaces
namespace mpi = boost::mpi;
namespace po = boost::program_options;


// Simplex and Boundary Type
typedef ctl::Abstract_simplex< int> Cell;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Cell, Z2> Simplex_boundary;
typedef ctl::Cell_less Cell_less;

//Nerve & Filtration
typedef ctl::Chain_complex< Cell, Simplex_boundary> Nerve;
typedef ctl::Complex_boundary< Nerve> Nerve_boundary;
typedef Nerve::iterator Nerve_iterator;
typedef ctl::distributed::Cover_data< Nerve_iterator > Cover_data;
typedef ctl::Filtration< Nerve, Cell_less>  Nerve_filtration;
typedef Nerve_filtration::iterator Nerve_filtration_iterator;

//Complex & Filtration
typedef ctl::Chain_complex< Cell, Simplex_boundary, Cover_data> Complex;
typedef ctl::Complex_boundary< Complex> Complex_boundary;
typedef Complex::iterator Complex_iterator;
typedef ctl::Filtration< Complex, Cell_less> Complex_filtration;
typedef Complex_filtration::iterator Complex_filtration_iterator;

/*
//Utility Types //TODO: Implement me
typedef ctl::distributed::Timer Timer;

//Product Cell & Boundary
typedef ctl::distributed::Product_cell< Nerve_filtration_iterator,  Complex_filtration_iterator> Product;
typedef ctl::distributed::Product_boundary< Product, Complex_filtration_boundary, Nerve_filtration_boundary> Product_boundary;

//Blowup Complex
typedef ctl::Chain_complex< Product, Product_boundary, ctl::Default_data, Product::Hash> Blowup;
typedef ctl::Complex_filtration_boundary< Blowup> Blowup_complex_boundary;
typedef ctl::parallel::Product_first_less < Blowup_iterator> Blowup_less;

//Blowup Filtration
typedef ctl::Filtration< Blowup, Blowup_less> Blowup_filtration;
typedef Blowup_filtration::iterator Blowup_filtration_iterator;
*/

template<typename Variable_map>
void process_args( int & argc, char *argv[],Variable_map & vm){
  //parse command line options
  po::options_description desc( "Usage: distributed_homology [options] input-file");
  desc.add_options()
  ( "help", "Display this message")
  ( "input-file", "input .asc file to parse");
  po::positional_options_description p;
  p.add( "input-file",1);
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
  if ( vm.count( "input-file") != 1){
	std::cout << desc << std::endl;	
	std::exit( -1);
  }
}

int main( int argc, char *argv[]){
  /** mpi::threading::level choices 
   single     = MPI_THREAD_SINGLE,
  -----------------------------------
   Only one thread will execute. 
   
   funneled   = MPI_THREAD_FUNNELED,
  -----------------------------------
   Only main thread will do MPI calls.
   The process may be multi-threaded, but only the main 
   thread will make MPI calls (all MPI calls are ``funneled''
   to the main thread). 

  serialized = MPI_THREAD_SERIALIZED,
  -----------------------------------
  Only one thread at the time do MPI calls.
  The process may be multi-threaded, and multiple 
  threads may make MPI calls, but only one at a time:
  MPI calls are not made concurrently from two distinct 
  threads (all MPI calls are ``serialized'').

  multiple   = MPI_THREAD_MULTIPLE
  -----------------------------------
  Multiple threads may call MPI, with no restrictions.
   */
  mpi::environment env(argc, argv);
  mpi::communicator world;
  po::variables_map vm;
  process_args( argc, argv, vm);

  //setup some variables
  std::string full_complex_name = vm[ "input-file"].as< std::string>();
  std::string complex_name( full_complex_name);
  std::string binary_name( argv[ 0]);
  size_t found = complex_name.rfind( '/');
  if ( found != std::string::npos){
        complex_name.replace( 0, found+1, "");
  }
  
  Complex complex;
  Nerve nerve;
  // Read the relevant cells of the complex into memory
  // all cells of K such that \sigma \in { ... my_rank() ... } \in nerve
  timer.start();
  //TODO: implement this
  ctl::distributed::read_complex( full_complex_name, complex, nerve, world);
  timer.stop();
  double io_time = timer.elapsed();

  /*
  timer.start();
  Complex_filtration complex_filtration( complex);
  Nerve_filtration nerve_filtration( nerve);
  std::size_t blowup_size = 0;
  for( Nerve_iterator i = nerve.begin(); i != nerve.end(); ++i){ blowup_size += (i->second.count()); }
  timer.stop();
  double filtration_time = timer.elapsed();

  timer.start();
  Nerve_boundary nerve_boundary( nerve);
  Complex_boundary complex_boundary( complex);
  Product_boundary blowup_cell_boundary( nerve_boundary, complex_boundary);
  Blowup blowup_complex( blowup_cell_boundary, blowup_size);
  //We construct an empty filtration, which we fill out in parallel
  //When we build the complex, with Id's so that Id_less provides a valid
  //filtration order using this structure.
  //We introduce a single O(m) loop, which can be parallelized
  //and made to be O(m/p + p)
  Blowup_filtration blowup_filtration( blowup_complex, blowup_size, true);

  Complex_filtration_map get;
  timer.start();
  ctl::distributed::build_blowup( complex_filtration, nerve_filtration, blowup_filtration, get);
  timer.stop();
  double blowup_time = timer.elapsed();

  ctl::distributed::persistence( blowup_complex, blowup_filtration, nerve_filtration, world);

#ifdef TESTS_ON
//  ctl::distributed::run_tests( complex, blowup_complex, nerve);
#endif
*/
  return 0;
}
