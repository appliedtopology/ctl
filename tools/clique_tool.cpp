// build_blobs.cpp
// Ryan Lewis
// July 8, 2011

// Project
#include "abstract_simplex/abstract_simplex.h"
#include "chain_complex/chain_complex.h"
#include "chain_complex/chain_complex_io.h"
#include "io/io.h"
// STL
#include <string>
#include <sstream>

typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_Boundary;
typedef ctl::Chain_complex< Simplex, Simplex_Boundary> Complex;

int main(int argc, char *argv[]) {
  if( argc < 2) {
   std::cerr << "Usage:  " << argv[ 0] << " filename num_vertices" << std::endl;
   std::exit( 1);
  }
  //initial stuff
  Complex complex;
  std::ofstream out;
  std::string basename(argv[ 1]);
  basename.append(".").append(argv[ 2]).append(".asc");
  std::size_t n = atoi( argv[ 2]);
  Simplex cell;
  for( std::size_t i = 0; i < n; ++i) { cell.insert(i); }
  std::cout << "building VR complex on: " << cell << std::endl;
  complex.insert_closed_cell(cell,true);
  std::cout << "complex has: " << complex.size() << " simplices" << std::endl;
  ctl::open_file(out,basename.c_str());
  out << complex;
  ctl::close_file( out);
  return 0;
}
