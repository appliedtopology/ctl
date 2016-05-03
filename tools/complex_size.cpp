/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************/
//#define COMPUTE_BETTI
//#define CTL_USE_MURMUR
#define CTL_USE_CITY
//#define CTL_USE_JENKINS
//#define COMPLEX_DIAGNOSTICS

#ifdef ZOOM_PROFILE
#include "zoom.h"
#endif

#ifdef COMPLEX_DIAGNOSTICS
	#include "utility/complex_diagnostics.h"
#endif
//CTL
//Types for Building a Simplicial Chain Complex and Filtration
#include <ctl/ctl.hpp>

//STL
#include <sstream>

//Build Complex type
typedef ctl::Abstract_simplex Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary Simplex_boundary;
typedef ctl::Cell_complex< Simplex_boundary> Complex;

template<typename String>
void process_args(int & argc, char *argv[], String & filename){
  std::string usage = "Usage: complex_size input-file";
  if (argc != 2){ std::cerr << usage << std::endl; exit( 1); }
  filename = std::string( argv[ 1]);
}

int main(int argc, char *argv[]){
  std::string full_complex_name; 
  process_args(argc, argv, full_complex_name);
  
  //create some data structures 
  Complex complex;
  ctl::Timer timer;
                                       
  // Read the cell_set in  
  timer.start();
  //ctl::read_complex( full_complex_name, complex); 
  timer.elapsed();
  std::cout << "I/O Time: " << timer.elapsed() << std::endl;
  std::cout << "number of simplices: " << complex.size() << std::endl; 
  std::cout << "complex dimension: " << complex.dimension() << std::endl;
  typedef std::vector< std::size_t> Vector;
  Vector counts( complex.dimension()+1, 0);
  typedef typename Complex::Cell Cell;
  typedef typename Cell::value_type Vertex_type;
  
  std::size_t complex_bytes=sizeof( Cell)*complex.size() + sizeof( Complex); 
  for( const auto & cell : complex){
	complex_bytes += sizeof( Vertex_type)*cell.first.capacity();
	counts[ cell.first.dimension()]++;
  }
  std::cout << "Cells by dimension: " << std::endl;
  for( const auto size : counts){ std::cout << size << std::endl; }
  std::cout << "Rough estimate of memory used: " 
	    << complex_bytes << " (bytes)" << std::endl; 
  return 0;
}
