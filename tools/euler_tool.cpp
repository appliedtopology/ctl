// euler_tool.cpp
// Ryan Lewis
// November 10, 2011
//

// Global Project Deps
#include "abstract_simplex/abstract_simplex.h"
#include "abstract_simplex/simplex_boundary.h"
#include "chain_complex/chain_complex.h"
#include "chain_complex/chain_complex_io.h"
#include "io/io.h" 
#include "utility/timer.h"

//boost
#include <boost/program_options.hpp>
namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2; 
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_boundary;
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;
typedef Complex::iterator Complex_iterator;
typedef ctl::Timer Timer;


template<typename Variable_map>
void process_args( int & argc, char *argv[],Variable_map & vm){
  //parse command line options
  po::options_description desc( "Usage: tp [options] input-file");
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

  size_t found = complex_name.rfind( '/');
  if ( found != std::string::npos){
        complex_name.replace( 0, found+1, "");
  }
  
  Complex complex;
  Timer timer;
  // Read the cell_set in
  ctl::read_complex( full_complex_name, complex);
  int euler=0;
  timer.start();
  for(auto cell : complex){ euler += 2*(cell.first.dimension()%2)-1; }
  timer.stop();
  std::cout << "Euler characteristic: " << euler << " : " 
            << timer.elapsed() << " secs" << std::endl;
  return 0;
}
