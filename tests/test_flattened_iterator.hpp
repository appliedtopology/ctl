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
//STL
#include <iostream>
#include <vector>
#include <unordered_map>
//CTL
#include <ctl/utility/flattening_iterator.hpp>

//GTest
#include "gtest/gtest.h"

TEST(FlatteningIterator, Constructs){
	typedef std::vector< std::unordered_map< int, bool> > Container;
	Container c;
	c.resize( 5);
	std::size_t count=0;
	for( auto i = 0; i < 5; ++i){
		for( auto j = 0; j <= i; ++j){ 
			c[ i].insert( std::make_pair( j, 1));
			++count;
		}
	}
 	auto begin = ctl::flatten( c.begin(), c.end());
	auto end = ctl::flatten( c.end());
	std::size_t distance= std::distance( begin, end);
	ASSERT_EQ( count, distance);
}
