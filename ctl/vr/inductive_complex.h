#ifndef CTL_INDUCTIVE_H
#define CTL_INDUCTIVE_H
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
* Copyright (C) Amr Mohamed 2014 <amr1@stanford.edu>
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
*******************************************************************************
* NOTES
* This is the construction of the Vietoris Rips Complex using the all pairs
* approach in the geometric phase and the inductive algorithm in the
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
        if (*ai < v) { lower_neighbors.push_back( *ai); }
    } 
} 

// We add the given graph to the complex. Also, we update the cells vectors to
// keep track of the dimensions of the cells.
template<typename Graph, typename Complex, typename Vector>
void add_graph_to_complex (const Graph& g,
			   Complex& complex,
			   Vector& k_cells,
			   Vector& k_plus_one_cells) {
    typedef typename Complex::Cell Simplex;
    typedef typename boost::graph_traits< Graph>::vertex_iterator vertex_iterator;
    typedef typename boost::graph_traits< Graph>::adjacency_iterator Iterator;
    vertex_iterator vi, vlast;
    for ( std::tie( vi, vlast) = boost::vertices( g); vi != vlast; ++vi) {
        // adding {*vi} to both k_cells and complex
        Simplex tau(1, *vi);
        k_cells.push_back(tau);
        complex.insert_open_cell(tau);
        Iterator ai, ai_end;
        for ( boost::tie(ai, ai_end) = boost::adjacent_vertices(*vi, g);
              ai != ai_end; ++ai) {
                // adding {*vi, *ai} to both k_plus_one_cells and complex
                Simplex sigma( tau);
                sigma.insert( *ai);
                k_plus_one_cells.push_back(sigma);
                complex.insert_open_cell(sigma);
        }
    }
}

// We loop through all nodes, selecting each one as the starting point for
// constructing our simplicial complex.
template<typename Graph, typename Complex> 
void inductive_vr (const Graph& g, Complex& complex, std::size_t dimension) { 
    typedef typename Complex::Cell Simplex;
    typedef typename Simplex::value_type Vertex;
    std::vector<Simplex> k_cells;
    std::vector<Simplex> k_plus_one_cells;
    // adding the graph to the complex
    add_graph_to_complex (g, complex, k_cells, k_plus_one_cells);
    // inductive algorithm
    for ( int k = 1; k < dimension; k++) {
        k_cells.swap(k_plus_one_cells);
	k_plus_one_cells.clear();
	for ( int i = 0; i < k_cells.size(); i++) {
	    Simplex tau(k_cells[i]);
	    // getting the intersection of all lower neighbors
	    std::vector< Vertex> final_neighbors;
    	    typedef typename Simplex::const_iterator simplex_iterator;
	    for(simplex_iterator vi = tau.begin(); vi != tau.end(); ++vi) {
		std::vector< Vertex> lower_neighbors;
		get_lower_neighbors(g, *vi, lower_neighbors);
		set_intersection(lower_neighbors.begin(),
				 lower_neighbors.end(),
				 final_neighbors.begin(),
				 final_neighbors.end(),
				 back_inserter(final_neighbors));
	    }
	    // constructing new simplices and adding them to the complex
	    for ( int j = 0; j < final_neighbors.size(); j++) {
		Simplex sigma(tau);
		sigma.insert(final_neighbors[j]);
		k_plus_one_cells.push_back(sigma);
		complex.insert_open_cell(sigma);
	    }
	}
    }
}

} //end namespace CTL
#endif // CTL_INDUCTIVE_H