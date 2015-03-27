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
#include <sstream>
#include <iomanip>

//CTL
#include <ctl/ctl.hpp>

//GTest
#include "gtest/gtest.h"


TEST(Chain,ChainTest){
//We build a simplicial chain complex with Z2 coefficients
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2 > Boundary;
typedef ctl::Chain_complex< Simplex, Boundary> Complex;
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
ASSERT_EQ(bdsandt.size(),10);

constexpr int N = 600000;
Chain temp;
for( int i = 0; i < N; ++i){	
 bds.scaled_add( 1, bdt, temp);
 Chain bds( bd.begin( complex_s), bd.end( complex_s));
 Chain bdt( bd.begin( complex_t), bd.end( complex_t));
}

}
