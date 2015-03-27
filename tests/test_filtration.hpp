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
*******************************************************************************
*******************************************************************************/
//STL
#include <iostream>

//CTL
//abstract_simplex
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>

//chain_complex
#include <ctl/chain_complex/chain_complex.hpp>

//filtration
#include <ctl/filtration/filtration.hpp>
#include <ctl/filtration/less.hpp>
//timer
#include <ctl/utility/timer.hpp>

//GTest
#include <gtest/gtest.h>

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

TEST( Filtration_cell_less, TotalOrderWorking){
	Complex c;
	Timer t;
	t.start();
	Cell s( {1,2,3,4,5,6,8,9,10} );
	c.insert_closed_cell( s);
	ASSERT_EQ(511,c.size());
	Filtration_cell_less f1( c);
	ASSERT_EQ(f1.size(), c.size());
	t.stop();
	ASSERT_TRUE( is_sorted( f1));
}
TEST( Filtration_id_less, TotalOrderWorking){ 
	Complex c;
	Timer t;
	t.start();
	Cell s( {1,2,3,4,5,6,8,9,10} );
	c.insert_closed_cell( s);
	t.stop();
	t.start();
	Filtration_id_less f2( c);
	t.stop();
	ASSERT_TRUE( is_sorted( f2));
}
