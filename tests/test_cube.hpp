/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************/
//STL
#include <iostream>

//CTL
#include <ctl/cube/cube.hpp>
#include <ctl/cube/cube_boundary.hpp>
#include <ctl/term/term.hpp>

#include "catch/catch.hpp"

TEST(Cube,CubeTest){
typedef ctl::Cube Cube;
typedef Cube::Interval Interval;
typedef ctl::Cube_boundary< Cube, int> Boundary;
	Cube s;
	s.insert( Interval( 2, 3) );
	s.insert( Interval( 3, 3) );
	Cube c( s);
	ASSERT_EQ(c,s);
	Cube t( {{1,2},{4,5},{4,4}});
	t.insert( {{8,9},{9,10}, {11,11}} );
	t.remove( {11,11} );
	t.remove( t.begin(), t.begin()+1);
	Boundary b;
	for (auto i = b.begin( t); i != b.end( t); ++i){
		auto j = (*i).cell();
	}
}
