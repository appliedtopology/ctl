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

//TIMER
#include <iomanip>
#include <ctl/utility/timer.h>

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
        typedef typename Complex::iterator Complex_iterator;
	typedef typename Cover_complex::Cell Nerve_cell;
	typedef typename Cover_complex::iterator Nerve_iterator;
	typedef typename Index_to_vertex_map::const_iterator Index_iterator;
	for( Index_iterator i = index_to_vertex.begin(); 
			    i < index_to_vertex.end(); ++i){
		const int j = std::distance( index_to_vertex.begin(), i);
		Cell cell = { (int)*i };
		Nerve_cell part_cell = { (int)part[ j] };
		Nerve_iterator iterator = nerve.find_cell( part_cell);
		vertex_to_nerve.insert( make_pair( *i,  iterator) );
		complex.find_cell( cell)->second.data() = iterator;
        }
}

} //end namespace parallel
}  //end namespace ctl
#endif // _CTL_COVERS_H
