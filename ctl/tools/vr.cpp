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

//Types for Building a Simplicial Chain Complex 
//Abstract Simplex
#include <ctl/finite_field/finite_field.h>
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>

//Chain Complex
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/chain_complex/complex_boundary.h>
#include <ctl/term/term.h>

//VR Construction
#include <ctl/nbhd_graph/all_pairs.h>
#include <ctl/vr/incremental_complex.h> 

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
    Graph graph;
    ctl::all_pairs::construct_graph(points, epsilon, graph);
    clock.stop();
    std::cerr << "# vertices = " <<  boost::num_vertices( graph) << std::endl;
    std::cerr << "# edges = " << boost:: num_edges( graph) << std::endl;
    std::cerr << "Graph construction: " << clock.elapsed() << std::endl;
    clock.start();
    Complex complex; 
    ctl::incremental_vr( graph, complex, dimension);
    clock.stop();
    std::ofstream out( output_file.c_str());
    std::cerr << "Complex construction: " << clock.elapsed() << std::endl;
    std::cerr << "|K| = " << complex.size() << std::endl;
    std::cout << "Writing to disk ... " << std::flush;
    complex.write( out);
    std::cout << " complete!" << std::endl;

    return 0;	
}
