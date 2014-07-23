#ifndef CTL_COMPUTE_BARCODES_H
#define CTL_COMPUTE_BARCODES_H
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
//tag dispatching
namespace detail{
struct weighted_tag {};
struct non_weighted_tag {};
} //namespace detail

template< typename Barcodes, 
	  typename Filtration, 
	  typename Cell_chain_map>
void compute_barcodes( Filtration & filtration, 
                       Cell_chain_map & cascade_boundary_map,
		       Barcodes & barcodes,
		       ctl::detail::weighted_tag t,
		       bool include_last_dim=false){ 
       typedef typename Filtration::iterator Filtration_iterator; 
       typedef typename Filtration::Complex Complex;
       typedef typename Complex::Data Data;
       typedef typename Data::Weight Weight;
       typedef typename Cell_chain_map::value_type Chain;
       typedef typename Barcodes::value_type Barcode;
       barcodes.resize( filtration.complex().dimension()+1);
       for(Filtration_iterator sigma = filtration.begin(); 
			       sigma != filtration.end(); ++sigma){
               const Chain& bd = cascade_boundary_map[ sigma];
	       if( bd.empty()){
		std::cout << "Infinity is: " << std::numeric_limits< Weight>::infinity() << std::endl;
	        Barcode & barcode = barcodes[ (*sigma)->first.dimension()];
	      	barcode.emplace( (*sigma)->second.weight(), 
				 std::numeric_limits< Weight>::infinity() );	 
	       }else if( sigma < bd.youngest().cell()){
	          Barcode & barcode = barcodes[ (*sigma)->first.dimension()];
	 	  const Filtration_iterator & destroyer = bd.youngest().cell();
		  barcode.emplace( (*sigma)->second.weight(),
				   (*destroyer)->second.weight());
	       }
       }
       if( !include_last_dim){ barcodes.pop_back(); }
}

template< typename Barcodes, 
	  typename Filtration, 
	  typename Cell_chain_map>
void compute_barcodes( Filtration & filtration, 
                       Cell_chain_map & cascade_boundary_map,
		       Barcodes & barcodes,
		       ctl::detail::non_weighted_tag t,
		       bool include_last_dim=false){
       typedef typename Filtration::iterator Filtration_iterator; 
       //typedef typename Filtration::Complex Complex;
       typedef typename Cell_chain_map::value_type Chain;
       typedef typename Barcodes::value_type Barcode;
       barcodes.resize( filtration.complex().dimension()+1);
       std::size_t pos=0;
       for(Filtration_iterator sigma = filtration.begin(); 
			       sigma != filtration.end(); ++sigma, ++pos){
               const Chain& bd = cascade_boundary_map[ sigma];
	       if( bd.empty()){
	        Barcode & barcode = barcodes[ (*sigma)->first.dimension()];
	      	barcode.emplace( pos, std::numeric_limits< std::size_t>::infinity());	 
	       }else if( sigma < bd.youngest().cell()){
	          Barcode & barcode = barcodes[ (*sigma)->first.dimension()];
	 	  const Filtration_iterator & destroyer = bd.youngest().cell();
		  barcode.emplace( pos, destroyer-filtration.begin());
	       }
       }
       if( !include_last_dim){ barcodes.pop_back(); }
}

}//namespace ctl
#endif //CTL_WRITE_BARCODES
