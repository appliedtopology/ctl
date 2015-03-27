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
*******************************************************************************/

//GTest
#include "gtest/gtest.h"

#include "test_abstract_simplex.hpp"
#include "test_chain.hpp"
#include "test_cube.hpp"
#include "test_cubical_chain_complex.hpp"
//#include <test_simplicial_chain_complex.hpp>
//#include <test_vr.hpp>
#include "test_filtration.hpp"
#include "test_finite_field.hpp"
#include "test_multi_array.hpp"
#include "test_one_skeleton.hpp"
//#include "test_product_cell.hpp"
//#include <test_cover_tool.hpp>
//#include <test_blowup_tool.hpp>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
