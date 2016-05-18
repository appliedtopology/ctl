#ifndef CTL_COMPUTE_BARCODES_H
#define CTL_COMPUTE_BARCODES_H
#include <ctl/persistence/barcodes/barcodes.hpp>
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
namespace ctl{
template< typename Complex, 
	  typename Cell_chain_map>
ctl::Barcodes 
compute_barcodes( ctl::Graded_cell_complex< Complex>& F, 
                  Cell_chain_map & R_map,
		  bool include_last_dim=false){
       ctl::Barcodes barcodes;
       barcodes.resize( F.complex().dimension()+1);
       double pos=0;
       auto offset_map = R_map.index();
       for(auto sigma = F.begin(); sigma != F.end(); ++sigma, ++pos){
               const auto& bd = R_map[ sigma];
	       auto& barcode = barcodes[ (*sigma)->first.dimension()];
	       if( bd.empty()){
	       	  barcode.emplace_back( pos, F.size());
	       }else if( offset_map[sigma] < bd.youngest().cell()){
	 	  auto destroyer = offset_map[bd.youngest().cell()];
		  barcode.emplace_back( pos, destroyer);
	       }
       }
       if( !include_last_dim){ barcodes.pop_back(); }
       return barcodes;
}

}//namespace ctl
#endif //CTL_WRITE_BARCODES
