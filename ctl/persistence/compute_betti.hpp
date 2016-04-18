#ifndef CTL_WRITE_BETTI_H
#define CTL_WRITE_BETTI_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
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
       auto fm = cascade_boundary_map.index();
       for(Cell_iterator cell = complex.begin(); cell != complex.end(); ++cell){
               const Chain& bd = cascade_boundary_map[ cell];
               const bool c = bd.empty() || 
				(cell->first < bd.youngest().cell()->first); 
		betti[ cell->first.dimension()-(!c)] += (2*c -1);
       }
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
       auto fm = cascade_boundary_map.index();
       for( auto cell  = filtration.begin(); 
			 cell != filtration.end(); ++cell){
               const Chain& bd = cascade_boundary_map[ cell];
               const bool c = bd.empty() || (cell < filtration.begin()+fm[bd.youngest().cell()]); 
	       betti[ (*cell)->first.dimension()-(!c)] += (2*c -1);
       }
	_betti.swap( betti);
}



}//namespace ctl
#endif //CTL_WRITE_BETTI
