#ifndef CTL_MATRIX_H
#define CTL_MATRIX_H

#include <vector>
#include <ctl/chain/chain.hpp>
#include <ctl/matrix/iterator_property_map.hpp>
#include <ctl/matrix/offset_maps.hpp>
namespace ctl {

/**
* @brief Defines a Sparse matrix using the Dictionary of Keys 
* Storage type
* 
* It stoes a ctl::Chain< ctl::Term< Coefficient, Index> >
* where index references the row or column index.
* 
* @tparam C
* @tparam T
*/
template< typename Coeff, typename Cell=std::size_t>
using Sparse_matrix = std::vector< ctl::Chain< ctl::Term< Cell, Coeff> > >;

template< typename Iterator>
using Offset_map = ctl::Pos_offset_map< Iterator>;

template< typename Coeff, typename Offset_map, typename Cell=std::size_t>
using Sparse_matrix_map = ctl::iterator_property_map< typename Sparse_matrix< Coeff, Cell>::iterator,
				    		    Offset_map,
				    		    typename Sparse_matrix< Coeff,Cell>::value_type,
				    		    typename Sparse_matrix< Coeff,Cell>::value_type&>;
}

#endif //CTL_MATRIX_H
