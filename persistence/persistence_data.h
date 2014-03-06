#ifndef CTLIB_PERSISTENCE_DATA_H
#define CTLIB_PERSISTENCE_DATA_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the below liscence, but please
* do not commit acts of academic dishonesty. We encourage and request that 
* for any academic use of this source code one should cite the following 
* papers:
* 
* \cite{$bibtex_names_here}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) $NAMES_OF_AUTHORS $YEARS_FIRST_CREATED <$emails>
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
namespace _ctl {

template< typename Term_less_, 
	  typename Boundary_operator_, 
	  typename Cascade_map_,
	  typename Output_policy_ > 
//hold onto persistence data
struct Persistence_data {

	typedef typename Term_less_ Term_less; 
	typedef typename Boundary_operator_ Boundary_operator;
	typedef typename Cascade_map_ Cascade_map;
	typedef typename Output_policy_ Output_policy;

	Persistence_data( Term_less  t, 
			  Boundary_operator & bd_,
			  Cascade_map & bd_map_,	
			  Cascade_map & map_,
			  Output_policy p):
	term_less( t), bd( bd_), cascade_boundary_map( bd_map_), 
	cascade_map( map_), policy( p), 
	cascade( 1), cascade_boundary( 1) {};

	Term_less term_less;
	Boundary_operator bd;
	Cascade_map& cascade_boundary_map; 
	Cascade_map& cascade_map; 
	Output_policy policy;

	Chain cascade;
	Chain cascade_boundary;
}; //struct Persistence_data 

} //end namespace _ctl

#endif //CTLIB_PERSISTENCE_DATA_H
