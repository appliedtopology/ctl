/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
******************************************************************************
* BSD-3
*******************************************************************************/
#define COMPUTE_BETTI
//#define TESTS_ON

// Global Project Deps
#include <ctl/finite_field/finite_field.hpp>
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>
#include <ctl/cell_complex/complex_boundary.hpp>
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/graded_chain_complex/graded_cell_complex.hpp>
#include <ctl/io/io.hpp> 
#include <ctl/utility/timer.hpp>

//BOOST
#include <boost/program_options.hpp>

//STL
#include <sstream>
#include <vector>
//TODO: tool should generically handle _any_ cell type
//TODO: should have a weighted complex type like in the VR paper 
// Should be a mechanism for arbitrary filtrations. 
namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex Cell;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary Simplex_boundary;
typedef ctl::Cell_complex< Simplex_boundary> Complex;
typedef Complex::iterator Complex_iterator;
typedef ctl::Cell_less Cell_less;
typedef ctl::Graded_cell_complex< Complex, Cell_less> Complex_filtration;
typedef Complex_filtration::iterator Complex_filtration_iterator;


template<typename Variable_map>
void process_args( int & argc, char *argv[],Variable_map & vm){
  //parse command line options
  po::options_description desc( "Usage: write_filtration [options] input-file");
  desc.add_options()
  ( "help", "Display this message")
  ( "input-file", "input .asc file to parse");
  po::positional_options_description p;
  p.add( "input-file",1);
  po::store( po::command_line_parser( argc,argv)
	    .options( desc)
	    .positional( p)
	    .run(),
	     vm);
  po::notify( vm);
  if ( vm.count( "help")){
	std::cout << desc << std::endl;
	std::exit( 0);
  }
  if ( vm.count( "input-file") != 1){
	std::cout << desc << std::endl;	
	std::exit( -1);
  }
}

int main( int argc, char *argv[]){
  po::variables_map vm;
  process_args( argc, argv, vm);

  //setup some variables
  std::string full_complex_name = vm[ "input-file"].as< std::string>();
  std::string complex_name( full_complex_name);
  std::string binary_name( argv[ 0]);
  std::string base_name( full_complex_name);
  std::string output_name;
  size_t found = complex_name.rfind( '/');
  if ( found != std::string::npos){
        complex_name.replace( 0, found+1, "");
  }
  found = full_complex_name.rfind('.');
   if ( found != std::string::npos){
        base_name.replace(found,full_complex_name.length(), "");
  	output_name = base_name + ".flt";
  }
  
  Complex complex;

  // Read the cell_set in
  //ctl::read_complex( full_complex_name, complex);
 
  Complex_filtration complex_filtration( complex);
  std::cout << "Writing Filtration To: " << output_name << std::endl;
  std::ofstream out(output_name.c_str());
  std::size_t count = 0;
  for (Complex_filtration_iterator i = complex_filtration.begin(); i != complex_filtration.end(); ++i){
	out << (*i)->second.id() << " " << ++count << std::endl;
  }
  
  return 0;
}
