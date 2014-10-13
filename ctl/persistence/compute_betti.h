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
template< typename Betti, typename Out>
void write_betti( const Betti & b, Out & out){ 
	for( auto n : b) { out << n << " "; }
	out << std::endl;
} 
template< typename Betti, 
	  typename Complex, 
	  typename Cell_chain_map>
void compute_betti( const Complex & complex, 
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
