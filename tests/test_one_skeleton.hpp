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
* \cite{hatcher, z-fcv-10a}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
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

//Project
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/one_skeleton/one_skeleton.h>

#include <gtest/gtest.h>

TEST(OneSkeleton, GrabsOnlyGraph){ 
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2; 
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_boundary;
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;



  Complex complex, complex1;

  // Types for METIS CSR & map
  typedef Complex::Cell Simplex;
  typedef Simplex::vertex_type vertex_type;
  typedef std::vector< unsigned int> Vector;
  std::vector< vertex_type> index_to_vertex_map, i1;

  // Results
  Vector neighbors, n1, offsets, o1;

  complex.insert_closed_cell( {1,2,3});
  complex1.insert_closed_cell( {1,2});
  complex1.insert_closed_cell( {1,3});
  complex1.insert_closed_cell( {2,3});

  // Extract out 1-skeleton in CSR format
  one_skeleton( complex, neighbors, offsets, index_to_vertex_map);
  one_skeleton( complex1, n1, o1, i1);
  ASSERT_TRUE(std::is_permutation( n1.begin(), n1.end(), neighbors.begin()));
  ASSERT_TRUE(std::is_permutation( o1.begin(), o1.end(), offsets.begin()));
  ASSERT_TRUE(std::is_permutation( i1.begin(), i1.end(), index_to_vertex_map.begin()));
}

