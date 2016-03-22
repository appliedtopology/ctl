/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************/

#include <catch/catch.hpp>

//STL
#include <iostream>

//CTL
#include <ctl/cube/cube.hpp>
#include <ctl/cube/cube_boundary.hpp>

//chain_complex
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/cell_complex/complex_boundary.hpp>

//We build a simplicial chain complex with Z2 coefficients

typedef ctl::Finite_field< 3> Z3;
typedef ctl::Cube Cube;
typedef ctl::Cube_boundary Cube_boundary;
typedef ctl::Cell_complex< Cube_boundary> Cubical_complex;
typedef Cubical_complex::Cell_boundary Cubical_cell_boundary;
typedef ctl::Complex_boundary< Cubical_complex> Cubical_complex_boundary;
typedef Cubical_complex_boundary::const_iterator cubical_boundary_iterator;
/*
TEST_CASE("EqualityTest","[cubical complex]"){
	Cubical_complex complex;
	Cubical_complex c2( complex);
	REQUIRE(complex==c2);
}


TEST_CASE("FindCellWorks","[cubical complex]"){
	Cubical_complex complex;
	for( auto i = complex.begin(); i != complex.end(); ++i){ 
	  REQUIRE( complex.find_cell( i->first) == i);
	}
}*/
