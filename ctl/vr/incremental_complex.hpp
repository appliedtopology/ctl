#ifndef CTL_INCREMENTAL_H
#define CTL_INCREMENTAL_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
* ********** BSD-3 License ****************
*******************************************************************************
* NOTES
* This is the construction of the Vietoris Rips Complex using the all pairs
* approach in the geometric phase and the incremental algorithm in the
* topological phase.
*******************************************************************************/

//STL
#include <vector> 
#include <algorithm>

//BOOST
#include <boost/graph/adjacency_list.hpp>

namespace ctl {

// Given a node, we loop through all its neighbors and return a vector
// of all the neighbors that have a lower ordering.
template<typename Graph, typename Vertex, typename Points> 
void get_lower_neighbors (const Graph& g,  
                 	  const Vertex& v,  
                 	  Points& lower_neighbors) {
    typedef typename boost::graph_traits<Graph>::adjacency_iterator Iterator;
    Iterator ai, ai_end;
    for( boost::tie(ai, ai_end) = boost::adjacent_vertices(v, g); 
	 ai != ai_end; ++ai) {
        if (*ai < v) { lower_neighbors.push_back( *ai); }
    } 
} 

// At every level in this recursive function, we introduce a new node to our
// simplicial complex and add the cofaces that arise from this new node.
template<typename Graph, typename Simplex, typename Neighbors, typename Complex>
void add_cofaces (const Graph& graph, Simplex& tau, 
		  const Neighbors& neighbors, 
		  Complex& complex, const std::size_t dimension) {
    typedef typename Neighbors::const_iterator Neighbor_iterator;
    complex.insert_open_cell(tau);
    if(tau.dimension() >= dimension) { return; }
    Neighbors lower_neighbors; 
    Neighbors final_neighbors; 
    for(Neighbor_iterator i = neighbors.begin(); i != neighbors.end(); ++i){
      lower_neighbors.clear();
      Simplex sigma( tau);
      sigma.insert( *i);
      get_lower_neighbors(graph, *i, lower_neighbors);
      final_neighbors.clear();
      set_intersection(lower_neighbors.begin(),lower_neighbors.end(),
                       neighbors.begin(),neighbors.end(),
                       back_inserter(final_neighbors));
      add_cofaces(graph, sigma, final_neighbors, complex, dimension); 
    }
} 

// We loop through all nodes, selecting each one as the starting point for
// constructing our simplicial complex.
template<typename Graph, typename Complex> 
void incremental_vr (const Graph& g, Complex& complex, std::size_t dimension) { 
    typedef typename Complex::Cell Simplex;
    typedef typename Simplex::value_type Vertex;
    typedef typename boost::graph_traits< Graph>::vertex_iterator vertex_iterator;
    vertex_iterator vi, vlast;
    for ( std::tie( vi, vlast) = boost::vertices( g); vi != vlast; ++vi) {
        std::vector< Vertex> neighbors;
        get_lower_neighbors(g, *vi, neighbors);
	Simplex tau(1, *vi); 
        add_cofaces( g, tau, neighbors, complex, dimension); 
    } 
}

} //end namespace CTL
#endif // CTL_INCREMENTAL_H
