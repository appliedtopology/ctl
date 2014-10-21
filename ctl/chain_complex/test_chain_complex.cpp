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
//STL
#include <iostream>

//CTL
//abstract_simplex
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>
//cube
#include <ctl/cube/cube.h>
#include <ctl/cube/cube_boundary.h>

//chain_complex
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/chain_complex/complex_boundary.h>

//We build a simplicial chain complex with Z2 coefficients

typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 3> Z3;
typedef ctl::Simplex_boundary< Simplex, Z3 > Simplex_boundary;
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Simplicial_complex;
typedef Simplicial_complex::Cell Cell;
typedef ctl::Complex_boundary< Simplicial_complex> Simplicial_complex_boundary;
typedef Simplicial_complex_boundary::const_iterator 
						  simplicial_boundary_iterator;


typedef ctl::Cube< int> Cube;
typedef ctl::Cube_boundary< Cube, Z3 > Cube_boundary;
typedef ctl::Chain_complex< Cube, Cube_boundary> Cubical_complex;
typedef Cubical_complex::Cell_boundary Cubical_cell_boundary;
typedef ctl::Complex_boundary< Cubical_complex> Cubical_complex_boundary;
typedef Cubical_complex_boundary::const_iterator cubical_boundary_iterator;

int main( int argc, char** argv){
	Simplicial_complex complex;
	Cell s( {1,2,3,4} );
	auto pair = complex.insert_closed_cell( s);
	std::cout << pair.second << " cells inserted!" << std::endl;
	std::cout << "complex is " << 
		((complex.is_closed())? "closed":"not closed") << std::endl; 
	std::cout << complex << std::endl;

	Simplicial_complex_boundary b( complex);
	Simplicial_complex_boundary copyd( b);
	Simplicial_complex_boundary moved( std::move( copyd));
	for( auto i = complex.begin(); i != complex.end(); ++i){
	std::cout << "boundary test: " << ctl::delta << "("  
		  << i->first << ")" << std::endl;
	std::cout << "boundary length: " << b.length( i) << std::endl;
	for( simplicial_boundary_iterator j = b.begin( i); j != b.end( i); ++j){
		std::cout << j->cell()->first << " ";
	}
	std::cout << std::endl;
	}
	std::ofstream out( "test.asc");
	complex.write( out); 
	std::cout << " ---------------------- " << std::endl;
	std::cout << " testing cubical complexes " << std::endl;
	std::vector< std::size_t> sizes{4,5};
	std::vector< std::size_t> start{3,97};
	Cubical_complex complex1( sizes, start);
	Cubical_complex c2( complex1);
	if( c2 != complex1) { std::cerr << "bug in copy/equality operators."; }
	Cubical_cell_boundary cube_cell_bd = complex1.cell_boundary();
	Cubical_complex_boundary c( complex1);
	std::cout << "cubical_complex dimension: " 
		  << complex1.dimension() << std::endl;	
	std::size_t csize=0;
	Cube temp_cube;
	for( auto i = complex1.begin(); i != complex1.end(); ++i){ 
	  std::cout << ctl::delta << "(";
	  std::cout << ctl::key_to_cube( complex1, i->first); 
	  std::cout << ") = ";
	  if( ctl::cube_dimension( complex1, i->first) > 0){
	  for( auto j = cube_cell_bd.begin( i->first); j != cube_cell_bd.end( i->first); ++j){
	  	std::cout << ctl::key_to_cube( complex1, j->cell()); 
		std::cout << " ";
	  }
	  } else { std::cout << 0; }
	  std::cout << std::endl;
	}
	std::cout << csize << std::endl;
	std::cout << complex1.size() << std::endl;
	return 0;
}
