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
#include "gtest/gtest.h"

//STL
#include <iostream>

//CTL
#include <ctl/cube/cube.h>
#include <ctl/cube/cube_boundary.h>

//chain_complex
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/chain_complex/complex_boundary.h>

//We build a simplicial chain complex with Z2 coefficients

typedef ctl::Finite_field< 3> Z3;
typedef ctl::Cube< int> Cube;
typedef ctl::Cube_boundary< Cube, Z3 > Cube_boundary;
typedef ctl::Chain_complex< Cube, Cube_boundary> Cubical_complex;
typedef Cubical_complex::Cell_boundary Cubical_cell_boundary;
typedef ctl::Complex_boundary< Cubical_complex> Cubical_complex_boundary;
typedef Cubical_complex_boundary::const_iterator cubical_boundary_iterator;

TEST(CubicalComplex, EqualityTest){
	std::vector< std::size_t> sizes{4,5};
	std::vector< std::size_t> start{3,97};
	Cubical_complex complex( sizes, start);
	Cubical_complex c2( complex);
	ASSERT_TRUE(complex==c2);
}


TEST(CubicalComplex, FindCellWorks){
	std::vector< std::size_t> sizes{4,5};
	std::vector< std::size_t> start{3,97};
	Cubical_complex complex( sizes, start);
	for( auto i = complex.begin(); i != complex.end(); ++i){ 
	  ASSERT_EQ( complex.find_cell( i->first), i);
	}
}

TEST(CubicalComplex, IdNBits2Index){
 	std::vector< std::size_t> sizes{16,16};
 	std::vector< std::size_t> start{16,16};
	Cubical_complex complex( sizes, start);
	std::size_t pos = 0;
	for( auto i = complex.begin(); i != complex.end(); ++i, ++pos){ 
	  ASSERT_EQ( complex.id_and_bits_to_index( i->first), pos);
	}
}

TEST(CubicalComplex, IdNBits2CoordinateIdempotency){
 	std::vector< std::size_t> sizes{16,16};
 	std::vector< std::size_t> start{16,16};
	Cubical_complex complex( sizes, start);
	std::size_t pos = 0;
	for( auto i = complex.begin(); i != complex.end(); ++i, ++pos){ 
	  ASSERT_EQ( (i->first).data(),
                 complex.coordinate_to_id_and_bits( 
			complex.id_and_bits_to_coordinate( i->first)).data());
	}
}
