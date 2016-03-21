// STL
#include <iostream> 
#include <fstream>
#include <sstream>

// Project
#include <ctl/ctl.h>

#include "gtest/gtest.h"


TEST_CASE( ProductCell, Constructors){
  typedef ctl::Abstract_simplex Simplex;
  typedef ctl::Finite_field< 2> Z2;
  typedef ctl::Simplex_boundary< Simplex, Z2> Boundary;
  typedef ctl::Chain_complex< Simplex, Boundary> Complex;
  // product type
  typedef ctl::Product_cell< Complex::iterator, Complex::iterator> Product;

  Complex c1, c2;
  Complex::Cell s1, s2;

  // Two starting complexes:  triangle 012, and edge 34
  s1.insert( 0);
  s1.insert( 1);
  s1.insert( 2);

  s2.insert( 3);
  s2.insert( 4);
  
  c1.insert_closed_cell( s1);
  c2.insert_closed_cell( s2);

  auto i1 = c1.find_cell( s1);
  auto i2 = c2.find_cell( s2);

  // first product
  Product p1( i1, i2);
  ASSERT_EQ( p1.dimension(), 3);
  // copy constructor
  Product p2( p1);
  ASSERT_EQ(p2, p1);

  // assignment 
  Product p3 = p1;
  ASSERT_EQ(p3, p1);
  ASSERT_EQ(p2, p3);
  ASSERT_EQ(p3.dimension(), 3);

  // second one
  Product p4( i2, i1);

  ASSERT_LT( p4, p2);
}

TEST_CASE(ProductCell, ProductBoundary){ 
  typedef ctl::Abstract_simplex Simplex;
  typedef ctl::Finite_field< 2> Z2;
  typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_boundary;
  typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;
  // product type
  typedef ctl::Product_cell< Complex::iterator, Complex::iterator> Product;

  Complex c1, c2;
  Complex::Cell s1, s2;

  // Two starting complexes:  triangle 012, and edge 34
  s1.insert( 0);
  s1.insert( 1);
  s1.insert( 2);

  s2.insert( 3);
  s2.insert( 4);
  
  c1.insert_closed_cell( s1);
  c2.insert_closed_cell( s2);

  auto i1 = c1.find_cell( s1);
  auto i2 = c2.find_cell( s2);

  // first product
  Product p1( i1, i2);
  // copy constructor
  Product p2( p1);

  // Now, onto boundary
  typedef ctl::Complex_boundary< Complex>   Boundary;
  typedef ctl::Product_boundary< Product, Boundary, Boundary> Product_boundary;

  Boundary b1( c1);
  Boundary b2( c2);

  // now the product
  Product_boundary boundary( b1, b2);

  // regularity
  Product_boundary::const_iterator j; // default
  j = boundary.begin( p1);  // assignable
  ASSERT_EQ(j, boundary.begin( p1));
}
