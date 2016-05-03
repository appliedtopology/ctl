#ifndef CTLIB_CTL_H
#define CTLIB_CTL_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>

//Abstract Cube 
//TODO

//IO
#include <ctl/io/io.hpp>

//Product Cell
#include <ctl/product_cell/iterator_product_boundary.hpp>
#include <ctl/product_cell/iterator_product_cell.hpp>
#include <ctl/product_cell/product_boundary.hpp>
#include <ctl/product_cell/product_cell.hpp>
#include <ctl/product_cell/product_cell_less.hpp>

//Coefficients
//Finite Fields
#include <ctl/finite_field/finite_field.hpp>

//Chain Complex
#include <ctl/cell_complex/cell_complex.hpp>
#include <ctl/cell_complex/complex_boundary.hpp>

//Weight Data
#include <ctl/weight_data/weight_data.hpp>
#include <ctl/weight_data/weight_functor.hpp>


//Term 
#include <ctl/term/term.hpp>
#include <ctl/term/term_less.hpp>
#include <ctl/term/term_tags.hpp>

//Chain
#include <ctl/chain/chain.hpp>
#include <ctl/chain/chain_add.hpp>

//Barcodes
//#include <ctl/barcodes/barcodes.hpp>

//Filtration
#include <ctl/graded_chain_complex/graded_boundary.hpp>
#include <ctl/graded_chain_complex/graded_cell_complex.hpp>
#include <ctl/graded_chain_complex/less.hpp>

//Graphs & metrics
#include <ctl/nbhd_graph/all_pairs.hpp>
#include <ctl/nbhd_graph/epsilon_search.hpp>
#include <ctl/nbhd_graph/nbhd_graph.hpp>

//VR
#include <ctl/vr/incremental_complex.hpp>

#include <ctl/cover/cover.hpp>

//Facilities for extracting the one_skeleton
#include <ctl/one_skeleton/complex_to_graph.hpp>
#include <ctl/one_skeleton/graph_to_metis.hpp>
#include <ctl/one_skeleton/one_skeleton.hpp>

//Parallel library (multithreaded)
#include <ctl/parallel/build_blowup_complex/build_blowup_complex.hpp>
#include <ctl/parallel/chain_complex/chain_complex.hpp>
#include <ctl/parallel/filtration/filtration.hpp>
#include <ctl/parallel/homology/persistence.hpp>
#include <ctl/parallel/partition_covers/cover_data.hpp>
#include <ctl/parallel/partition_covers/covers.hpp>
#include <ctl/parallel/partition_covers/cover_stats.hpp>
#include <ctl/parallel/partition_covers/graph_partition.hpp>
#include <ctl/parallel/utility/timer.hpp>
#include <ctl/parallel/homology/homology.hpp>

#include <ctl/matrix/iterator_property_map.hpp>
#include <ctl/matrix/offset_maps.hpp>
#include <ctl/matrix/matrix.hpp>

//Persistence 
#include <ctl/persistence/compute_barcodes.hpp>
#include <ctl/persistence/compute_betti.hpp>
#include <ctl/persistence/persistence_data.hpp>
#include <ctl/persistence/persistence.hpp>

//Not implemented
//#include <ctl/zigzag/persistence.h>

#endif //CTLIB_CTL_H
