//STL
#include <iostream>

//CTL
//abstract_simplex
#include "abstract_simplex/abstract_simplex.h"
#include "abstract_simplex/simplex_boundary.h"

//chain_complex
#include "chain_complex.h"

//We build a simplicial chain complex with Z2 coefficients
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2 > Boundary;
typedef ctl::Chain_complex< Simplex, Boundary> Complex;
typedef Complex::Cell Cell;
int main( int argc, char** argv){
	Complex c;
	Cell s( {1,2,3,4} );
	c.insert_closed_cell( s);
	std::cout << c << std::endl;
	return 0;
}
