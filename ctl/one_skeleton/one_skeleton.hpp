#ifndef CTL_ONE_SKELETON_H
#define CTL_ONE_SKELETON_H
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
// STL
#include <cstdlib>
#include <iostream>
#include <unordered_map>

// Project
#include <ctl/utility/timer.hpp>

namespace ctl{

template< typename Complex, typename Vector, typename Index_to_vector_map>
static void one_skeleton( const Complex& complex, 
			  Vector &neighbors,
			  Vector &offsets,
			  Index_to_vector_map &index_to_vertex_map) {
  typedef typename Complex::Cell Simplex;
  typedef typename Simplex::vertex_type vertex_type;
  typedef std::unordered_map< vertex_type, std::size_t> vertex_to_index_map_t;
  
  //Create Maps
  std::size_t last_index = 0;
  vertex_to_index_map_t vertex_to_index_map;
  // Assign indices & create maps
  for( const auto &  cell : complex){
    const auto& simplex = cell.first; 
    if( simplex.dimension() == 0) {
      const vertex_type& vertex = *simplex.begin();
      index_to_vertex_map.emplace_back( vertex);
      vertex_to_index_map.emplace( vertex, last_index++);
    }
  }
  
  //Construct adjacency_list
  typedef std::vector< Vector> Adjacency_list;
  Adjacency_list adjacency_list( last_index);
						
  //Extract edges
  std::size_t num_edges = 0;
  for( const auto & cell : complex){
    const auto& simplex = cell.first; 
    if( simplex.dimension() == 1) {
      ++num_edges;
      const std::size_t edge[ 2] = { vertex_to_index_map[ *simplex.begin()], 
				     vertex_to_index_map[ *simplex.rbegin()] };
      adjacency_list[ edge[ 0]].emplace_back( edge[ 1]);
      adjacency_list[ edge[ 1]].emplace_back( edge[ 0]);
    }
  }

  //Write into METIS CSR format
  neighbors.resize( 2*num_edges);
  offsets.reserve( last_index);
  std::size_t offset = 0;
  typename Vector::iterator start = neighbors.begin();

  for( const auto & i : adjacency_list){
    offsets.push_back( offset);
    std::copy( i.begin(), i.end(), start);
    start += i.size();
    offset += i.size();
  }
  offsets.emplace_back( neighbors.size());
}

} //namespace ctl
#endif //end namespace CTL_ONE_SKEL_H
