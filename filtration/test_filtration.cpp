//STL
#include <iostream>

//CTL
//abstract_simplex
#include "abstract_simplex/abstract_simplex.h"
#include "abstract_simplex/simplex_boundary.h"

//chain_complex
#include "chain_complex/chain_complex.h"

//filtration
#include "filtration/filtration.h"
#include "filtration/less.h"
//timer
#include "utility/timer.h"

//We build a simplicial chain complex with Z2 coefficients
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2 > Boundary;
typedef ctl::Chain_complex< Simplex, Boundary> Complex;
typedef Complex::Cell Cell;
typedef ctl::Filtration< Complex> Filtration_cell_less;
typedef ctl::Timer Timer;
typedef ctl::Filtration< Complex, ctl::Id_less> Filtration_id_less;

template< typename Filtration>
bool is_sorted( const Filtration & f){
	typedef typename Filtration::Less Less;
	Less less;
	auto last = f.begin();
	for( auto i = ++f.begin(); i < f.end(); ++i, ++last){
		if (!less( *last, *i)){ return false; }
	}
	return true;
}

int main( int argc, char** argv){
	Complex c;
	Timer t;
	t.start();
	Cell s( {1,2,3,4,5,6,8,9,10} );
	c.insert_closed_cell( s);
	t.stop();
	std::cout << "construction: " << t.elapsed() << std::endl;
	std::cout << "size: " << c.size() << std::endl;
	t.start();
	Filtration_cell_less f1( c);
	t.stop();
	std::cout << "f1 is sorted " << is_sorted( f1) 
		  << " " << t.elapsed() << std::endl; 
	t.start();
	Filtration_id_less f2( c);
	t.stop();
	std::cout << "f2 is sorted " << is_sorted( f2) 
		  << " " << t.elapsed() << std::endl; 
	for( auto i = f2.begin(); i != f2.end(); ++i){
		std::cout << i.pos() << ": " << (*i)->first << std::endl;
	}
	return 0;
}
