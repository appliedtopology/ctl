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
//NOTE: syntax in this file is C++1x style.
//STL
#include <unordered_map>
#include <sstream>

//BOOST
#include <boost/graph/adjacency_list.hpp>

//CTL
#include <ctl/io/io.hpp>


//non-exported functionality
namespace ctl{
namespace detail{
template< typename vertex_name_t>
using vertex_name = boost::property< boost::vertex_name_t, vertex_name_t>;
template< typename weight_t>
using edge_weight = boost::property< boost::edge_weight_t,  weight_t>;
} // detail anonymous namespace
} //ctl namespace

//exported functionality
namespace ctl{
template < typename vertex_name_t = std::size_t, 
	   typename edge_weight_t = double>
using Nbhd_graph = boost::adjacency_list< boost::vecS, 
					  boost::vecS,
					  boost::undirectedS,
					  detail::vertex_name< vertex_name_t>,
					  detail::edge_weight< edge_weight_t>
					>;

template< typename Stream, typename Graph>
Stream& print_vertices( Stream & out, const Graph & graph){
    typedef typename boost::graph_traits<Graph> graph_traits;
    typedef typename graph_traits::vertex_iterator vertex_iterator;
    typedef typename boost::property_map< Graph, 
					  boost::vertex_name_t>::const_type 
		     const_name_map_t; 

    const_name_map_t name_map = get( boost::vertex_name, graph);
    vertex_iterator vi, vlast;
    for( std::tie( vi, vlast) = boost::vertices( graph); vi != vlast; ++vi){
	out << get( name_map, *vi) << std::endl;	  
    }
    return out;
}
template< typename Stream, typename Graph>
Stream& print_edges( Stream & out, const Graph & graph){
    typedef typename boost::graph_traits< Graph> graph_traits;
    typedef typename graph_traits::edge_iterator edge_iterator;
    typedef typename boost::property_map< Graph, 
					   boost::vertex_name_t>::const_type 
		     const_name_map_t; 
    typedef typename boost::property_map< Graph, 
					  boost::edge_weight_t>::const_type 
		     			   const_weight_map_t; 

    const_name_map_t name_map = get( boost::vertex_name, graph);
    const_weight_map_t weight_map = get( boost::edge_weight, graph);
    
    edge_iterator ei, elast; 
    for( std::tie( ei, elast) = boost::edges( graph); ei != elast; ++ei){
	out << boost::get( name_map, source( *ei, graph)) << " "
	    << boost::get( name_map, target( *ei, graph)) << " "
	    << get( weight_map, *ei) << std::endl; 
    }
    return out;
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
Stream& read_graph( Stream& in, Graph & graph, 
		    std::size_t num_vertices, std::size_t num_edges){
    typedef typename boost::property_map< Graph, boost::vertex_name_t>::type 
    name_map_t;
    
    typedef typename boost::property_traits< name_map_t>::value_type 
							vertex_name_t;
    typedef typename boost::graph_traits< Graph>::vertex_descriptor 
						  vertex_descriptor; 
   //Relates vertex names
    typedef std::unordered_map< vertex_name_t, vertex_descriptor> 
    	Name_to_descriptor_map;
    
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
	vertex_descriptor v_descriptor = boost::add_vertex( graph);
	name_map[ v_descriptor] = name;
	descriptor.emplace( name, v_descriptor);
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
	ss >> source_name >> target_name;
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
	double weight_;
 	ss >> weight_;
	if (ss.fail()){ weight_ = 0.0; }
	boost::edge_weight_t weight = boost::edge_weight_t( weight_); 
	vertex_descriptor source = descriptor[ source_name];
 	vertex_descriptor target = descriptor[ target_name];
	if( boost::edge( source, target, graph).second){
		std::cerr << "error duplicate edges not allowed!" << std::endl;
		return in;
	}
	add_edge( source, target, weight, graph);
   }
   return in;
}
} //namespace ctl
template < typename Stream, typename v, typename e> 
Stream& operator>>( Stream& in, ctl::Nbhd_graph< v,e> & graph){
	std::string header;
	std::size_t line_num=0;
	if( !ctl::get_line( in, header, line_num)){
		std::cerr << "Error invalid header" << std::endl;
		return in;
	}
	std::istringstream ss( header);
	std::size_t num_vertices, num_edges;
	ss >> num_vertices >> num_edges;
	return ctl::read_graph( in, graph, num_vertices, num_edges);
}
template < typename Stream, typename v, typename e> 
Stream & operator<<( Stream& out, const ctl::Nbhd_graph< v, e> & graph){
	return ctl::print_graph( out, graph, 
			    boost::num_vertices( graph), 
			    boost::num_edges( graph));	
}



#endif //CTLIB_NBHD_GRAPH_H
