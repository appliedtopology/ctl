#ifndef CTL_GRAPH_TO_METIS_H
#define CTL_GRAPH_TO_METIS_H
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
* \cite{hatcher, z-fcv-10a}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program in a file entitled COPYING; if not, write to the 
* Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************************
*******************************************************************************/
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
#include <ctl/io/io.h> 
#include <ctl/nbhd_graph/nbhd_graph.h>

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
	typedef typename Vertex_to_index_map::value_type Index;
	Name_map name_map = boost::get( boost::vertex_name, graph);

        Vertex_iterator vi, vlast;
        //CSR calls each vertex 1 ... n
	Vertices_size_t count = 0;
        for (tie( vi, vlast) = vertices( graph); vi != vlast; ++vi, ++count){
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
