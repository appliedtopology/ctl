/*******************************************************************************
******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
******************************************************************************
* BSD-3
*******************************************************************************/
// STL
#include <iostream> 
#include <fstream>
#include <utility>

// Project
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>
#include <ctl/nbhd_graph/nbhd_graph.hpp>
#include <ctl/one_skeleton/graph_to_metis.hpp>
#include <ctl/io/io.hpp>

typedef ctl::Abstract_simplex Simplex;
typedef ctl::Finite_field< 2> Z2; 
typedef ctl::Simplex_boundary Simplex_boundary;
typedef ctl::Cell_complex<Simplex_boundary> Complex;
typedef Complex::Cell Cell;

typedef  ctl::Nbhd_graph<> Graph;
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
  Graph graph;

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
