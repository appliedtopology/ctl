/*******************************************************************************
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
*******************************************************************************/
#include <ctl/finite_field/finite_field.hpp>
#include <iostream>

//GTest
#include <catch/catch.hpp>

TEST(FiniteField,Z2){
typedef ctl::Finite_field< 2> Z2;
Z2  a( 1);
ASSERT_EQ(a+a,Z2(0));
}	

TEST(FiniteField,Z3){
	typedef ctl::Finite_field< 3> Z3;
	Z3  b( 1);	
	REQUIRE (b+b+b == Z3( 0));
	Z3 bneg( -1); 
	REQUIRE (-1*b == bneg);
}

TEST(FiniteField,Z11){
	typedef ctl::Finite_field< 11> Z11;
	Z11 c( 1);
	REQUIRE(c*11 == Z11(0));
	for( std::size_t i = 1; i < c.prime(); ++i){
		c = i;
		//ASSERT_TRUE(((cc.inverse())==Z11(1)));
	}
	for( std::size_t i = 1; i < c.prime(); ++i){
		c = i;
		REQUIRE(c/c == Z11(1));
	}
}
