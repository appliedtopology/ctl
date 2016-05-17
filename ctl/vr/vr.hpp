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
std::pair< ctl::Simplicial_complex<>, std::vector< double>>
vr(const ctl::Nbhd_graph<>& graph, std::size_t dimension){
	//TODO: glue in other algorithms
	ctl::Simplicial_complex<> complex;
	ctl::incremental_vr( graph, complex, dimension);
	std::vector< double> weights;
	weights.resize( complex.size());
	ctl::Complex_boundary< decltype(complex)> bd( complex);
	for(auto sigma = complex.begin(); sigma != complex.end(); ++sigma){
	 auto& wgt = weights[ sigma->second.id()];
	 if( sigma->first.dimension() == 1){
		const auto& ed = boost::edge(sigma->first.front(),sigma->first.back(),graph);
		wgt = boost::get(boost::edge_weight_t(), graph, ed.first);
	 	continue;
	 }
	 for( auto tau = bd.begin( sigma); tau != bd.end( sigma); ++tau){
	 	auto& cw = weights[ tau->cell()->second.id()];
	 	if (wgt < cw){ wgt = cw; } 
	 }
	}
	return std::make_pair(complex, weights);
}

template< typename Points>
decltype(auto)
vr(const Points& points, double epsilon, std::size_t dimension){
	//Simplex
	typedef typename ctl::Nbhd_graph<> Graph;
	//Chain Complex
	typedef ctl::Cell_complex< ctl::Simplex_boundary> Complex;
	Graph graph;
	Complex complex; 
	ctl::all_pairs::construct_graph(points, epsilon, graph);
	return ctl::vr( graph, dimension);
}

} //end namespace ctl
