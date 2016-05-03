/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
******************************************************************************
* BSD-3
*******************************************************************************/
// STL
#include <iostream> 
#include <fstream>
#include <utility>
#include <vector>
// Project
#include <ctl/finite_field/finite_field.hpp>
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/nbhd_graph/nbhd_graph.hpp>
#include <ctl/one_skeleton/complex_to_graph.hpp>
#include <ctl/one_skeleton/one_skeleton.hpp>
#include <ctl/one_skeleton/graph_to_metis.hpp>
#include <ctl/io/io.hpp>

//BOOST
#include <boost/program_options.hpp>
#include <boost/graph/graphviz.hpp>

namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary Simplex_boundary;
typedef ctl::Cell_complex< Simplex_boundary> Complex;
typedef std::vector<int> index_to_vertex_t;
int main(int argc, char *argv[]) {
  //setup options
  po::options_description desc("Usage: one_skeleton [options] input_file");
  desc.add_options()
  ( "help", "Display this message")
  ( "metis", "extract to a metis format")
  ( "dot", "extract to a dot format")
  ( "metis-index-off", "do not construct a metis vertex index file")
  ( "input-file", "input .asc file to parse");
  po::positional_options_description p;
  p.add("input-file",-1);
  po::variables_map vm;
  po::store(
	  po::command_line_parser(argc,argv).options(desc).positional(p).run(),
	  vm
	 );
  po::notify(vm);
  if (vm.count("help")){
	std::cout << desc << std::endl;
	std::exit( 0);
  }
  if (vm.count("input-file") != 1){
	std::cout << desc << std::endl;	
	std::exit( -1);
  }
  if( (vm.count("metis") || vm.count("metis-index-off")) && vm.count("dot")){
	std::cerr << "improper combination of flags" << std::endl;
  }

  // Set up complex
  Complex complex;
  ctl::Nbhd_graph<> graph;

  // Read the complex in 
  std::ifstream in;   
  ctl::open_file( in, vm["input-file"].as< std::string >().c_str());   
  //in >> complex;  
  ctl::close_file( in);

  std::string extension;
  std::vector< int> neighbors; //num_vertices+1
  std::vector< int> offsets; //2*num_edges
  index_to_vertex_t index_to_vertex_map;

  //extract graph
  if (vm.count("metis")){
	 ctl::one_skeleton( complex, neighbors, offsets, index_to_vertex_map);
	 extension.append(".met");
  }else{
	 ctl::complex_to_graph(complex, graph);
	if (vm.count("dot")){
		extension.append(".dot");
	}else{
		extension.append(".ngh");
	}
  }

  //make filename
  std::string graph_name ( vm["input-file"].as< std::string >());
  std::string index_file ( vm["input-file"].as< std::string >());
  size_t found = graph_name.rfind( '.');
  if (found == std::string::npos){
        graph_name.append(extension);
	index_file.append(".idxmet");
  }else{
        graph_name.replace( found, graph_name.size()-1, extension);
        index_file.replace( found, index_file.size()-1, ".idxmet");
  }

  //write to a file
  std::ofstream out;
  ctl::open_file( out,graph_name.c_str());
  if(vm.count("metis")){
	ctl::write_metis( out, offsets, neighbors);
  }else if(vm.count("dot")){
	boost::write_graphviz( out, graph);
  }else{
	  out << graph;
  }
  ctl::close_file( out);

  //write out a metis index file, if you want to get vertex names back later
  if (vm.count("metis") && !vm.count("metis-index-off")){
	  std::ofstream index_out;
  	  ctl::open_file( index_out, index_file.c_str());
	  int count = 1;
	  for (index_to_vertex_t::iterator it  = index_to_vertex_map.begin();
				           it != index_to_vertex_map.end();
					   ++it, ++count){
		index_out << count << " " << *it << std::endl;
	  }
	  ctl::close_file( index_out);
  }
  return 0;
}
