#ifndef CTLIB_BARCODE_H
#define CTLIB_BARCODE_H
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
*******************************************************************************
* NOTES
* We use the type alias in C++11. This is ok for the definitions, but, we might
* be muddying the waters by defining operator<< and operator>> on them.
* on the other hand it doesn't seem like this should really cause an issue here.
*******************************************************************************/
//STL
#include <iostream>
#include <set>
#include <vector>
#include <utility>
#include <functional>
#include <ctl/hash/hash.hpp>

//exported functionality
namespace ctl{
//each barcode is composed of intervals
template< typename Weight>
using Interval = std::pair< Weight, Weight>;

//a multiset of intervals in a fixed dimension is a barcode
template< typename Weight>
using Barcode = std::multiset< ctl::Interval< Weight> >;

//and the barcodes for a space are an array of them, one for each dimension
template< typename Weight>
struct Barcodes : std::vector< Barcode< Weight> > {
	private:
	typedef std::vector< Barcode< Weight> > Vector;
	public:
	Barcodes(): Vector() {}
	Barcodes( const Barcodes & b): Vector( b) {}
	using Vector::operator=;
}; //struct Barcodes

//I/O for an interval
template< typename Stream, typename Weight>
Stream& operator<<( Stream& out, const ctl::Interval< Weight>& interval) {
  out  << interval.first << " " << interval.second;
  return out;
}

template< typename Stream, typename Weight>
Stream& operator>>( Stream& in, ctl::Interval< Weight>& interval) {
  in >> interval.first >> interval.second;
  return in; 
}

//I/O for a barcode
template< typename Stream, typename Weight>
Stream& operator<<( Stream& out, const ctl::Barcode< Weight>& barcode){
  typedef typename Barcode< Weight>::const_iterator Iterator;
  for( Iterator i = barcode.begin(); i != barcode.end(); ++i) {
   out << *i << std::endl;
  }
  return out;
}

template< typename Stream, typename Weight>
Stream& operator>>( Stream& in, ctl::Barcode< Weight>& barcode) {
  assert(in.good());
  typedef ctl::Barcode< Weight> Barcode;
  typedef typename Barcode::size_type bsize_t;
  typedef typename Barcode::key_type Interval;
  bsize_t num;
  //ignore the word 'size' 
  in.ignore( 5, ' '); 
  in >> num;
  Interval interval;
  for( bsize_t i = 0; i < num; ++i) {
    in >> interval;
    barcode.insert( interval);
  }
  return in;
}

//I/O for all barcodes
template< typename Stream, typename Weight>
Stream& operator<<( Stream& out, const ctl::Barcodes< Weight> & barcodes) {
  typedef ctl::Barcodes< Weight> Barcodes;
  typedef typename Barcodes::const_iterator Iterator; 

  out << "max_dimension " << barcodes.size() << std::endl;
  for( Iterator i = barcodes.begin(); i != barcodes.end(); ++i) {
    out << std::endl << *i;
  }
  return out;
}

template< typename Stream, typename Weight>
Stream& operator>>( Stream& in, ctl::Barcodes< Weight>& barcodes) {
  typedef ctl::Barcodes< Weight> Barcodes;
  typedef typename Barcodes::size_type bsize_t;
  typedef typename Barcodes::iterator Iterator;
  // Get max dimension
  assert(in.good());
  bsize_t num_dimensions;
  //ignore the token 'max_dimension'
  in.ignore(256, ' ');
  in >> num_dimensions;
  barcodes.resize( num_dimensions);
  for( Iterator i = barcodes.begin(); i != barcodes.end(); ++i) {
    in >> *i;
  }
  return in;
}

} //namespace ctl

#endif //CTLIB_BARCODES_H
