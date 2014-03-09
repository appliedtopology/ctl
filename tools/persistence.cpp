// serial_homology.cpp
// Ryan Lewis
// November 7, 2010
// May 31, 2011
//#define COMPUTE_BETTI

//CTL Types for Building a Simplicial Chain Complex and Filtration
#include "finite_field/finite_field.h"
#include "abstract_simplex/abstract_simplex.h"
#include "abstract_simplex/simplex_boundary.h"
#include "chain_complex/complex_boundary.h"
#include "filtration/filtration.h"
#include "filtration/less.h"
#include "io/io.h" 
#include "chain_complex/chain_complex.h"
#include "chain_complex/chain_complex_io.h"
#include "term/term.h"

#include "utility/timer.h"

//Boost
#include <boost/property_map/property_map.hpp> //For Cascades in persistence

//Persistence 
#include "chain/chain.h"
#include "persistence/persistence.h"
#include "persistence/property_maps.h"

//Boost
#include <boost/program_options.hpp>

//STL
#include <sstream>

namespace po = boost::program_options;

//Build Complex type
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_boundary;
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;

//Build Filtration type
typedef	Complex::iterator Complex_iterator;
typedef ctl::Cell_less Complex_cell_less;
typedef ctl::Filtration< Complex, Complex_cell_less > Filtration;
typedef Filtration::iterator Filtration_iterator;

//Complex Boundary Operator
typedef ctl::Complex_boundary< Complex> Complex_boundary;

//Build Chain Type
typedef Complex_boundary::Term Complex_term;
typedef ctl::Term_cell_less< Complex_cell_less> Complex_term_less_cell;
typedef ctl::Chain< Complex_term, Complex_term_less_cell> Complex_chain;

//Type which stores all the chains for persistence
//It would be a good project to see if storing the internal pointer here
//inside of the chain_complex would be a good idea
typedef std::vector< Complex_chain> Complex_chains;
typedef ctl::Id_property_map< Complex::iterator> Complex_offset_map;
typedef boost::iterator_property_map< Complex_chains::iterator, 
	Complex_offset_map, Complex_chain, Complex_chain&> 
	Complex_chain_map;

typedef ctl::Timer Timer;

template<typename Variable_map>
void process_args(int & argc, char *argv[],Variable_map & vm){
  //parse command line options
  po::options_description desc("Usage: persistence [options] input-file");
  desc.add_options()
  ( "help", "Display this message")
  ( "input-file", "input .asc file to parse");
  
  po::positional_options_description p;
  p.add("input-file",1);
  po::store(po::command_line_parser(argc,argv)
	    .options(desc)
	    .positional(p)
	    .run(),
	     vm);
  po::notify(vm);
  if (vm.count("help")){
	std::cout << desc << std::endl;
	std::exit( 0);
  }
  if (vm.count("input-file") != 1){
	std::cout << desc << std::endl;	
	std::exit( -1);
  }
}

int main(int argc, char *argv[]){
  po::variables_map vm;
  process_args(argc,argv,vm);
  
   //get `path/to/complex_name`
  std::string full_complex_name = vm["input-file"].as< std::string>();
  //get `complex_name`
  std::string complex_name(full_complex_name);
  size_t found = complex_name.rfind('/');
  if (found != std::string::npos){ complex_name.replace(0, found+1, ""); }
  std::cout << "full_complex_name: " << full_complex_name << std::endl;
  //create some data structures 
  Complex complex;
  Complex_boundary complex_boundary( complex);
  Timer timer;
                                       
  // Read the cell_set in  
  ctl::read_complex( full_complex_name, complex); 
  std::cout << "complex size: " << complex.size() << std::endl; 
  std::cout << "complex dimension: " << complex.dimension() << std::endl;

  //produce a filtration
  timer.start();
  Filtration complex_filtration( complex);
  timer.stop();
  double complex_filtration_time = timer.elapsed();
  //display some info
  std::cout << "time for complex filtration (secs): " 
	    << complex_filtration_time << std::endl;
  
  //begin instantiate our vector of cascades homology
  Complex_chains complex_cascades( complex.size(), Complex_chain());
  
  //we hand persistence a property map for genericity!                                         
  Complex_chain_map cascade_property_map( complex_cascades.begin(), 
					  Complex_offset_map());

  //serial persistence (complex)
  timer.start();
  ctl::persistence(complex_filtration.begin(), complex_filtration.end(),
  		 complex_boundary, cascade_property_map);
  timer.stop();
  double complex_persistence = timer.elapsed();
  std::cout << "serial persistence (complex): " 
            << complex_persistence << std::endl;
  std::cout << "total time : " << complex_filtration_time + complex_persistence 
	   << std::endl;
                                                                  
#ifdef COMPUTE_BETTI  
  //betti numbers
  typedef std::vector<int> Betti;
  Betti betti;
  std::cout << "serial betti (complex): " << std::endl;
  ctl::compute_betti(complex,cascade_property_map,betti);
  std::cout << std::endl;
  int euler=0;
  int coeff=1;
  for(Betti::iterator i = betti.begin(); i != betti.end(); ++i){
	euler += (*i)*coeff;
	coeff = coeff*(-1);
  }
  std::cout << "(betti) Euler Characteristic: " << euler << std::endl; 
  std::vector< std::size_t> counts( complex.maximum_dimension()+1, 0);
  for (cell_it i = complex.begin(); i != complex.end(); ++i){
	 	counts[ i->first.dimension()]++; 
  }
  euler=0;
  for (std::size_t i = 0; i < counts.size(); ++i){
   if( i % 2 == 0){ euler += counts[ i]; } 
   else{ euler -= counts[ i]; }
  }
  std::cout << "(cell) Euler Characteristic: " << euler << std::endl; 
#endif
  return 0;
}
