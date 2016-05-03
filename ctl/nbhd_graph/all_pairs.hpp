#ifndef CTL_ALL_PAIRS_H
#define CTL_ALL_PAIRS_H
/*******************************************************************************
 Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
 ********** BSD-3 License ****************
*******************************************************************************
* NOTES
* We use the all pairs approach to constuct a graph. 
*
*******************************************************************************/

//STL 
#include <vector> 
#include <algorithm>

//BOOST
#include <boost/graph/adjacency_list.hpp>

namespace ctl{
namespace all_pairs{
template< typename It1, typename It2>
double lp( It1 begin1, It1 end1, It2 begin2){
	double r = 0.0;
	for( ; begin1 != end1; ++begin1, ++begin2){
		double v = (*begin1 - *begin2);
		r += v*v;
	}
	return r;
}



template<typename Points, typename Graph>
void construct_graph( const Points& points, 
		      const double epsilon,
		      Graph& graph) {
    typedef typename boost::graph_traits<Graph> graph_traits;
    typedef typename graph_traits::vertex_iterator vertex_iterator;
    typedef typename boost::graph_traits< Graph>::vertex_descriptor vertex_descriptor; 
    typedef typename boost::property_map< Graph,
					  boost::vertex_name_t>::type name_map_t;
    typedef typename boost::property_traits< name_map_t>::value_type  vertex_name_t;
    typedef std::unordered_map< vertex_name_t, vertex_descriptor> Name_to_descriptor_map;

    name_map_t name_map = boost::get( boost::vertex_name, graph);
    Name_to_descriptor_map descriptor( points.size());

    // add vertices
    for( std::size_t i = 0; i < points.size(); ++i) {
	vertex_descriptor v_descriptor = boost::add_vertex( graph);
	name_map[ v_descriptor] = i;
	descriptor.emplace( i, v_descriptor);
    }

    // add edges
    vertex_iterator vi, vj, vlast;
    double epsilon_squared = epsilon*epsilon;
    for ( std::tie( vi, vlast) = boost::vertices( graph); vi != vlast; ++vi) {
      for ( std::tie( vj, vlast) = boost::vertices (graph); vj != vi; ++vj) {
       
	if( lp(points.begin(name_map[*vi]), 
	       points.end(name_map[*vi]), 
	       points.begin(name_map[*vj])) < epsilon_squared){
             boost::add_edge(*vi, *vj, graph);
       }
      } 
    }
} 
} //end namespace all_pairs
} //end namespace CTL


#endif //CTL_ALL_PAIRS_H
