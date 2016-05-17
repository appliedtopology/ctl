#define CTL_USE_CITY
//STL
#include <iostream>
#include <fstream>

//CTL 
//Utility 
#include <ctl/io/io.hpp>
#include <ctl/utility/timer.hpp>

//Types for Graphs
#include <ctl/nbhd_graph/nbhd_graph.hpp>

//Types for Building a Simplicial Chain Complex 
//Abstract Simplex
#include <ctl/finite_field/finite_field.hpp>
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>

//Chain Complex
#include <ctl/chain_complex/chain_complex.hpp>
#include <ctl/chain_complex/complex_boundary.hpp>
#include <ctl/term/term.hpp>

//VR Construction
#include <ctl/nbhd_graph/all_pairs.hpp>
#include <ctl/vr/incremental_complex.hpp> 
#include <ctl/vr/inductive_complex.hpp>

//Graph
typedef typename ctl::Nbhd_graph<> Graph;

//Simplex
typedef ctl::Abstract_simplex Simplex;
typedef ctl::Finite_field< 2> Z2; 
typedef ctl::Simplex_boundary Simplex_boundary;

//Chain Complex
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;


// temporary solution until we build points.h
typedef std::vector<double> Point;
typedef std::vector<Point> Points;

int main (int argc, char* argv[]) {
    if( argc != 5){ 	
	std::cerr << "Usage: " << argv[ 0] 
		  << " filename.dat epsilon dimension option" << std::endl;
	return 0;
    } 
    ctl::Timer clock;
    double epsilon = atof( argv[ 2]);
    std::size_t dimension = atoi( argv[ 3]);
    std::size_t option = atoi( argv[ 4]);
    if( option >= 2){
	std::cerr << "Usage: please enter a valid option:" << std::endl
		  << "0 for incremental construction," << std:: endl
		  << "1 for inductive construstion." << std::endl;
	return 0;
    }
    Points points; 
    std::ifstream in;
    ctl::open_file( in, argv[ 1]);
 
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
    Graph graph;
    ctl::all_pairs::construct_graph(points, epsilon, graph);
    clock.stop();
    std::cerr << "Graph: " <<  boost::num_vertices( graph) << ", "
	      << boost:: num_edges( graph) << std::endl;
    std::cerr << "Graph construction: " << clock.elapsed() << std::endl;
    clock.start();
    Complex complex; 
    if( option == 0) { ctl::incremental_vr(graph, complex, dimension);}
    else if( option == 1) { ctl::inductive_vr(graph, complex, dimension);}
    clock.stop();
    std::cerr << "Complex construction: " << clock.elapsed() << std::endl;
    std::cerr << "Number of simplices: " << complex.size() << std::endl;
    std::cerr << "dimension of complex: " << complex.dimension() << std::endl;
    return 0;	
}

