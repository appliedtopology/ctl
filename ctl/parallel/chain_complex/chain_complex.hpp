#ifndef CTLIB_CONCURRENT_CELL_MAP_H
#define CTLIB_CONCURRENT_CELL_MAP_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
/*********************
* April 5th, 2014
* Notes: presently we wrap the std::unordered_map a.k.a Map type.
* We fail to have perfect hashing, and we have collisions between
* cells of various different dimensions.
* A relatively easy optimization to explore is to  have something like
* vector< Map> where array element i stores cells of dimension i.
* Aside from certainly having less collisions,
* this has the benefit that the skeletal filtration will now be available with
* zero extra work after complex construction.
* We don't even need to worry about resizing time, since, copying a map is done
* via a swap of ~4 pointers, and in c++11 they are moves.
* Also this only happens at most O(log(d)) or something..
* the only thing to be careful of is the begin() and end() needs to be the
* original begin() and end() iterators concatenated together.
* This is somewhat frustrating because the naive implementation will require a
* 5x space bloat for each iterator, so less iterators fit on a cache line.
* A massive sort might have 5x more cache misses, for example, and
* the chains we store for persistence will have more stuff in them.
* We would save space by forcing a single hash table to hash cells of different
* dimensions to different places.
* This also makes iterators
* invalid less often then they were before.
* In particular, iterators are only invalidated in one dimension at a time,
* AND rehashing will be marginally less expensive.
* begin()/end() would be overloaded to take a dimension as well.
**********************/
//STL
#include <unordered_map>
#include <vector>
#include <sstream>

//CTL
#include <ctl/hash/hash.hpp>
#include <ctl/io/io.hpp>
#include <ctl/abstract_simplex/abstract_simplex.hpp>

#include <tbb/concurrent_unordered_map.h>

//exported functionality
namespace ctl{
namespace parallel {
template< typename Boundary_,
	  typename Data_ = ctl::detail::Default_data,
	  typename Hash_ = ctl::Hash< typename Boundary_::Cell_> >
using Cell_complex = ctl::Cell_complex< Boundary_, 
Data_, Hash_, true>;
} //namespace parallel
} //namespace ctl

#endif //CTL_CONCURRENT_CHAIN_COMPLEX_MAP_H
