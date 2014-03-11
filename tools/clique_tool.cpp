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
// build_blobs.cpp
// Ryan Lewis
// July 8, 2011

// Project
#include "abstract_simplex/abstract_simplex.h"
#include "chain_complex/chain_complex.h"
#include "chain_complex/chain_complex_io.h"
#include "io/io.h"
// STL
#include <string>
#include <sstream>

typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_Boundary;
typedef ctl::Chain_complex< Simplex, Simplex_Boundary> Complex;

int main(int argc, char *argv[]) {
  if( argc < 2) {
   std::cerr << "Usage:  " << argv[ 0] << " filename num_vertices" << std::endl;
   std::exit( 1);
  }
  //initial stuff
  Complex complex;
  std::string basename(argv[ 1]);
  basename.append(".").append(argv[ 2]).append(".asc");
  std::size_t n = atoi( argv[ 2]);
  Simplex cell;
  for( std::size_t i = 0; i < n; ++i) { cell.insert(i); }
  std::cout << "building VR complex on: " << cell << std::endl;
  complex.insert_closed_cell(cell,true);
  std::cout << "complex has: " << complex.size() << " simplices" << std::endl;
  std::ofstream out;
  ctl::open_file(out, basename.c_str());
  complex.write( out);
  ctl::close_file( out);
  return 0;
}
