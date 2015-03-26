#ifndef CTL_EPSILON_SEARCH_H
#define CTL_EPSILON_SEARCH_H
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
* We use the all pairs approach to constuct a graph. 
*
*******************************************************************************/
// ANN Library
#include <ANN/ANN.h>

//STL 
#include <vector> 
#include <algorithm>

//BOOST
#include <boost/graph/adjacency_list.hpp>

//CTL
#include <ctl/metrics/metric.hpp>

namespace ctl{
namespace epsilon_search{
template<typename Points, typename Graph>
void construct_graph( const Points& points, 
		      const double epsilon, 
		      Graph& graph) {
    typedef typename boost::graph_traits<Graph> graph_traits;
    //typedef typename graph_traits::vertex_iterator vertex_iterator;
    typedef typename graph_traits::vertex_descriptor vertex_descriptor; 
    typedef typename boost::property_map< Graph,
					  boost::vertex_name_t>::type name_map_t;
    //typedef typename boost::property_traits< name_map_t>::value_type  vertex_name_t;
    typedef typename boost::property_map< Graph, boost::edge_weight_t>::type
      weight_map_t;
    typedef typename boost::property_traits< weight_map_t>::value_type
      edge_weight_t;
 
    // ANN is space inefficient, requires copying of dataset.
    typedef std::vector< ANNpoint> Ann_points;
    double epsilon2 = epsilon*epsilon;
    double error=0.0;
    Ann_points ann_points;
    for( auto & i : points){ ann_points.push_back( (ANNpoint)&(i[0])); }

    //Search structure
    int dim = (int)( points[ 0]).size();
    ANNpointSet* tree = new ANNkd_tree( &*ann_points.begin(), 
				   ann_points.size(), dim, 30);

    //add vertices
    name_map_t name_map = boost::get( boost::vertex_name, graph);
    for( std::size_t i = 0; i < points.size(); ++i){ 
      vertex_descriptor v_descriptor = boost::add_vertex( graph);
      name_map[ v_descriptor] = i;
    }
 
    //add edges with weights
    ANNpointArray points_array = &*ann_points.begin();
    for( std::size_t i = 0; i < points.size(); ++i) {
      // first call is to find # of neighbors within radius
      int k = tree->annkFRSearch( points_array[ i],
                                  epsilon2, 0, NULL, NULL, error);
      std::vector< ANNidx> indices( k);
      std::vector< ANNdist> distances (k);
      //second call to get the elements themselves..
      tree->annkFRSearch( points_array[ i],
                          epsilon2, k,
                          &*indices.begin(),
                          &*distances.begin(),
                          error);

     //iterate over points in ball adding weighted edges..
     for( int j = 0; j < k; ++j) {
      //no self loops or duplicate edges..
      if( indices[ j] <= i) { continue; }
      vertex_descriptor source = i; 
      vertex_descriptor target = indices[ j];
      edge_weight_t weight = std::sqrt(distances[ j]);
      boost::add_edge( source, target, weight, graph);
     }
   }
  delete tree;
  annClose();
} 
} //end namespace epsilon_search
} //end namespace CTL


#endif //CTL_EPSILON_SEARCH_H
