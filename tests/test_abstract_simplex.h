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
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>
#include <ctl/term/term.h>

//GTest
#include "gtest/gtest.h"

TEST(Simplex, DefaultConstruct){
	typedef ctl::Abstract_simplex< int> Simplex;

	Simplex s;
	ASSERT_EQ(s.size(), 0);
}

TEST(Simplex, InsertCopyTest){
	typedef ctl::Abstract_simplex< int> Simplex;

	Simplex s;
	s.insert( 2);
	s.insert( 1);
	s.insert( 4);
	s.insert( 3);
	Simplex c( s);
	ASSERT_EQ( c, s);
}

TEST(Simplex,InitializerListAndRemove){
	typedef ctl::Abstract_simplex< int> Simplex;

	Simplex t( {1,2,5,5,4});
	Simplex s;
	s.insert( 1); s.insert( 2); s.insert( 4); s.insert( 5);
	ASSERT_EQ( s, t);
	t.insert( {8,9,10} );
	t.remove( 5);
	ASSERT_EQ(t.size(), 6);
	t.remove( t.begin()+2, t.begin()+4);
	ASSERT_EQ(t.size(), 4);
}
	
TEST(Simplex, SimplexBoundary){
	typedef ctl::Abstract_simplex< int> Simplex;
	typedef ctl::Simplex_boundary< Simplex, int> Boundary;

	Boundary b;
	Simplex s{1,2,3};
	std::vector< Simplex> bd{ {2,3},{1,3},{1,2}};	
	for (auto i = b.begin( s); i != b.end( s); ++i){
	 ASSERT_NE(std::find( bd.begin(), bd.end(), i->cell()), bd.end()); 	
	}
}

TEST(Simplex, SelfAssignmentComparison){
	typedef ctl::Abstract_simplex< int> Simplex;

	Simplex s{ 3,4,5};
	s = s;
	ASSERT_EQ(s,s);
}
