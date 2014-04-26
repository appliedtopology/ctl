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



//STL
#include <iostream>

//Project
#include <ctl/one_skeleton/one_skeleton.h>

typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2; 
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_boundary;
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;

int main(int argc, char *argv[])
{
  if( argc < 2) {
    std::cerr << "Usage:  " << argv[0] << " complex" << std::endl;
    std::exit(1);
  }

  // Read in complex
  std::ifstream in;
  Complex complex;
  ctl::open_file( in, argv[ 1]);
  in >> complex;
  ctl::close_file( in);

  // Types for METIS CSR & map
  typedef Complex::Cell Simplex;
  typedef Simplex::vertex_type vertex_type;
  typedef std::vector< unsigned int> Vector;
  std::vector< vertex_type> index_to_vertex_map;

  // Results
  Vector neighbors, offsets;

  // Extract out 1-skeleton in CSR format
  one_skeleton( complex, neighbors, offsets, index_to_vertex_map);

  return EXIT_SUCCESS;
}

