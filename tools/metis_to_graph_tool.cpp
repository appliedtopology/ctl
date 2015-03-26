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
#include <unordered_map>
#include <iostream> 
#include <fstream>
#include <utility>

//BOOST
#include <boost/throw_exception.hpp>
#include <boost/graph/metis.hpp>

// CTL
#include <ctl/nbhd_graph/nbhd_graph.hpp>
#include <ctl/io/io.hpp>
#include <ctl/one_skeleton/graph_to_metis.hpp>
#include <ctl/one_skeleton/complex_to_graph.hpp>

typedef  ctl::Nbhd_graph<> Graph;
typedef  boost::property_map< Graph,boost::vertex_name_t>::const_type 
							  const_name_map_t;
typedef  boost::property_map< Graph,boost::vertex_name_t>::type name_map_t;
typedef  boost::graph_traits< Graph> graph_traits;
typedef  graph_traits::vertex_descriptor  vertex_descriptor;
typedef  boost::property_traits< name_map_t>::value_type vertex_name_t;
typedef  std::unordered_map< vertex_name_t, vertex_descriptor> Inverse_map;
typedef  graph_traits::vertex_descriptor  vertex_descriptor;

int main(int argc, char *argv[])
{
  if( argc != 2) {
    std::cerr << "Usage:  " << argv[ 0] << " metis-graph" << std::endl;
    std::exit(1);
  }
  // Read the graph in 
  std::ifstream in(argv[ 1]);   
  boost::graph::metis_reader reader( in);
  std::string index_name ( argv[ 1]);
  size_t loc = index_name.rfind('.');
  if (loc == std::string::npos){
        index_name.append(".idxmet");
  }else{
        index_name.replace(loc,index_name.size()-1,".idxmet");
  }

  typedef boost::graph::metis_reader::edge_iterator edges;
  typedef std::set<edges::value_type> edge_set;
  typedef std::set<edges::value_type::first_type> vertex_set;
  typedef vertex_set::value_type vertex_t;

  //read in edges
  edge_set my_edges;
  my_edges.insert(reader.begin(),reader.end());

  std::ifstream index_in(index_name.c_str());
  std::map<vertex_t,vertex_t> index_to_vertex_map; 
  if (index_in.is_open()){
	vertex_t idx,vertx;
        while(!index_in.eof()){
                index_in >> idx >> vertx;
		index_to_vertex_map.insert(std::pair<vertex_t,vertex_t>(idx,
									vertx));
        }
  }

  //create graph
  Graph graph;
  name_map_t name_map = get(boost::vertex_name, graph);
  Inverse_map to_descriptor;

  vertex_set my_verts;
  for ( edge_set::iterator it = my_edges.begin(); it != my_edges.end(); ++it){
		vertex_descriptor vertex_u_desc, vertex_v_desc;
		bool found_a = (index_to_vertex_map.find(it->first+1) 
						!= index_to_vertex_map.end());
		bool found_b = (index_to_vertex_map.find(it->second+1) 
						!= index_to_vertex_map.end());
		vertex_name_t vertex_u_name = (found_a) ? 
			index_to_vertex_map[it->first+1] : it->first+1;
		vertex_name_t vertex_v_name = (found_b) ? 
			index_to_vertex_map[it->second+1] : it->second+1;
		ctl::insert_vertex(vertex_u_desc,vertex_u_name,
					   to_descriptor,name_map,graph);
		ctl::insert_vertex(vertex_v_desc,vertex_v_name,
					   to_descriptor,name_map,graph);
		if (!boost::edge(vertex_u_desc,vertex_v_desc,graph).second){
			boost::add_edge(vertex_u_desc,vertex_v_desc,graph);
		}
  }
  std::ofstream out;
  std::string graph_name ( argv[ 1]);
  size_t found = graph_name.rfind( '.');
  if (found == std::string::npos){
        graph_name.append(".ngh");
  }else{
        graph_name.replace(found,graph_name.size()-1,".ngh");
  }
  ctl::open_file( out,graph_name.c_str());
  out << graph;
  ctl::close_file( out);
 return 0;
}
