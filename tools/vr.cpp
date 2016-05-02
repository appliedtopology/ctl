//STL
#include <iostream>
#include <fstream>

//CTL 
//Utility 
#include <ctl/io/io.hpp>
#include <ctl/utility/timer.hpp>


//Types for Points
//#include <ctl/points/points.h>

//Types for Graphs
#include <ctl/nbhd_graph/nbhd_graph.hpp>

//Types for Building a Simplicial Chain Complex 
//Abstract Simplex
#include <ctl/finite_field/finite_field.hpp>
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>

//Chain Complex
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/cell_complex/complex_boundary.hpp>
#include <ctl/term/term.hpp>

//VR Construction
#include <ctl/nbhd_graph/all_pairs.hpp>
#include <ctl/nbhd_graph/epsilon_search.hpp>
#include <ctl/vr/incremental_complex.hpp> 

//Graph

template< typename Points>
decltype(auto) compute_vr_complex(Points points, double epsilon, std::size_t dimension){
	//Simplex
	typedef typename ctl::Nbhd_graph<> Graph;
	//typedef ctl::Abstract_simplex Simplex;
	//typedef ctl::Finite_field< 2> Z2; 
	typedef ctl::Simplex_boundary Simplex_boundary;
	
	//Chain Complex
	typedef ctl::Cell_complex< Simplex_boundary> Complex;
	Graph graph;
	Complex complex; 
	ctl::epsilon_search::construct_graph(points, epsilon, graph);
	ctl::incremental_vr( graph, complex, dimension);
	return complex;
}
 

// temporary solution until we build points.h
typedef std::vector<double> Point;
typedef std::vector<Point> Points;

int main (int argc, char* argv[]) {
    if( argc != 4){ 	
	std::cerr << "Usage: " 
		  << argv[ 0]  
		  << " filename.dat max_epsilon max_dimension" << std::endl;
	return 0;
    } 
    ctl::Timer clock;
    double epsilon = atof( argv[ 2]);
    std::size_t dimension = atoi( argv[ 3]);

    Points points; 
    std::ifstream in;
    ctl::open_file( in, argv[ 1]);
    std::string output_file( argv[ 1]);
    auto pos = output_file.rfind('.');
    if(  pos != std::string::npos){
	output_file.erase(output_file.begin()+pos, output_file.end());
    }
    output_file.append(".asc");
    Point point;
    double coordinate;
    std::size_t line_num = 0;
    while( true){
	std::string line;
	ctl::get_line( in, line, line_num);
	if( in.eof() ) break;
	std::stringstream ss( line);
	while( ss.good()){ 
		ss >> coordinate;
		point.push_back( coordinate);
	}
	points.push_back( point); 
	point.clear();
    }
    clock.start();
    ctl::Nbhd_graph<> graph;
    ctl::Simplicial_complex<> complex; 
    ctl::epsilon_search::construct_graph(points, epsilon, graph);
    ctl::incremental_vr( graph, complex, dimension);
    std::cerr << "Complex construction: " << clock.elapsed() << std::endl;
    std::ofstream out( output_file.c_str());
    std::cerr << "|K| = " << complex.size() << std::endl;
    std::cout << "Writing to disk ... " << std::flush;
    //complex.write( out);
    std::cout << " complete!" << std::endl;

    return 0;	
}
