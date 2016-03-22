/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*
*******************************************************************************
* BSD-3
*******************************************************************************/
#include <catch/catch.hpp>


//STL
#include <iostream>

//Project
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/one_skeleton/one_skeleton.hpp>



TEST_CASE("GrabsOnlyGraph", "[one-skeleton]"){ 
typedef ctl::Abstract_simplex Simplex;
typedef ctl::Simplex_boundary Simplex_boundary;
typedef ctl::Cell_complex< Simplex_boundary> Complex;

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
  REQUIRE(true ==std::is_permutation( n1.begin(), n1.end(), neighbors.begin()));
  REQUIRE(true ==std::is_permutation( o1.begin(), o1.end(), offsets.begin()));
  REQUIRE(true ==std::is_permutation( i1.begin(), i1.end(), index_to_vertex_map.begin()));
}

