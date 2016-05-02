// STL
#include <iostream> 
#include <fstream>
#include <sstream>

#include <catch/catch.hpp>
// Project
#include <ctl/ctl.hpp>

TEST_CASE( "IteratorConstructors", "[product-cell]"){

  //typedef ctl::Abstract_simplex Simplex;
  typedef ctl::Simplex_boundary Boundary;
  typedef ctl::Cell_complex< Boundary> Complex;
  // product type
  typedef ctl::Iterator_product_cell< Complex::iterator, Complex::iterator> Product;

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
  REQUIRE( p1.dimension() == 3);
  // copy constructor
  Product p2( p1);
  REQUIRE(p2 == p1);

  // assignment 
  Product p3 = p1;
  REQUIRE(p3== p1);
  REQUIRE(p2== p3);
  REQUIRE(p3.dimension() == 3);

  // second one
  Product p4( i2, i1);

  REQUIRE( p4<  p2);
}

TEST_CASE( "Constructors", "[product-cell]"){

  //typedef ctl::Abstract_simplex Simplex;
  typedef ctl::Simplex_boundary Boundary;
  typedef ctl::Cell_complex< Boundary> Complex;
  // product type
  typedef ctl::Product_cell< Complex::Cell, Complex::Cell> Product;

  Complex::Cell s1, s2;

  // Two starting complexes:  triangle 012, and edge 34
  s1.insert( 0);
  s1.insert( 1);
  s1.insert( 2);

  s2.insert( 3);
  s2.insert( 4);
  
  // first product
  Product p1( s1, s2);
  REQUIRE( p1.dimension() == 3);
  // copy constructor
  Product p2( p1);
  REQUIRE(p2 == p1);

  // assignment 
  Product p3 = p1;
  REQUIRE(p3== p1);
  REQUIRE(p2== p3);
  REQUIRE(p3.dimension() == 3);

  // second one
  Product p4( s2, s1);

  REQUIRE( p4<  p2);
}

TEST_CASE("ProductBoundary", "[product-cell]"){ 
  //typedef ctl::Abstract_simplex Simplex;
  typedef ctl::Simplex_boundary Simplex_boundary;
  typedef ctl::Cell_complex< Simplex_boundary> Complex;
  // product type
  typedef ctl::Product_cell< Complex::Cell, Complex::Cell> Product;

  Complex::Cell s1, s2;

  // Two starting complexes:  triangle 012, and edge 34
  s1.insert( 0);
  s1.insert( 1);
  s1.insert( 2);

  s2.insert( 3);
  s2.insert( 4);
  s2.insert( 5);
  
  // first product
  Product p1( s1, s2);
  // copy constructor
  Product p2( p1);

  typedef ctl::Product_boundary< Simplex_boundary, Simplex_boundary> Product_boundary;

  // now the product
  Product_boundary boundary;

  // regularity
  auto j = boundary.begin( p1);  // assignable
  REQUIRE(j == boundary.begin( p1));
  for(; j != boundary.end( p1); *j++){
	  std::cout << *j << std::endl;
  }
}



TEST_CASE("IteratorProductBoundary", "[product-cell]"){ 
  //typedef ctl::Abstract_simplex Simplex;
  typedef ctl::Simplex_boundary Simplex_boundary;
  typedef ctl::Cell_complex< Simplex_boundary> Complex;
  // product type
  typedef ctl::Iterator_product_cell< Complex::iterator, Complex::iterator> Product;

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
  typedef ctl::Iterator_product_boundary< Product, Boundary, Boundary> Product_boundary;

  Boundary b1( c1);
  Boundary b2( c2);

  // now the product
  Product_boundary boundary( b1, b2);

  // regularity
  Product_boundary::const_iterator j; // default
  j = boundary.begin( p1);  // assignable
  REQUIRE(j == boundary.begin( p1));
}
