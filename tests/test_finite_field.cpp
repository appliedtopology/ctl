/*******************************************************************************
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
*******************************************************************************/

#include <catch/catch.hpp>
#include <ctl/finite_field/finite_field.hpp>
#include <iostream>

TEST_CASE("Z2", "[finite-field]"){
	typedef ctl::Finite_field< 2> Z2;
	Z2  a( 1);
	REQUIRE(a+a == Z2(0));
}	

TEST_CASE("Z3", "[finite-field]"){
	typedef ctl::Finite_field< 3> Z3;
	Z3  b( 1);	
	REQUIRE (b+b+b == Z3( 0));
	Z3 bneg( -1); 
	REQUIRE (-1*b == bneg);
}

TEST_CASE("Z11", "[finite-field]"){
	typedef ctl::Finite_field< 11> Z11;
	Z11 c( 1);
	REQUIRE(c*11 == Z11(0));
	for( std::size_t i = 1; i < 11; ++i){
		c = i;
		REQUIRE(c*ctl::inverse(c)==Z11(1));
	}
	for( std::size_t i = 1; i < 11; ++i){
		c = i;
		REQUIRE(c/c == Z11(1));
	}
}
