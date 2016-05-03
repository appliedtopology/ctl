/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************/
//STL
#include <iostream>

//CTL
#include <ctl/ctl.hpp>
#include <catch/catch.hpp>

TEST_CASE("Construct & Iterate", "[relative complex]"){
	ctl::Simplicial_complex<> X;
	ctl::Simplicial_complex<> A;
	ctl::Abstract_simplex sigma{1,2,3,4};
	X.insert_closed_cell( sigma );
	ctl::Simplex_boundary bd;
	for(auto elt = bd.begin( sigma); elt != bd.end( sigma); ++elt){
		A.insert_closed_cell(elt->cell());
	}
	typedef ctl::Relative_cell_complex<decltype(X)> Rcx;
        Rcx	R( X, A);
	std::cout << "X/A = " << R << std::endl;
	REQUIRE( std::distance( std::begin(R), std::end(R)) == 1);
	REQUIRE( std::begin(R)->first == sigma);
	typedef ctl::Relative_complex_boundary< Rcx> Rcb;
	Rcb rbd( R);
	for( auto rel_sigma = R.begin(); rel_sigma != R.end(); ++rel_sigma){
		REQUIRE( std::distance( rbd.begin(rel_sigma), rbd.end(rel_sigma)) == 0);
	}
	ctl::Simplicial_complex<> B;
	int count = 0;
        for(auto elt = bd.begin( sigma); elt != bd.end( sigma); ++elt){
		if( count < 3){
		B.insert_closed_cell(elt->cell());
		}
		++count;

	}
	std::cout << "B = " << B << std::endl;
	Rcx	A_rel_B( A, B);
	std::cout << "A/B" << A_rel_B << std::endl;
	Rcx	X_rel_B( X, B);
	std::cout << "X/B" << X_rel_B << std::endl;
	auto sigma_in_X_rel_B = X_rel_B.find_cell( sigma);
	REQUIRE( sigma_in_X_rel_B != X_rel_B.end());
	Rcb rbd1( X_rel_B);

	for( auto i = rbd1.begin( sigma_in_X_rel_B); i != rbd1.end( sigma_in_X_rel_B); ++i){ 
		std::cout << i->cell()->first << " " << std::endl;
	}
}


