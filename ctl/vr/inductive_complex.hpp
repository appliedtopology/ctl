#ifndef CTL_INDUCTIVE_H
#define CTL_INDUCTIVE_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
* Copyright (C) Amr Mohamed 2014 <amr1@stanford.edu>
*******************************************************************************
* NOTES
* This is the construction of the Vietoris Rips Complex using the all pairs
* approach in the geometric phase and the inductive algorithm in the
* topological phase.
*******************************************************************************/

//STL
#include <set>
#include <vector> 
#include <algorithm>

//BOOST
#include <boost/graph/adjacency_list.hpp>

//CTL
#include <ctl/vr/incremental_complex.hpp>

namespace ctl {

// We add the given graph to the complex. Also, we update the cells vectors to
// keep track of the dimensions of the cells.
template<typename Graph, typename Complex, typename Vector>
void add_graph_to_complex (const Graph& g,
			   Complex& complex,
			   Vector& k_cells,
			   Vector& k_plus_one_cells) {
    typedef typename Complex::Cell Simplex;
    typedef typename boost::graph_traits< Graph>::vertex_iterator vertex_iterator;
    typedef typename boost::graph_traits< Graph>::adjacency_iterator Iterator;
    vertex_iterator vi, vlast;
    for ( std::tie( vi, vlast) = boost::vertices( g); vi != vlast; ++vi) {
        // adding {*vi} to both k_cells and complex
        Simplex tau(1, *vi);
        k_cells.push_back(tau);
        complex.insert_open_cell(tau);
        Iterator ai, ai_end;
        for ( boost::tie(ai, ai_end) = boost::adjacent_vertices(*vi, g);
              ai != ai_end; ++ai) {
                // adding {*vi, *ai} to both k_plus_one_cells and complex
                Simplex sigma( tau);
                sigma.insert( *ai);
                k_plus_one_cells.push_back(sigma);
                complex.insert_open_cell(sigma);
        }
    }
}

// We loop through all nodes, selecting each one as the starting point for
// constructing our simplicial complex.
template<typename Graph, typename Complex> 
void inductive_vr (const Graph& g, Complex& complex, std::size_t dimension) { 
    typedef typename Complex::Cell Simplex;
    typedef typename Simplex::value_type Vertex;
    std::vector<Simplex> k_cells;
    std::vector<Simplex> k_plus_one_cells;
    // adding the graph to the complex
    add_graph_to_complex (g, complex, k_cells, k_plus_one_cells);
    // inductive algorithm
    std::vector< Vertex> intersect;
    for ( int k = 1; k < dimension; k++) {
        k_cells.swap(k_plus_one_cells);
	k_plus_one_cells.clear();
	for( const auto & tau: k_cells){
	    // getting the intersection of all lower neighbors
	    std::vector< Vertex> final_neighbors;
            std::vector< Vertex> lower_neighbors;
	    ctl::get_lower_neighbors(g, *(tau.begin()), final_neighbors);
	    for( auto & vi: tau) {
		if( final_neighbors.empty()) { break; }
		lower_neighbors.clear();
		ctl::get_lower_neighbors( g, vi, lower_neighbors);
		//std::vector< Vertex> intersect;
		set_intersection(lower_neighbors.begin(),
				 lower_neighbors.end(),
				 final_neighbors.begin(),
				 final_neighbors.end(),
				 std::back_inserter(intersect));
		final_neighbors.swap(intersect);
		intersect.clear();
		//final_neighbors.clear();
		//std::copy (intersect.begin(),intersect.end(),back_inserter(final_neighbors));
	    }
	    // constructing new simplices and adding them to the complex
    	    for( int j = 0; j < final_neighbors.size(); j++) {
		Simplex sigma( tau);
		sigma.insert( final_neighbors[j]); 
		k_plus_one_cells.push_back(sigma);
		complex.insert_open_cell(sigma);
	    }
	}
    }
}

} //end namespace CTL
#endif // CTL_INDUCTIVE_H
