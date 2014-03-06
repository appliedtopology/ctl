//STL
#include <iostream>
#include <sstream>

//CTL
//abstract_simplex
#include "abstract_simplex/abstract_simplex.h"
#include "abstract_simplex/simplex_boundary.h"

//chain_complex
#include "chain_complex/chain_complex.h"
#include "chain_complex/complex_boundary.h"

//chain
#include "chain/chain.h"

//delta
#include "io/io.h"

#include "filtration/filtration.h"

//We build a simplicial chain complex with Z2 coefficients
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2 > Boundary;
typedef ctl::Chain_complex< Simplex, Boundary> Complex;
typedef Complex::Cell Cell;
typedef ctl::Complex_boundary< Complex> Complex_boundary;
typedef Complex_boundary::Term Term;
typedef ctl::Chain< Term> Chain;
typedef ctl::Filtration< Complex, ctl::Id_less> Filtration;
 
std::string print_chain( const Chain & a){
	std::stringstream out;
	if (a.size() == 0) { return out.str(); }
	out << a.begin()->coefficient() 
	    << "*" << a.begin()->cell()->first << " "; 
	for( auto i = ++(a.begin()); i != a.end(); ++i){
		out << i->coefficient() <<  "*" << i->cell()->first << " ";
	}
	return out.str();
}

int main( int argc, char** argv){
	Complex complex;
	Cell s( {1,2,3,4} );
	auto cell_and_count = complex.insert_closed_cell( s);
	std::cout << cell_and_count.second << " cells inserted!" << std::endl;
	std::cout << complex << std::endl;
	Complex_boundary bd( complex);	
	std::cout << "created complex boundary" << std::endl;
	auto complex_cell = complex.begin();
	auto begin_iterator = bd.begin( complex_cell);
	auto end_iterator = bd.end( complex_cell);

	std::cout << "explicitify chain" << std::endl;
	Chain bda( begin_iterator, end_iterator);
	std::cout << "chain made." << std::endl;
	std::cout << "a = " << complex_cell->first << std::endl; 
	std::cout << ctl::delta << "(a) = " << print_chain( bda) << std::endl;
	complex_cell++;
	Chain bdb( bd.begin( complex_cell), bd.end( complex_cell));
	std::cout << ctl::delta << "(b) = " << print_chain( bdb) << std::endl;
	std::cout << "b = " << complex_cell->first << std::endl; 
	Chain bdp = bda+bdb;
	std::cout << ctl::delta << "(a+b) = " << print_chain( bdp) << std::endl;
	return 0;
}
