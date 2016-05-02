/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************/

#include <catch/catch.hpp>
//STL
#include <iostream>
#include <sstream>
#include <iomanip>

//CTL
#include <ctl/ctl.hpp>

//Catch 



TEST_CASE("ChainTest","[chain]"){

//We build a simplicial chain complex with Z2 coefficients
//typedef ctl::Abstract_simplex Simplex;
//typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary Boundary;
typedef ctl::Cell_complex< Boundary> Complex;
typedef Complex::Cell Cell;
typedef ctl::Complex_boundary< Complex> Complex_boundary;
typedef Complex_boundary::Term Term;
//typedef ctl::Filtration< Complex, ctl::Id_less> Filtration;
typedef ctl::Chain< Term, ctl::Term_cell_less< ctl::Id_less> > Chain;
 
Complex complex;
ctl::Timer timer;
Cell s( {1,2,3,4,5,6} );
Cell t( {1,2,3,4,5,11} );
complex.insert_closed_cell( s);
complex.insert_closed_cell( t);

Complex_boundary bd( complex);	
auto complex_s = complex.find_cell( s);
auto complex_t = complex.find_cell( t);
Chain bds( bd.begin( complex_s), bd.end( complex_s));
Chain bdt( bd.begin( complex_t), bd.end( complex_t));
auto bdsandt = bds+bdt;
REQUIRE(bdsandt.size()==10);

constexpr int N = 600000;
Chain temp;
for( int i = 0; i < N; ++i){	
 bds.scaled_add( 1, bdt, temp);
 Chain bds( bd.begin( complex_s), bd.end( complex_s));
 Chain bdt( bd.begin( complex_t), bd.end( complex_t));
}

}
