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
#include <iostream> 
#include <fstream>
#include <utility>

// Project
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>
#include <ctl/nbhd_graph/nbhd_graph.h>
#include <ctl/one_skeleton/graph_to_metis.h>
#include <ctl/io/io.h>

typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2; 
typedef ctl::Simplex_boundary< Simplex, Z2> Simplex_boundary;
typedef ctl::Chain_complex< Simplex, Simplex_boundary> Complex;
typedef Complex::Cell Cell;

typedef  ctl::Nbhd_graph Graph;
typedef  boost::graph_traits< Graph>::edges_size_type edges_size_type;
typedef  boost::vertex_name_t bvtx_name_t;
typedef  boost::property_map< Graph, bvtx_name_t>::type name_map_t;
typedef  boost::property_traits<name_map_t>::value_type vertex_name_t;


int main(int argc, char *argv[])
{
  if( argc != 2) {
    std::cerr << "Usage:  " << argv[ 0] << " graph" << std::endl;
  }
  // Set up complex
  ctl::Nbhd_graph graph;

  // Read the complex in 
  std::ifstream in;   
  ctl::open_file( in, argv[ 1]);   
  in >> graph;  
  ctl::close_file( in);

  int num_vertices = boost::num_vertices(graph);
  edges_size_type num_edges = boost::num_edges(graph);

  std::vector< int> offsets(num_vertices+1);
  std::vector< int> neighbors(2*num_edges);
  std::vector< int> part(num_vertices);

  //int options[10];
  //options[0] = 0;

  //TODO: hash_map? 
  std::map< vertex_name_t, int> vertex_to_idx;
  std::map< int, vertex_name_t> idx_to_vertex;
  
  //extract graph
  ctl::graph_to_metis( graph, offsets, neighbors, vertex_to_idx, idx_to_vertex);
  std::ofstream out;
  std::string metis_name ( argv[ 1]);
  size_t found = metis_name.rfind( '.');
  if (found == std::string::npos){
        metis_name.append( ".met");
  }else{
        metis_name.replace( found, metis_name.size()-1,".met");
  }
  ctl::open_file( out, metis_name.c_str());
  ctl::write_metis( out, offsets, neighbors);
  ctl::close_file( out);
 return 0;
}
