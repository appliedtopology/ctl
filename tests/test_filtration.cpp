/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************/

#include <catch/catch.hpp>
//STL
#include <iostream>

//CTL
//abstract_simplex
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>

//chain_complex
#include <ctl/cell_complex/cell_complex.hpp>

//filtration
#include <ctl/graded_chain_complex/graded_cell_complex.hpp>
#include <ctl/graded_chain_complex/less.hpp>
//timer
#include <ctl/utility/timer.hpp>

//GTest


//We build a simplicial chain complex with Z2 coefficients
typedef ctl::Abstract_simplex Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary Boundary;
typedef ctl::Cell_complex< Boundary> Complex;
typedef Complex::Cell Cell;
typedef ctl::Graded_cell_complex< Complex> Filtration_cell_less;
typedef ctl::Timer Timer;
typedef ctl::Graded_cell_complex< Complex, ctl::Id_less> Filtration_id_less;

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

TEST_CASE( "Filtration_cell_less", "TotalOrderWorking"){
	Complex c;
	Timer t;
	t.start();
	Cell s( {1,2,3,4,5,6,8,9,10} );
	c.insert_closed_cell( s);
	REQUIRE(511 == c.size());
	Filtration_cell_less f1( c);
	REQUIRE(f1.size() == c.size());
	t.elapsed();
	REQUIRE( is_sorted( f1) == true);
}
TEST_CASE( "Filtration_id_less", "TotalOrderWorking"){ 
	Complex c;
	Timer t;
	t.start();
	Cell s( {1,2,3,4,5,6,8,9,10} );
	c.insert_closed_cell( s);
	t.elapsed();
	t.start();
	Filtration_id_less f2( c);
	t.elapsed();
	REQUIRE( true == is_sorted( f2));
}
