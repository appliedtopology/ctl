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
// STL
#include <string>
#include <sstream>
#include <fstream>

// Project
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/io/io.h>

typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_Boundary;
typedef ctl::Chain_complex< Simplex, Simplex_Boundary> Complex;

void usage( const char* argv){
    std::cerr << "Usage:  " 
	      << argv << " input_name num_duplicates (=2) dim_of_connection=[no connection = default, 1..d specifies a cell of that dimension] " << std::endl;
    std::exit(1);
} 

int main(int argc, char *argv[]) {
  if( argc < 2) { usage( argv[ 0]); }
  std::size_t num=2;
  if(argc==3 || argc == 4){
	num=atoi(argv[ 2]);
	if (num < 2){ usage( argv[ 0]); }
  }
  int flag = -1;
  if( argc == 4){
	flag = atoi( argv[ 3]);
  }
  //initial stuff
  Complex input_complex, output_complex;
  std::ifstream in;
  std::string basename( argv[ 1]);
  ctl::open_file(in, basename.c_str());
  in >> input_complex;
  std::cout << "original complex size: " 
	    << input_complex.size() << std::endl; 
  output_complex = input_complex;
  size_t found = basename.rfind('.');
  if (found != std::string::npos){
	basename.replace( found, basename.size()-1, "");
  }

  typedef typename Complex::Cell Cell;
  typedef typename Cell::value_type Value;
  //compute num_vertices
  output_complex.reserve( (num-1)*input_complex.size());
  Value max_vertex_name= *(input_complex.begin()->first.rbegin());
  std::size_t num_vertices=0;
  for(auto cell: input_complex){
	//simplices are stored sorted
	typename Complex::Cell::value_type cur = *(cell.first.rbegin()); 
	if(cell.first.dimension() == 0){ 
		if( max_vertex_name < cur){ 
			max_vertex_name = cur; 
		}
		++num_vertices;
	}
  }
  std::cout << "flag: " << flag << std::endl;
  std::cout << "num_vertices: " << num_vertices << std::endl;
  max_vertex_name++;
  for ( auto i : input_complex){  
	for(std::size_t j = 1; j < num; ++j){
	    Cell cell = i.first;
	    for( auto& vertex_name : cell){ vertex_name+=max_vertex_name*j; }
	    output_complex.insert_open_cell( cell);
	}
  }
  if (flag > 0 && flag <= num_vertices) { 
	for( int i=0; i < (int)num-1; ++i){
		Cell cell = { i*max_vertex_name, (i+1)*max_vertex_name};
		for (int j = 2; j < flag+1; ++j){
			cell.insert( (i+1)*max_vertex_name + (j-1) );
		}
		std::cout << "inserting: " << cell << std::endl;
	        output_complex.insert_closed_cell( cell);
	}
  }
  std::string name( basename);
  std::stringstream ss;
  ss << "." << num << ".asc";
  name.append(ss.str());
  std::cout << "writing complex: " << name;
  std::cout << " of size: " << output_complex.size() << std::endl;
  std::ofstream out;
  ctl::open_file( out, name.c_str());
  output_complex.write( out);
  out.close();
 return 0;
}
