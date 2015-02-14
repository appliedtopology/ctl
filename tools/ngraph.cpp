//STL
#include <iostream>
#include <fstream>

//CTL 
//Utility 
#include <ctl/io/io.h>
#include <ctl/utility/timer.h>


//Types for Points
//#include <ctl/points/points.h>

//Types for Graphs
#include <ctl/nbhd_graph/nbhd_graph.h>
#include <ctl/nbhd_graph/epsilon_search.h>
#include <ctl/nbhd_graph/all_pairs.h>

//Types for Building a Simplicial Chain Complex 
//Abstract Simplex
#include <ctl/finite_field/finite_field.h>
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>

//Chain Complex
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/chain_complex/complex_boundary.h>
#include <ctl/term/term.h>

//Graph
typedef typename ctl::Nbhd_graph<> Graph;

//Simplex
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2; 
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_boundary;

//Chain Complex
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;


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

    Points points; 
    std::ifstream in;
    ctl::open_file( in, argv[ 1]);
    std::string output_file( argv[ 1]);
    auto pos = output_file.rfind('.');
    if(  pos != std::string::npos){
	output_file.erase(output_file.begin()+pos, output_file.end());
    }
    output_file.append(".ngh");
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
    ctl::epsilon_search::construct_graph(points, epsilon, graph);
    clock.stop();
    std::cerr << "# vertices = " <<  boost::num_vertices( graph) << std::endl;
    std::cerr << "# edges = " << boost:: num_edges( graph) << std::endl;
    std::cerr << "Graph construction: " << clock.elapsed() << std::endl;
    std::ofstream out( output_file.c_str());
    out << graph;
    out.close();
    return 0;	
}
