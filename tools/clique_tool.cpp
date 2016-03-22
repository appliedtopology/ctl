/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************/
// build_blobs.cpp
// Ryan Lewis
// July 8, 2011

//STL
#include <string>
#include <sstream>

//CTL
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/io/io.hpp>

typedef ctl::Abstract_simplex Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary Simplex_Boundary;
typedef ctl::Cell_complex< Simplex_Boundary> Complex;

int main(int argc, char *argv[]) {
  if( argc < 2) {
   std::cerr << "Usage:  " << argv[ 0] << " filename num_vertices" << std::endl;
   std::exit( 1);
  }
  //initial stuff
  Complex complex;
  std::string basename(argv[ 1]);
  basename.append(".").append(argv[ 2]).append(".asc");
  std::size_t n = atoi( argv[ 2]);
  Simplex cell;
  for( std::size_t i = 0; i < n; ++i) { cell.insert(i); }
  std::cout << "building VR complex on: " << cell << std::endl;
  complex.insert_closed_cell(cell,true);
  std::cout << "complex has: " << complex.size() << " simplices" << std::endl;
  std::ofstream out;
  ctl::open_file(out, basename.c_str());
  //complex.write( out);
  ctl::close_file( out);
  return 0;
}
