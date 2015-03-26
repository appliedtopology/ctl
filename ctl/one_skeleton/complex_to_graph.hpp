#ifndef CTL_OLD_ONE_SKEL_H_
#define CTL_OLD_ONE_SKEL_H_
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
*******************************************************************************
* ********** BSD-3 License ****************
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software without 
* specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
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
#include <ctl/io/io.hpp> 
#include <ctl/nbhd_graph/nbhd_graph.hpp>

namespace ctl {

// Private types

//just an abstraction so I don't need to write alot of code
template<typename Graph, typename vertex, 
	typename name_map_t, typename Inverse_map, typename vertex_descriptor>
bool insert_vertex( vertex_descriptor & descriptor,
		    const vertex& name,
                    Inverse_map& name_to_descriptor,
                    name_map_t& descriptor_to_name,
                    Graph & graph) {
        //typedef typename boost::graph_traits< Graph> graph_traits;
	typedef typename Inverse_map::iterator Inverse_map_iterator;

	//thus given one of the maps, we check the other one to 
	//make sure the vertex exists, if it doesn't we first insert it
	Inverse_map_iterator result = name_to_descriptor.find(name);
	if (result  == name_to_descriptor.end()){ 
		descriptor = add_vertex(graph); 
		descriptor_to_name[ descriptor] = name; 
		name_to_descriptor[ name] = descriptor;
		return true;
       	} 
	descriptor = result->second;
	return false;
}

template<typename Complex, typename Graph>
void complex_to_graph(Complex &complex, Graph &graph){
	typedef typename boost::property_map< Graph,
                        boost::vertex_name_t>::type name_map_t;	
	typedef typename boost::graph_traits< Graph> graph_traits; 
	typedef typename graph_traits::vertex_descriptor  vertex_descriptor;
	typedef typename boost::property_traits< name_map_t>::value_type 
							vertex_name_t;
	typedef typename std::map< vertex_name_t, vertex_descriptor> 
								Inverse_map;	
	Inverse_map to_descriptor;
	complex_to_graph( complex, graph, to_descriptor);
}

template<typename Complex, typename Graph, typename Inverse_Map>
void complex_to_graph(Complex &c, Graph &g, Inverse_Map to_descriptor){	
	//typedef typename boost::property_map< Graph, 
			 //boost::vertex_name_t>::const_type const_name_map_t;
	typedef typename boost::property_map< Graph, 
			boost::vertex_name_t>::type name_map_t; 
	typedef typename boost::graph_traits< Graph> graph_traits;
	typedef typename graph_traits::vertex_descriptor  vertex_descriptor;
	typedef typename boost::property_traits< name_map_t>::value_type 
								vertex_name_t;
	typedef typename Complex::const_iterator const_iterator;
	typedef typename Complex::Cell Cell;

	name_map_t name_map = get( boost::vertex_name, g);
	
	for (const_iterator i = c.begin(); i != c.end(); ++i){
		const Cell& cell = i->first;
		switch(cell.dimension()){
			case 0: 
			{
				#ifdef BONESKEL
				std::cout << "Attempting to insert vertex " 
				<< cell << std::endl;
 				#endif
		        	vertex_name_t vertex = *cell.begin();
				vertex_descriptor vert_descriptor;
				ctl::insert_vertex(
						  vert_descriptor,
						  vertex, 
						   to_descriptor, 
						   name_map, 
							g);
				#ifdef BONESKEL	
				std::cout << "Vertex "<< cell <<  " inserted! " << std::endl;
				#endif
			break;	
			}
			case 1:
			{
		        	vertex_name_t source_name = *cell.begin();
				vertex_name_t target_name = *(++cell.begin());
				vertex_descriptor source_descriptor,
						  target_descriptor;
				#ifdef BONESKEL	
				bool vtx_added = false;
                	        vtx_added = 
				#endif
				insert_vertex( source_descriptor,
					       source_name,
					       to_descriptor,
					       name_map, g);
				#ifdef BONESKEL	
                        	vtx_added |= 
				#endif
				insert_vertex( target_descriptor,
					       target_name,
					       to_descriptor,
					       name_map, g); 
				//Now check if the edge exists
				//If it doesn't add the edge
				if (!boost::edge( source_descriptor, 
						  target_descriptor ,g).second){
		
				#ifdef BONESKEL
				std::cout << "Adding Edge: " 
					  << cell << std::endl;
				std::cout << "vert added: " 
					  << vtx_added << std::endl;
				std::cout << "source: " 
					  << source_descriptor << std::endl;
				std::cout << "target: " 
					  << target_descriptor << std::endl;
				#endif
				boost::add_edge( source_descriptor, 
					         target_descriptor, g);
				#ifdef BONESKEL
				std::cout << "Success: " << cell << std::endl;
				#endif	
				}

			break;
			}
			default: 
			{
				continue;
			}
		}
	}
}

template< typename vertex> 
static bool edge_equal (vertex v_source, vertex v_target, 
			vertex u_source, vertex u_target){
	bool a = (v_source == u_source) && (v_target == u_target); 
	bool b = (v_target == u_source) && (v_source == u_target); 
	return a || b;
}

template< typename Graph>
static bool graph_includes ( const Graph &g, const Graph &h){
	typedef boost::graph_traits< Graph> graph_traits;
	typedef typename graph_traits::vertex_iterator vertex_iterator;
	typedef typename graph_traits::edge_iterator edge_iterator;	
        typedef typename graph_traits::vertex_descriptor vertex_descriptor;
	vertex_iterator gi, hi, lastg,lasth;
	edge_iterator e1, h2, lastge,lasthe;

	for (tie(gi,lastg) = vertices( g); gi != lastg; ++gi){
		bool found_v = false;
		for (tie(hi,lasth) = vertices( h); hi != lasth; ++hi){
			if (*gi == *hi){
				found_v = true;
				break;
			}
		}
		if (!found_v){
		std::cout << std::endl;
		return false;
		}
	}


	for (tie(e1,lastge) = edges( g); e1 != lastge; ++e1){
		bool found_edge = false;
		vertex_descriptor e1_s = boost::source(*e1,g);
		vertex_descriptor e1_t = boost::target(*e1,g);
                for (tie(h2,lasthe) = edges( h); h2 != lasthe; ++h2){
			vertex_descriptor h2_s = boost::source(*h2,h);
			vertex_descriptor h2_t = boost::target(*h2,h);
			if(edge_equal(e1_s,e1_t,h2_s,h2_t)){
				found_edge = true;
				break;
			}
		}

		if (!found_edge){
			return false;
		}
        }
	return true;	
}

template< typename Graph> 
static bool graph_equals( const Graph &g, const Graph &h){
	return graph_includes(g,h) && graph_includes(h,g);
}

}
#endif //CTL_OLD_ONE_SKEL_H_
