#ifndef _CTL_METIS_H_
#define _CTL_METIS_H_
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
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
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

//TIMER
#include <iomanip>
#include <ctl/utility/timer.hpp>

//STL
#include <algorithm> //max & min_element

//METIS
extern "C" {
#include <metis.h>
}

namespace ctl {
namespace parallel{
template< typename Vector>
void metis( Vector & xadj, 
	    Vector & adjncy, 
	    Vector & part, 
	    idx_t & edgecut, 
	    idx_t & num_vertices, 
	    idx_t num_parts){
	idx_t ncon = 1;
	//hopefully updated to v5.0 spec
        METIS_PartGraphRecursive( &num_vertices,
				  &ncon, //no idea what this is for, but the manual
					 //suggests making it = 2. 
				  &xadj[0], &adjncy[0], 
				  NULL, NULL, NULL,
				  &num_parts, 
				  NULL, NULL, NULL, 
				  &edgecut, &part[0]);
}

template< typename Complex,
	  typename Cover_complex, 
	  typename Index_to_vertex_map,
	  typename Vertex_to_part_map,
	  typename Vertex_to_nerve_map>
void metis_to_complex( Complex & complex,
		       Cover_complex & nerve, 
		       const Index_to_vertex_map & index_to_vertex, 
		       Vertex_to_nerve_map & vertex_to_nerve,
		       const Vertex_to_part_map  & part){

        typedef typename Complex::Cell Cell;
        //typedef typename Complex::iterator Complex_iterator;
	typedef typename Cover_complex::Cell Nerve_cell;
	typedef typename Cover_complex::iterator Nerve_iterator;
	typedef typename Index_to_vertex_map::const_iterator Index_iterator;
	for( Index_iterator i = index_to_vertex.begin(); 
			    i < index_to_vertex.end(); ++i){
		const int j = std::distance( index_to_vertex.begin(), i);
		Cell cell = { (std::size_t)*i };
		Nerve_cell part_cell = { (std::size_t)part[ j] };
		Nerve_iterator iterator = nerve.find_cell( part_cell);
		vertex_to_nerve.insert( make_pair( *i,  iterator) );
		complex.find_cell( cell)->second.data() = iterator;
        }
}

} //end namespace parallel
}  //end namespace ctl
#endif // _CTL_COVERS_H
