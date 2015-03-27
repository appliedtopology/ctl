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
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program in a file entitled COPYING; if not, write to the 
* Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************************
*******************************************************************************/
//STL
#include <iostream>

//CTL
#include <ctl/cube/cube.hpp>
#include <ctl/cube/cube_boundary.hpp>
#include <ctl/term/term.hpp>

#include "gtest/gtest.h"

TEST(Cube,CubeTest){
typedef ctl::Cube< int> Cube;
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
