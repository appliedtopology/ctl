#ifndef CTL_CONSTRUCT_GRAPH_H
#define CTL_CONSTRUCT_GRAPH_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the license below, but, please
* do not commit acts of academic dishonesty. We strongly encourage and request 
* that for any [academic] use of this source code one should cite one the 
* following works:
* 
* \cite{hatcher, z-ct-10}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************************
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

template<typename Point>
double l2_distance(const Point& one, const Point& two) {
    double result = 0;
    for ( int i = 0; i < one.size(); ++i) {
	result += ((one[i] - two[i]) * (one[i] - two[i]));
    }
    return result;
}


template<typename Points, typename Graph>
void construct_graph(const Points& points, const double epsilon, Graph& graph) {
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
    for ( std::tie( vi, vlast) = boost::vertices( graph); vi != vlast; ++vi) {
	for ( std::tie( vj, vlast) = boost::vertices (graph); vj != vi; ++vj) {
	    if(l2_distance(points[name_map[*vi]], points[name_map[*vj]]) <= (epsilon * epsilon)) {
		boost::add_edge(*vi, *vj, graph);
	    }
	} 
    }
} 

} //end namespace CTL


#endif //CTL_CONSTRUCT_GRAPH_H
