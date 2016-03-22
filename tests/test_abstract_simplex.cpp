/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************/
//STL
#include <iostream>

//CTL
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>
#include <ctl/term/term.hpp>

#include <catch/catch.hpp>

TEST_CASE("DefaultConstruct", "[simplex]"){
	typedef ctl::Abstract_simplex Simplex;

	Simplex s;
	REQUIRE(s.size() == 0);
}

TEST_CASE("InsertCopy", "[simplex]"){
	typedef ctl::Abstract_simplex Simplex;

	Simplex s;
	s.insert( 2);
	s.insert( 1);
	s.insert( 4);
	s.insert( 3);
	Simplex c( s);
	REQUIRE( c == s);
}

TEST_CASE("InitializerListAndRemove", "[simplex]"){
	typedef ctl::Abstract_simplex Simplex;

	Simplex t( {1,2,5,5,4});
	Simplex s;
	s.insert( 1); s.insert( 2); s.insert( 4); s.insert( 5);
	REQUIRE( s==t);
	t.insert( {8,9,10} );
	t.remove( 5);
	REQUIRE(t.size() == 6);
	t.remove( t.begin()+2, t.begin()+4);
	REQUIRE(t.size() == 4);
}
	
TEST_CASE("SimplexBoundary", "[simplex]"){
	typedef ctl::Abstract_simplex Simplex;
	typedef ctl::Simplex_boundary Boundary;

	Boundary b;
	Simplex s{1,2,3};
	std::vector< Simplex> bd{ {2,3},{1,3},{1,2}};	
	for (auto i = b.begin( s); i != b.end( s); ++i){
	 REQUIRE(std::find( bd.begin(), bd.end(), i->cell()) != bd.end()); 	
	}
}

TEST_CASE("SelfAssignmentComparison", "[simplex]"){
	typedef ctl::Abstract_simplex Simplex;

	Simplex s{ 3,4,5};
	s = s;
	REQUIRE(s==s);
}
