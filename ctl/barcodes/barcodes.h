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
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program in a file entitled COPYING; if not, write to the 
* Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************************
*******************************************************************************
* NOTES
* We use the type alias in C++11. This is ok for the definitions, but, we might
* be muddying the waters by defining operator<< and operator>> on them.
* on the other hand it doesn't seem like this should really cause an issue here.
*******************************************************************************/
//STL
#include <unordered_multiset>
#include <vector>
#include <pair>

//exported functionality
namespace ctl{
//each barcode is composed of intervals
template< typename Weight>
using Interval = std::pair< Weight, Weight>;

//a multiset of intervals in a fixed dimension is a barcode
template< typename Weight>
using Barcode = std::unordered_multiset< ctl::Interval< weight> >;

//and the barcodes for a space are an array of them, one for each dimension
template< typename Weight>
using Barcodes = std::vector< Barcode< Weight> >;

//I/O for an interval
template< typename Stream, typename Weight>
Stream& operator<<( Stream& out, const ctl::Interval< Weight>& interval) {
  out << "[" << interval.first << " " << interval.second << "]";
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
  out << "size " << barcode.size() << std::endl;
  typedef typename Barcode< Weight>::const_iterator Iterator;
  for( Iterator i = barcode.begin(); i != barcode.end(); ++i) {
   out << *i;
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
  typedef Barcodes::size_type bsize_t;
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
