/*******************************************************************************
******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
******************************************************************************
* BSD-3
*******************************************************************************/
//CTL Headers

//Utility 
#include <ctl/io/io.hpp>
#include <ctl/utility/timer.hpp>
#include <ctl/parallel/utility/timer.hpp>

//#define COMPUTE_BETTI
//#define CTL_USE_MURMUR
#define CTL_USE_CITY
//#define CTL_USE_JENKINS
//#define COMPLEX_DIAGNOSTICS
//#define ZOOM_PROFILE
#ifdef ZOOM_PROFILE
#include "zoom.h"
#endif

//CTL 
//Types for Building a Simplicial Chain Complex and Filtration
//Abstract Simplex
#include <ctl/finite_field/finite_field.hpp>
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>

//Filtration & Filtration Boundary Operator
#include <ctl/graded_chain_complex/graded_cell_complex.hpp>
#include <ctl/graded_chain_complex/graded_boundary.hpp>
#include <ctl/graded_chain_complex/less.hpp>

//Weights
#include <ctl/weight_data/weight_data.hpp>
#include <ctl/weight_data/weight_functor.hpp>

//Chain Complex
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/cell_complex/complex_boundary.hpp>
#include <ctl/term/term.hpp>


//Chains & Persistence 
#include <ctl/chain/chain.hpp>
#include <ctl/persistence/persistence.hpp>
#include <ctl/matrix/iterator_property_map.hpp>
#include <ctl/matrix/offset_maps.hpp>
#include <ctl/matrix/matrix.hpp>
#include <ctl/persistence/compute_betti.hpp>
#include <ctl/persistence/compute_barcodes.hpp>

//Barcodes
#include <ctl/persistence/barcodes/barcodes.hpp>

//STL
#include <sstream>


//Simplex
typedef ctl::Abstract_simplex Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary Simplex_boundary;

//Chain Complex
typedef ctl::Cell_complex< Simplex_boundary> Complex;

//Chain Complex with weights
typedef ctl::Weight_data< double> Weight_data;
typedef ctl::Cell_complex< Simplex_boundary, Weight_data> Weighted_complex;

//Filtration order
typedef ctl::Cell_less Complex_cell_less;
typedef ctl::Weight_less Complex_weight_less;

template< typename Complex, typename Cell_less, 
	  typename Barcode, typename Timer, 
	  typename Tag>
void run_persistence( Complex & complex, 
		      Cell_less & less, 
		      Barcode & barcode,
 		      Timer & timer,
		      const Tag tag){
   typedef ctl::Graded_cell_complex< Complex, Cell_less > Filtration;
   //Boundary Operator
   //NOTE: This is not a general purpose boundary operator.
   //It works correctly only when successive 
   //boundaries are taken in a filtration order
   //typedef typename Filtration::iterator Filtration_iterator;
   typedef ctl::Graded_boundary< Filtration> Filtration_boundary;
   typedef typename Filtration::Term Filtration_term;
   typedef typename Filtration_term::Coefficient Coefficient;

   typedef typename ctl::Sparse_matrix< Coefficient> Sparse_matrix;
   typedef typename ctl::Offset_map< typename Filtration::iterator> Offset_map;
   typedef typename ctl::Sparse_matrix_map< Coefficient, Offset_map> Chain_map;

   //produce a filtration
   timer.start();
   Filtration complex_filtration( complex);
   Filtration_boundary filtration_boundary( complex_filtration);
   timer.stop();
   double complex_filtration_time = timer.elapsed();
   //display some info
   std::cout << "time to compute complex filtration (secs): " 
             << complex_filtration_time << std::endl;
   
   //begin instantiate our vector of cascades homology
   Sparse_matrix R( complex.size());
   
   Offset_map offset_map( complex_filtration.begin());
   //we hand persistence a property map for genericity!                        
   Chain_map R_map( R.begin(), offset_map);
   timer.start();
   auto times = ctl::persistence( complex_filtration.begin(), 
				  complex_filtration.end(),
  		    		  filtration_boundary, 
				  R_map, false, offset_map);
   timer.stop();
   double boundary_map_build = times.first;
   double complex_persistence = times.second;
  std::cout << "initialize_cascade_data (complex): " 
            << boundary_map_build << std::endl;
  std::cout << "serial persistence (complex): " 
            << complex_persistence << std::endl;
  std::cout << "total time : " << timer.elapsed() << std::endl;

   ctl::compute_barcodes( complex_filtration, 
  			  R_map, barcode, tag, true);
  std::vector< std::size_t> bti;
  compute_betti( complex_filtration, R_map, bti, true);
}
typedef ctl::parallel::Timer Timer;

template<typename String>
void process_args(int & argc, char *argv[], 
		  String & filename, String & filtration){
  std::string usage = "Usage: persistence input-space.asc [filtration.flt]";
  if (argc != 2 && argc != 3){ std::cerr << usage << std::endl; exit( 1); }
  filename = std::string( argv[ 1]);
  if( argc == 2){
    filtration = filename;
    std::size_t pos = filtration.rfind('.');
    if (pos != std::string::npos){ filtration.erase(pos); }
    filtration.append(".flt");
    return;
  }else { filtration = std::string( argv[ 2]); }
  std::cout << "filename: " << filename << std::endl;
  std::cout << "filtration filename: " << filtration << std::endl;

}

template< typename String>
bool can_read_weights( const String & filename){ 
	 std::ifstream infile(filename.c_str());
	return infile.good();
}

int main(int argc, char *argv[]){
  std::string full_complex_name, filtration_file;
  process_args(argc, argv, full_complex_name, filtration_file);
  
  std::string barcode_file_name( full_complex_name); 
  std::size_t pos = barcode_file_name.rfind( '.');
  if( pos != std::string::npos){
	barcode_file_name.replace(pos, barcode_file_name.length(), ".bcs");
  }
  std::ofstream out; 
  ctl::open_file( out, barcode_file_name.c_str());
  //Create some data structures 
  Timer timer;
  timer.start();
  if( can_read_weights( filtration_file)){
 	//ctl::Weight_data_functor< Weighted_complex> weight_functor;
	typedef typename ctl::Barcodes< double> Barcodes;
	Barcodes barcodes;
  	Weighted_complex complex;
  	//ctl::read_complex_and_data( full_complex_name, filtration_file, 
	//			    complex, weight_functor); 
  	timer.stop();
  	std::cout << "I/O Time: " << timer.elapsed() << std::endl;
  	std::cout << "complex size: " << complex.size() << std::endl; 
  	std::cout << "complex dimension: " << complex.dimension() << std::endl;
	Complex_weight_less less;
	run_persistence( complex, less, barcodes, timer, 
			 ctl::detail::weighted_tag());
	out << barcodes << std::endl;

  }else{
	typedef typename ctl::Barcodes< double> Barcodes;
	Barcodes barcodes;
  	Complex complex;
  	//ctl::read_complex( full_complex_name, complex);
  	timer.stop();
  	std::cout << "I/O Time: " << timer.elapsed() << std::endl;
	std::cout << "complex size: " << complex.size() << std::endl; 
  	std::cout << "complex dimension: " << complex.dimension() << std::endl;
	Complex_cell_less less;
  	run_persistence( complex, less, barcodes, timer, 
			 ctl::detail::non_weighted_tag());   
	out << barcodes << std::endl;
  } 
  return 0;
}
