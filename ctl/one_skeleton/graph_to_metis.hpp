#ifndef CTL_GRAPH_TO_METIS_H
#define CTL_GRAPH_TO_METIS_H
/*******************************************************************************
 Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
******************************************************************************/
// STL
#include <algorithm> // not sure if algorithm,sstream are necessary yet
#include <sstream>
#include <iterator> 
#include <map> 
#include <utility>
#include <iostream>

// BGL
#include <boost/graph/adjacency_list.hpp> 
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

// PROJECT
#include <ctl/io/io.hpp> 
#include <ctl/nbhd_graph/nbhd_graph.hpp>

namespace ctl {

template<typename OutputStream, typename offsets_t, typename neighbors_t>
void write_metis( OutputStream & out, 
		  offsets_t & offsets, 
		  neighbors_t & neighbors){
  int num_vertices = offsets.size() - 1;
  int num_edges = neighbors.size()/2.0;
  out << num_vertices << " " << num_edges << " " << "0" << std::endl;
  for (int i = 0; i < num_vertices; ++i){
        for (int j = offsets[i]; j < offsets[i+1]; ++j){
                out << neighbors[j] << " ";
        } 
        out << std::endl;
  }
}


template< typename Graph, 
	  typename Index_to_vertex_map, 
	  typename Vertex_to_index_map, 
	  typename Vector>
void graph_to_metis( Graph & graph, 
		     Vector & offsets, 
		     Vector & neighbors,
		     Vertex_to_index_map & vertex_to_index, 
		     Index_to_vertex_map & index_to_vertex){ 
	typedef typename boost::graph_traits< Graph> graph_traits;
	typedef typename graph_traits::vertex_iterator Vertex_iterator;
	typedef typename graph_traits::vertices_size_type Vertices_size_t;
	
	typedef typename boost::property_map< Graph,
			 boost::vertex_name_t>::type Name_map;

	typedef typename boost::property_traits< Name_map>::value_type 
						           Vertex_name;
	typedef typename boost::graph_traits<Graph>::adjacency_iterator 
							Adjacency_iterator;
	typedef typename std::pair<Adjacency_iterator,Adjacency_iterator> Pair;
	//typedef typename Vertex_to_index_map::value_type Index;
	Name_map name_map = boost::get( boost::vertex_name, graph);

        Vertex_iterator vi, vlast;
        //CSR calls each vertex 1 ... n
	Vertices_size_t count = 0;
        for (std::tie( vi, vlast) = vertices( graph); vi != vlast; ++vi, ++count){
                Vertex_name name = boost::get( name_map, *vi);
                vertex_to_index[ name] = count;
                index_to_vertex[ count] = name;
        }
	//CSR stores all edges in a length 2*numedges array, (it stores each
	//edge twice (u,v) and (v,u)
	//edges (i,j) are in the range 
	//neighbors[offsets[i]] ... neighbors[offsets[i+1]-1]
	offsets[ 0] = 0;
	size_t index = 0;
	for (std::tie(vi, vlast) = vertices( graph); vi != vlast; ++vi){
                Pair edge = boost::adjacent_vertices(*vi, graph);
		Vector local;
		//assumes this dist() makes sense..
		local.reserve( std::distance( edge.first, edge.second));
		for ( Adjacency_iterator u = edge.first; 
					 u != edge.second; u++){
                	Vertex_name v_name = boost::get( name_map,*u);
			local.push_back( vertex_to_index[ v_name]+1);
                }
		//why not keep things sorted..?
		std::sort( local.begin(), local.end());
		std::copy( local.begin(), local.end(), 
			   neighbors.begin()+offsets[ index++]);
                offsets[ index] = offsets[index-1] + 
					std::distance( edge.first, edge.second);
        }
}
}
#endif // CTL_GRAPH_TO_METIS_H
