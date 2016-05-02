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
#include <ctl/finite_field/finite_field.hpp>
#include <iostream>

//GTest
#include "gtest/gtest.h"

TEST(FiniteField,Z2){
typedef ctl::Finite_field< 2> Z2;
Z2  a( 1);
ASSERT_EQ(a+a,Z2(0));
}	

TEST(FiniteField,Z3){
	typedef ctl::Finite_field< 3> Z3;
	Z3  b( 1);	
	ASSERT_EQ(b+b+b, Z3( 0));
	Z3 bneg( -1); 
	ASSERT_EQ(-1*b,bneg);
}

TEST(FiniteField,Z11){
	typedef ctl::Finite_field< 11> Z11;
	Z11 c( 1);
	ASSERT_EQ(c*11,Z11(0));
	for( std::size_t i = 1; i < c.prime(); ++i){
		c = i;
		//ASSERT_TRUE(((cc.inverse())==Z11(1)));
	}
	for( std::size_t i = 1; i < c.prime(); ++i){
		c = i;
		ASSERT_EQ(c/c,Z11(1));
	}
}

TEST(FiniteField,InverterClass) {
  ctl::detail::Inverter< 3 > inverter;
  ASSERT_EQ(inverter.inverse(2), 2);
}
