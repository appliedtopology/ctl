#ifndef CTL_MATRIX_H

#include <vector>
#include <ctl/chain.h>

namespace ctl {

/**
* @brief Defines a Sparse matrix using the Dictionary of Keys 
* Storage type
* 
* It stoes a ctl::Term< Coefficient, index>
* where index references the row or column index.
* 
* @tparam C
* @tparam T
*/
template< typename C>
using Sparse_matrix = std::vector< ctl::Term< C, std::size_t> >;

}

#endif //CTL_MATRIX_H
