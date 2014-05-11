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
*******************************************************************************/
//#define COMPUTE_BETTI
//#define CTL_USE_MURMUR
#define CTL_USE_CITY
//#define CTL_USE_JENKINS
//#define COMPLEX_DIAGNOSTICS

#ifdef ZOOM_PROFILE
#include "zoom.h"
#endif

#ifdef COMPLEX_DIAGNOSTICS
	#include "utility/complex_diagnostics.h"
#endif
//CTL 
//Types for Building a Simplicial Chain Complex and Filtration
#include <ctl/finite_field/finite_field.h>
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>
#include <ctl/chain_complex/complex_boundary.h>
#include <ctl/filtration/filtration.h>
#include <ctl/filtration/filtration_boundary.h>
#include <ctl/filtration/less.h>
#include <ctl/io/io.h>
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/chain_complex/chain_complex_io.h>
#include <ctl/term/term.h>

#include <ctl/utility/timer.h>

//Persistence 
#include <ctl/chain/chain.h>
#include <ctl/persistence/persistence.h>
#include <ctl/persistence/iterator_property_map.h>
#include <ctl/persistence/offset_maps.h>
#include <ctl/persistence/compute_betti.h>

//STL
#include <sstream>

//Build Complex type
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_boundary;
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;

//Build Filtration type
typedef	Complex::iterator Complex_iterator;
typedef ctl::Cell_less Complex_cell_less;
typedef ctl::Filtration< Complex, Complex_cell_less > Filtration;
typedef Filtration::iterator Filtration_iterator;
typedef ctl::Filtration_boundary< Filtration> Filtration_boundary;
typedef Filtration::Term Filtration_term;

//Build Chain Type
typedef ctl::Chain< Filtration_term> Complex_chain;

//Type which stores all the chains for persistence
//It would be a good project to see if storing the internal pointer here
//inside of the chain_complex would be a good idea
typedef std::vector< Complex_chain> Complex_chains;

typedef ctl::Pos_offset_map< Filtration_term::Cell> Complex_offset_map;
typedef ctl::iterator_property_map< Complex_chains::iterator, 
                                      Complex_offset_map, 
                                      Complex_chain, 
                                      Complex_chain&> Complex_chain_map;

typedef ctl::Timer Timer;

template<typename String>
void process_args(int & argc, char *argv[], String & filename){
  std::string usage = "Usage: persistence input-file";
  if (argc != 2){ std::cerr << usage << std::endl; exit( 1); }
  filename = std::string( argv[ 1]);
}

int main(int argc, char *argv[]){
  std::string full_complex_name; 
  process_args(argc, argv, full_complex_name);
  
  //create some data structures 
  Complex complex;
  Timer timer;
                                       
  // Read the cell_set in  
  timer.start();
  ctl::read_complex( full_complex_name, complex); 
  timer.stop();

  std::cout << "I/O Time: " << timer.elapsed() << std::endl;
  std::cout << "complex size: " << complex.size() << std::endl; 
  std::cout << "complex dimension: " << complex.dimension() << std::endl;

  //produce a filtration
  timer.start();
  Filtration complex_filtration( complex);
  Filtration_boundary filtration_boundary( complex_filtration);
  timer.stop();
  double complex_filtration_time = timer.elapsed();
  //display some info
  std::cout << "time to compute complex filtration (secs): " 
	    << complex_filtration_time << std::endl;
  
  //begin instantiate our vector of cascades homology
  Complex_chains complex_cascade_boundaries( complex.size(), Complex_chain());
  
  Complex_offset_map offset_map( complex_filtration.begin());
  //we hand persistence a property map for genericity!                                         
  Complex_chain_map cascade_bd_property_map( complex_cascade_boundaries.begin(),
					     offset_map);
  //serial persistence (complex)
  timer.start();
  ctl::persistence( complex_filtration.begin(), complex_filtration.end(),
  		    filtration_boundary, cascade_bd_property_map);
  timer.stop();


  double complex_persistence = timer.elapsed();
  std::cout << "serial persistence: " 
            << complex_persistence << std::endl;
  return 0;
}
