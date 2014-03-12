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
#include <fstream>

typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_Boundary;
typedef ctl::Chain_complex< Simplex, Simplex_Boundary> Complex;

int main(int argc, char *argv[]) {
  std::size_t num=2;

  if( argc < 2) {
    std::cerr << "Usage:  " 
	      << argv[ 0] << " input_name num_duplicates (=2)" << std::endl;
    std::exit(1);
  }

  if(argc==3){
	num=atoi(argv[ 2]);
	if (num < 2){
	 std::cerr << "Usage:  " 
		   << argv[ 0] 
		   << " input_name num_duplicates (=2)" 
		   << std::endl;
	 std::exit(-1);
	}
  }
  
  //initial stuff
  Complex input_complex, output_complex;
  std::ifstream in;
  std::string basename( argv[ 1]);
  ctl::open_file(in, basename.c_str());
  in >> input_complex;
  std::cout << "original complex size: " << input_complex.size() << std::endl; 
  output_complex = input_complex;
  size_t found = basename.rfind('.');
  if (found != std::string::npos){
	basename.replace( found, basename.size()-1, "");
  }

  typedef typename Complex::Cell Cell;
  typedef typename Cell::value_type Value;
  //compute num_vertices
  output_complex.reserve( (num-1)*input_complex.size());
  Value max_vertex_name= *(input_complex.begin()->first.begin());
  for(auto cell: input_complex){
	//simplices are stored sorted
	typename Complex::Cell::value_type cur = *(cell.first.rbegin()); 
	if(cell.first.dimension() == 0 && 
		max_vertex_name < cur){ max_vertex_name = cur; }
  }
  for ( auto i : input_complex){  
	for(int j = 1; j < num; ++j){
	    Cell cell = i.first;
	    for( auto vertex_name : cell){ vertex_name+=max_vertex_name*j; }
	    output_complex.insert_open_cell( cell);
	}
  }
  std::string name( basename);
  std::stringstream ss;
  ss << "." << num << ".asc";
  name.append(ss.str());

  std::ofstream out;
  ctl::open_file( out, name.c_str());
  output_complex.write( out);
  out.close();
 return 0;
}
