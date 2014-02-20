//STL
#include <iostream>

//CTL
//abstract_simplex
#include "abstract_simplex/abstract_simplex.h"
#include "abstract_simplex/simplex_boundary.h"

//chain_complex
#include "chain_complex/chain_complex.h"
#include "chain_complex/complex_boundary.h"

//We build a simplicial chain complex with Z2 coefficients
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2 > Cell_boundary;
typedef ctl::Chain_complex< Simplex, Cell_boundary> Complex;
typedef Complex::Cell Cell;
typedef ctl::Complex_boundary< Complex> Complex_boundary;
typedef Complex_boundary::const_iterator boundary_iterator;
int main( int argc, char** argv){
	Complex c;
	Cell s( {1,2,3,4} );
	auto pair = c.insert_closed_cell( s);
	std::cout << pair.second << " cells inserted!" << std::endl;
	std::cout << c << std::endl;
	Complex_boundary b( c);
	Complex_boundary copyd( b);
	Complex_boundary moved( std::move( copyd));
	std::cout << "boundary test: " 
		  << c.begin()->first << std::endl;
	auto wtf = c.begin();
	std::cout << "boundary length: " << b.length( wtf) << std::endl;
	for( boundary_iterator i = b.begin( wtf); i != b.end( wtf); ++i){
		std::cout << i->cell()->first 
			  << " --> {" 
			  << i->cell()->second  
			  << "}"<< std::endl;
	}
	return 0;
}
