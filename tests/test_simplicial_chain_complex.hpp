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
*******************************************************************************/
//STL
#include <iostream>

//CTL
//abstract_simplex
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>

//Chain_complex
#include <ctl/chain_complex/chain_complex.hpp>
#include <ctl/chain_complex/complex_boundary.hpp>

//We build a simplicial chain complex with Z2 coefficients

typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 3> Z3;
typedef ctl::Simplex_boundary< Simplex, Z3 > Simplex_boundary;
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Simplicial_complex;
typedef Simplicial_complex::Cell Cell;
typedef ctl::Complex_boundary< Simplicial_complex> Simplicial_complex_boundary;

typedef Simplicial_complex_boundary::const_iterator simplicial_boundary_iterator;


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
	return 0;
}
