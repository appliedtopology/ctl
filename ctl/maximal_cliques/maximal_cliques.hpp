#ifndef CTL_MAXIMAL_CLIQUES_H
#define CTL_MAXIMAL_CLIQUES_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* Release under BSD-3 License See LICENSE
*******************************************************************************
* NOTES
* Based on original implementation by Afra Zomorodian.
*******************************************************************************/

// STL
#include <vector>              // storing R, P, X in sorted vectors
#include <functional>          // function
#include <iterator>            // back_insert_iterator
#include <algorithm>           // sort, binary_search, lower_bound, ...

// BGL
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace ctl {

namespace detail {
// Binary Predicate:  Degree_less
// For std::max_element in GX
template< class Graph>
class Degree_less : 
  public std::function< bool(
  typename boost::graph_traits< Graph>::vertex_descriptor,
  typename boost::graph_traits< Graph>::vertex_descriptor)>	{
private:
  typedef typename 
  boost::graph_traits< Graph>::vertex_descriptor vertex_descriptor;
public:
  Degree_less( const Graph& graph) : graph_( graph) {}
  bool operator()( vertex_descriptor v1, vertex_descriptor v2) {
    return out_degree( v1, graph_) < out_degree( v2, graph_);
  }
private:
  const Graph& graph_;
}; // struct Degree_less
 

// Algorithm IK_GX
// ---------------
template< class Graph, class CliqueOutputIterator, class Vector>
void IK_GX( const Graph& graph, CliqueOutputIterator &out,
	    Vector& R, Vector& P, Vector& X) {
  typedef boost::graph_traits< Graph>               graph_traits;
  typedef typename graph_traits::vertex_descriptor  vertex_descriptor; 
  typedef typename graph_traits::vertex_iterator    vertex_iterator;
  typedef typename graph_traits::adjacency_iterator adjacency_iterator;
  // Report R as a maximal clique
  if( P.empty() && X.empty()) { *out++ = R; } else {
    // Choose pivot using greedy heuristic GX
    vertex_descriptor pivot;
    GX( graph, P, X, pivot);
    for( auto ri = P.rbegin(); ri != P.rend();  ++ri) {
      auto u_i = *ri;
      // If u_i is not a neighbor of pivot
      if( !edge( pivot, u_i, graph).second) {
	
	// P = P - { u_i }
	P.erase( (ri+1).base());
	
	// R_new = R U { u_i }
	Vector R_new( R.begin(), R.end());
	auto i = std::lower_bound( R_new.begin(), R_new.end(), u_i);
	R_new.insert( i, u_i);
	
	// Sort N[ u_i] 
	adjacency_iterator nbhd_first, nbhd_last;
	tie( nbhd_first, nbhd_last) = adjacent_vertices( u_i, graph);
	Vector Nu_i( nbhd_first, nbhd_last);
	std::sort( Nu_i.begin(), Nu_i.end());

	// P_new = P \cap N[u_i]
	Vector P_new;
	std::set_intersection( Nu_i.begin(), Nu_i.end(),
			       P.begin(), P.end(),
			       std::back_insert_iterator< Vector>( P_new));

	// X_new = X \cap N[u_i]
	Vector X_new;
	std::set_intersection( Nu_i.begin(), Nu_i.end(),
			       X.begin(), X.end(),
			       std::back_insert_iterator< Vector>( X_new));

	// Recursive call
	detail::IK_GX( graph, out, R_new, P_new, X_new);
	
	// X = X U { u_i }
	i=std::lower_bound( X.begin(), X.end(), u_i);
	X.insert( i, u_i);
      }
    }
  }
}

// Algorithm GX
// Chooses vertex with highest degree in P U X
template< class Graph, class Vector>
void GX( const Graph& graph, Vector& P, Vector& X, 
	 typename boost::graph_traits< Graph>::vertex_descriptor& pivot) {

  typedef boost::graph_traits< Graph>               graph_traits;
  typedef typename graph_traits::vertex_descriptor  vertex_descriptor; 
  typedef typename graph_traits::degree_size_type   degree_size_type;

  Degree_less< Graph> degree_less( graph);

  // both cannot be empty (invariant from IK_GX)
  if( P.empty()) {
    pivot = *std::max_element( X.begin(), X.end(), degree_less);
    return;
  }
  if( X.empty()) {
    pivot = *std::max_element( P.begin(), P.end(), degree_less);
    return;
  }

  // neither is empty
  auto from_P = *std::max_element( P.begin(), P.end(), degree_less);
  auto from_X = *std::max_element( X.begin(), X.end(), degree_less);
  pivot = std::max( from_P, from_X, degree_less);
}

} // namespace detail 

// Procedure:  maximal_cliques 
// ---------------------------
// Given a BGL graph, outputs the cliques as vectors of
// vertex_descriptors in the output iterator
//
template< class Graph, class CliqueOutputIterator>
void maximal_cliques( const Graph& graph, CliqueOutputIterator &out) {
  typedef boost::graph_traits< Graph>               graph_traits;
  typedef typename graph_traits::vertex_descriptor  vertex_descriptor; 
  typedef typename graph_traits::vertex_iterator    vertex_iterator;
  typedef std::vector< vertex_descriptor>           Vector;

  // Trivial Case
  if( boost::num_vertices( graph) == 0) {  return; }

  // Initially, P = V(G)
  vertex_iterator first, last;
  tie( first, last) = vertices( graph);
  Vector P( first, last);
  
  //Maintain R, P, X as sorted vectors
  std::sort( P.begin(), P.end());
  
  // Initially, R = X = {}
  // Declared here as IK_GX parameters are by reference
  Vector R = {}, X = {};
  detail::IK_GX( graph, out, R, P, X);
} 

} // namespace detail

} // namespace ctl

#endif // CTL_MAXIMAL_CLIQUES_H
