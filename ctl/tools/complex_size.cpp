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
#include <ctl/io/io.h>
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/chain_complex/chain_complex_io.h>

#include <ctl/utility/timer.h>

//STL
#include <sstream>

//Build Complex type
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_boundary;
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;

template<typename String>
void process_args(int & argc, char *argv[], String & filename){
  std::string usage = "Usage: complex_size input-file";
  if (argc != 2){ std::cerr << usage << std::endl; exit( 1); }
  filename = std::string( argv[ 1]);
}

int main(int argc, char *argv[]){
  std::string full_complex_name; 
  process_args(argc, argv, full_complex_name);
  
  //create some data structures 
  Complex complex;
  ctl::Timer timer;
                                       
  // Read the cell_set in  
  timer.start();
  ctl::read_complex( full_complex_name, complex); 
  timer.stop();
  std::cout << "I/O Time: " << timer.elapsed() << std::endl;
  std::cout << "number of simplices: " << complex.size() << std::endl; 
  std::cout << "complex dimension: " << complex.dimension() << std::endl;
  typedef std::vector< std::size_t> Vector;
  Vector counts( complex.dimension()+1, 0);
  typedef typename Complex::Cell Cell;
  typedef typename Cell::value_type Vertex_type;
  
  std::size_t complex_bytes=sizeof( Cell)*complex.size() + sizeof( Complex); 
  for( const auto & cell : complex){
	complex_bytes += sizeof( Vertex_type)*cell.first.capacity();
	counts[ cell.first.dimension()]++;
  }
  std::cout << "Cells by dimension: " << std::endl;
  for( const auto size : counts){ std::cout << size << std::endl; }
  std::cout << "Rough estimate of memory used: " 
	    << complex_bytes << " (bytes)" << std::endl; 
  return 0;
}
