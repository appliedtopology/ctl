#ifndef CTL_WRITE_BETTI_H
#define CTL_WRITE_BETTI_H
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
* \cite{zc-cph-04, z-ct-10}
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


namespace ctl{
template< typename Betti, typename Out>
void write_betti( const Betti & b, Out & out){ 
	for( auto n : b) { out << n << " "; }
	out << std::endl;
} 
template< typename Betti, 
	  typename Complex, 
	  typename Cell_chain_map>
void compute_betti( Complex & complex, 
                    Cell_chain_map & cascade_boundary_map,
		    Betti & _betti){ 
       typedef typename Complex::iterator Cell_iterator;
       typedef typename Cell_chain_map::value_type Chain;
       Betti betti( complex.dimension()+1,0);
       for(Cell_iterator cell = complex.begin(); cell != complex.end(); ++cell){
               const Chain& bd = cascade_boundary_map[ cell];
               const bool c = bd.empty() || 
				(cell->first < bd.youngest().cell()->first); 
		betti[ cell->first.dimension()-(!c)] += (2*c -1);
       }
       ctl::write_betti( betti,std::cout);
	_betti.swap( betti);
}

template< typename Betti, 
	  typename Filtration, 
	  typename Cell_chain_map>
void compute_betti( Filtration & filtration, 
                    Cell_chain_map & cascade_boundary_map,
		    Betti & _betti, bool){ 
       typedef typename Filtration::Complex Complex;
       typedef typename Cell_chain_map::value_type Chain;
       const Complex & complex = filtration.complex();
       Betti betti( complex.dimension()+1,0);
       for( auto cell  = filtration.begin(); 
			 cell != filtration.end(); ++cell){
               const Chain& bd = cascade_boundary_map[ cell];
               const bool c = bd.empty() || (cell < bd.youngest().cell()); 
	       betti[ (*cell)->first.dimension()-(!c)] += (2*c -1);
       }
       ctl::write_betti( betti,std::cout);
	_betti.swap( betti);
}



}//namespace ctl
#endif //CTL_WRITE_BETTI
