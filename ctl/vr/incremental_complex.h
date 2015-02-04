#ifndef CTL_INCREMENTAL_H
#define CTL_INCREMENTAL_H
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
* \cite{hatcher, z-ct-10}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
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
*******************************************************************************
*******************************************************************************
* NOTES
* This is the construction of the Vietoris Rips Complex using the all pairs
* approach in the geometric phase and the incremental algorithm in the
* topological phase.
*******************************************************************************/

//STL
#include <vector> 
#include <algorithm>

//BOOST
#include <boost/graph/adjacency_list.hpp>

namespace ctl {

// Given a node, we loop through all its neighbors and return a vector
// of all the neighbors that have a lower ordering.
template<typename Graph, typename Vertex, typename Points> 
void get_lower_neighbors (const Graph& g,  
                 	  const Vertex& v,  
                 	  Points& lower_neighbors) {
    typedef typename boost::graph_traits<Graph>::adjacency_iterator Iterator;
    Iterator ai, ai_end;
    for( boost::tie(ai, ai_end) = boost::adjacent_vertices(v, g); 
	 ai != ai_end; ++ai) {
        if (*ai < v) { lower_neighbors.insert( lower_neighbors.end(), *ai); }
    } 
} 

// At every level in this recursive function, we introduce a new node to our
// simplicial complex and add the cofaces that arise from this new node.
template<typename Graph, typename Simplex, typename Neighbors, typename Complex>
void add_cofaces (const Graph& graph, Simplex& tau, 
		  const Neighbors& neighbors, 
		  Complex& complex, const std::size_t dimension) {
    typedef typename Neighbors::const_iterator Neighbor_iterator;
    complex.insert_open_cell(tau);
    if(tau.dimension() >= dimension) { return; }
    Neighbors lower_neighbors; 
    Neighbors final_neighbors; 
    for(Neighbor_iterator i = neighbors.begin(); i != neighbors.end(); ++i){
      lower_neighbors.clear();
      Simplex sigma( tau);
      sigma.insert( *i);
      get_lower_neighbors(graph, *i, lower_neighbors);
      final_neighbors.clear();
      set_intersection(lower_neighbors.begin(),lower_neighbors.end(),
                       neighbors.begin(),neighbors.end(),
                       back_inserter(final_neighbors));
      add_cofaces(graph, sigma, final_neighbors, complex, dimension); 
    }
} 

// We loop through all nodes, selecting each one as the starting point for
// constructing our simplicial complex.
template<typename Graph, typename Complex> 
void incremental_vr (const Graph& g, Complex& complex, std::size_t dimension) { 
    typedef typename Complex::Cell Simplex;
    typedef typename Simplex::value_type Vertex;
    typedef typename boost::graph_traits< Graph>::vertex_iterator vertex_iterator;
    vertex_iterator vi, vlast;
    for ( std::tie( vi, vlast) = boost::vertices( g); vi != vlast; ++vi) {
        std::vector< Vertex> neighbors;
        get_lower_neighbors(g, *vi, neighbors);
	Simplex tau(1, *vi); 
        add_cofaces( g, tau, neighbors, complex, dimension); 
    } 
}

} //end namespace CTL
#endif // CTL_INCREMENTAL_H
