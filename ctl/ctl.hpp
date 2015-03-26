#ifndef CTLIB_CTL_H
#define CTLIB_CTL_H
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
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>

//Abstract Cube 
//TODO

//IO
#include <ctl/io/io.hpp>

//Product Cell
#include <ctl/product_cell/product_boundary.hpp>
#include <ctl/product_cell/product_cell.hpp>
#include <ctl/product_cell/product_cell_less.hpp>

//Coefficients
//Finite Fields
#include <ctl/finite_field/finite_field.hpp>

//Chain Complex
#include <ctl/chain_complex/chain_complex.hpp>
#include <ctl/chain_complex/complex_boundary.hpp>

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
#include <ctl/barcodes/barcodes.hpp>

//Filtration
#include <ctl/filtration/filtration_boundary.hpp>
#include <ctl/filtration/filtration.hpp>
#include <ctl/filtration/less.hpp>

//Graphs & metrics
#include <ctl/points/points.hpp>
#include <ctl/metrics/metric.hpp>
#include <ctl/nbhd_graph/all_pairs.hpp>
#include <ctl/nbhd_graph/epsilon_search.hpp>
#include <ctl/nbhd_graph/nbhd_graph.hpp>

//VR
#include <ctl/vr/incremental_complex.hpp>

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
