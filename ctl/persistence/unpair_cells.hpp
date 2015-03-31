#ifndef CTLIB_UNPAIR_CELLS_H
#define CTLIB_UNPAIR_CELLS_H
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
*******************************************************************************
* NOTES
*
*
*******************************************************************************/

#include <ctl/io/io.hpp>

//exported functionality
namespace ctl{
template< typename Iterator, typename Cell_to_chain_map, typename Filtration_map=ctl::identity>
void unpair_cells( Iterator begin, Iterator end, 
		   Cell_to_chain_map& boundary_matrix,
		   Filtration_map fm = Filtration_map()){ 
       for( auto i = begin; i != end; ++i){
             auto & c = boundary_matrix[ i];
             if ( (c.size() == 1) && (fm[ i] < c.youngest().cell())){
             	c.clear();
             } 
       }  
}
} //namespace ctl


#endif //CTLIB_EXAMPLE_H
