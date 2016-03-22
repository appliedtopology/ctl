/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net"
*******************************************************************************
* BSD-3
*******************************************************************************/
#define CATCH_CONFIG_RUNNER
#include <catch/catch.hpp>

#include "test_abstract_simplex.hpp"
#include "test_blowup_tool.hpp"
#include "test_chain.hpp"
#include "test_cover_generation.hpp"
#include "test_cubical_chain_complex.hpp"
#include "test_finite_field.hpp"
#include "test_one_skeleton.hpp"
#include "test_product_cell.hpp"
#include "test_cube.hpp"

int main( int argc, char* argv[] ){
  int result = Catch::Session().run( argc, argv );
  return result;
}
