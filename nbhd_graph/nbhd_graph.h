#ifndef CTLIB_NBHD_GRAPH_H
#define CTLIB_NBHD_GRAPH_H
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
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
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
#include <boost/graph/adjacency_list.hpp>
#include <unordered_map>
//NOTE: syntax in this file is C++1x style.

//non-exported functionality
namespace {
template< typename vertex_name_t>
using vertex_name = boost::property< boost::vertex_name_t, vertex_name_t>;
template< typename weight_t>
using edge_weight = boost::property< boost::edge_weight_t,  weight_t>;
} //anonymous namespace


//exported functionality
namespace ctl{
template < typename vertex_name_t = std::size_t, 
	   typename edge_weight_t = double>
using Nbhd_graph = boost::adjacency_list< boost::vecS, 
					  boost::vecS,
					  boost::undirectedS,
					  vertex_name< vertex_name_t>,
					  edge_weight< edge_weight_t>
					>;

template< typename Stream, typename Graph>
Stream& print_vertices( Stream & out, const Graph & graph){
    typedef typename boost::graph_traits<Graph> graph_traits;
    typedef typename graph_traits::vertex_iterator vertex_iterator;
    typedef typename boost::property_map<Graph, boost::vertex_name>::const_type 
		     const_name_map_t; 

    const_name_map_t name_map = get( boost::vertex_name, graph);
    vertex_iterator vi, vlast;
    for( std::tie( vi, vlast) = boost::vertices( graph); vi != vlast; ++vi){
	out << get( name_map, *vi) << std::endl;	  
    }
}
template< typename Stream, typename Graph>
Stream& print_edges( Stream & out, const Graph & graph){
    typedef typename boost::graph_traits< Graph> graph_traits;
    typedef typename graph_traits::edge_iterator edge_iterator;
    typedef typename boost::property_map< Graph, 
					   boost::vertex_name>::const_type 
		     const_name_map_t; 
    typedef typename boost::property_map< Graph, 
					  boost::edge_weight_t>::const_type 
		     			   const_weight_map_t; 

    const_name_map_t name_map = get( boost::vertex_name, graph);
    const_weight_map_t weight_map = get( boost::edge_weight, graph);
    
    edge_iterator ei, last; 
    for( std::tie( ei, elast) = boost::edges( graph); ei != elast; ++ei){
	out << boost::get( name_map, source( *ei, boost::graph)) << " "
	    << boost::get( name_map, target( *ei, boost::graph)) << " "
	    << get( weight_map, *ei) << std::endl; 
    }
}

template< typename Stream, typename Graph>
Stream& print_graph( Stream& out, const Graph & graph, 
		     const std::size_t num_vertices, 
		     const std::size_t num_edges){
	out << num_vertices << " " << num_edges;
	print_vertices( out, graph);
	print_edges( out, graph);
	return out;
}

template< typename Stream, typename Graph>
Stream& read_graph( Stream& in, Graph & graph){
    typedef typename boost::property_map< Graph, boost::vertex_name_t>::type 
    name_map_t;
    typedef boost::property_map< Graph, boost::edge_weight_t>::type 
    weight_map_t;
    
    typedef boost::property_traits< name_map_t>::value_type vertex_name_t;
    typedef boost::property_traits< weight_map_t>::value_type edge_weight_t;
    
    //Relates vertex names
    typedef std::unordered_map< vertex_name_t, vertex_descriptor> 
    	Name_to_descriptor_map;
    
    weight_map_t weight_map = boost::get( boost::edge_weight, graph);
        name_map_t name_map = boost::get( boost::vertex_name, graph);
     Name_to_descriptor_map   descriptor( num_vertices); //preallocate buckets!
    
    //starts at 1 since we assume header already been read;
    std::size_t line_num=1; 
    std::string line;
    std::istringstream ss;
    //First, read vertices
    for (std::size_t i = 0; i < num_vertices; ++i){
	if( !ctl::get_line( in, line, line_num)){
		std::cerr << "Error: Not Enough Vertices!" << std::endl;
		return in;
	}
	ss.clear();
	ss.str( line);
	vertex_name_t name;
	ss >> name;	
	if( descriptor.find( name) != descriptor.end()){
		std::cerr << "Duplicate vertex!" << name << std::endl;
		return in;
	}
	vertex_descriptor descriptor = boost::add_vertex( graph);
	name_map[ descriptor] = name;
	descriptor.emplace( name, descriptor);
   }

   //Second, read edges
   for (std::size_t i = 0; i < num_edges; ++i){
	if( !ctl::get_line( in, line, line_num)){
		std::cerr << "Error: Not Enough Vertices!" << std::endl;
		return in;
	}
	ss.clear();
	ss.str( line);
	vertex_name_t source_name, target_name;
	line >> source_name >> target_name;
	if (ss.fail()){
		std::cerr << "Incorrect line: " << std::endl 
			  << line << std::endl;
		return in;
	}
	if (source_name == target_name){ 
		std::cerr << "no loops are allowed." << std::endl
			  << source_name << std::endl;
		return in;
	}
	if ( target_name < source_name){ std::swap( source_name, target_name); }
	edge_weight_t weight;
 	line >> weight;
	if (ss.fail()){ weight = 0.0; }
	vertex_descriptor source = find_descriptor( to_descriptor, source_name);
 	vertex_descriptor target = find_descriptor( to_descriptor, target_name);
	if( boost::edge( source, target, graph).second){
		std::cerr << "error duplicate edges not allowed!" << std::endl;
		return in;
	}
	add_edge( source, target, weight, graph);
   }
   return in;
}

std::istream& operator>>( std::istream& in, Nbhd_graph& graph){
	std::string header;
	std::size_t line_num=0;
	if( !ctl::get_line( in, header, line_num)){
		std::cerr << "Error invalid header" << std::endl;
		return in;
	}
	std::istringstream ss( header);
	std::size_t num_vertices, num_edges;
	ss >> num_vertices >> num_edges;
	return read_graph( in, graph, num_vertices, num_edges);
}

std::ostream& operator<<( std::ostream& out, const Nbhd_graph& graph){
	return print_graph( out, graph, 
			    boost::num_vertices( graph), 
			    boost::num_edges( graph));	
}

} //namespace ctl


#endif //CTLIB_NBHD_GRAPH_H
