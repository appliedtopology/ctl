#ifndef CTL_COMPUTE_BARCODES_H
#define CTL_COMPUTE_BARCODES_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
 (Encyclopedia Britannica, 2008.)
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
       Cell_less less;
       barcodes.resize( filtration.complex().dimension()+1);
       auto offset_map = cascade_boundary_map.index();
       for(Filtration_iterator sigma = filtration.begin();
			       sigma != filtration.end(); ++sigma){
               const Chain& bd = cascade_boundary_map[ sigma];
	       if( bd.empty()){
	        Barcode & barcode = barcodes[ (*sigma)->first.dimension()];
	      	barcode.emplace( (*sigma)->second.weight(), 
				 std::numeric_limits< Weight>::infinity() );
	       }else if( cascade_boundary_map.index()[sigma] < bd.youngest().cell()){
	          Barcode & barcode = barcodes[ (*sigma)->first.dimension()];
	 	  const Filtration_iterator destroyer = filtration.begin()+offset_map[bd.youngest().cell()];
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
       typedef typename Barcode::value_type Bar;
       typedef typename Bar::first_type T;
       barcodes.resize( filtration.complex().dimension()+1);
       std::size_t pos=0;
       auto offset_map = cascade_boundary_map.index();
       for(Filtration_iterator sigma = filtration.begin(); 
			       sigma != filtration.end(); ++sigma, ++pos){
               const Chain& bd = cascade_boundary_map[ sigma];
	       if( bd.empty()){
	       Barcode & barcode = barcodes[ (*sigma)->first.dimension()];
	       barcode.emplace( pos, std::numeric_limits< T>::infinity());
	       }else if( offset_map[sigma] < bd.youngest().cell()){
	          Barcode & barcode = barcodes[ (*sigma)->first.dimension()];
	 	  auto destroyer = offset_map[bd.youngest().cell()];
		  barcode.emplace( pos, destroyer-pos);
	       }
       }
       if( !include_last_dim){ barcodes.pop_back(); }
}

}//namespace ctl
#endif //CTL_WRITE_BARCODES
