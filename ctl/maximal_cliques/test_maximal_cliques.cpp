/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* Release under BSD-3 License See LICENSE
*******************************************************************************
* NOTES
* Based on original implementation by Gabe Weaver
*******************************************************************************/

// Standard & STL
#include <iostream> // ifstream, cerr

// BGL
#include <boost/graph/graph_traits.hpp>

// CTL
#include <ctl/nbhd_graph/nbhd_graph.h>
#include <ctl/maximal_cliques/clique_output_iterator.h>

#include <ctl/maximal_cliques/maximal_cliques.h>
#include <ctl/io/io.h>
#include <ctl/utility/timer.h>

int main(int argc, char *argv[]) {
  typedef boost::graph_traits< ctl::Nbhd_graph>     graph_traits;
  typedef graph_traits::vertex_descriptor           vertex_descriptor; 
  typedef std::vector< vertex_descriptor>           Vector;

  // Parse command line
  if( argc < 2) {
    std::cerr << argv[0] << " graph" << std::endl;
    std::exit(1);
  }

  // Establish input stream
  std::ifstream in;
  ctl::open_file( in, argv[1]);

  // Read the graph
  ctl::Nbhd_graph graph;
  in >> graph;

  // Setup the output iterator
  ctl::Clique_output_iterator< Vector> out;

  // Count number of maximal cliques
  ctl::Timer timer;
  timer.start();
  ctl::maximal_cliques(graph, out);
  std::cout << out.num() << " cliques in " << timer.get() << " seconds" <<
  std::endl;
  return 0;
}
