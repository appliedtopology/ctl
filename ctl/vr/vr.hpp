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
//#include <ctl/nbhd_graph/epsilon_search.hpp>
#include <ctl/vr/incremental_complex.hpp> 

namespace ctl {

template< typename It1, typename It2>
double lp( It1 begin1, It1 end1, It2 begin2, It2 end2){
	double r = 0.0;
	for( ; begin1 != end1; ++begin2, ++begin2){
		double v = (*begin1 - *begin2);
		r += v*v;
	}
	return r;
}

template< typename Points>
ctl::Cell_complex< ctl::Simplex_boundary> 
vr(const Points& points, double epsilon, std::size_t dimension){
	//Simplex
	typedef typename ctl::Nbhd_graph<> Graph;
	
	//Chain Complex
	typedef ctl::Cell_complex< ctl::Simplex_boundary> Complex;
	Graph graph;
	Complex complex; 
	ctl::all_pairs::construct_graph(points, epsilon, graph);
	//TODO: glue in other algorithms
	ctl::incremental_vr( graph, complex, dimension);
	return complex;
}

} //end namespace ctl
