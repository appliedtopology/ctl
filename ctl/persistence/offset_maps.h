#ifndef CTLIB_OFFSET_MAPS_H
#define CTLIB_OFFSET_MAPS_H
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
* \cite{kuhl-design-96, z-ct-10}
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
*******************************************************************************/
#include <boost/property_map/property_map.hpp>

//code here modeled after:

namespace ctl {

template< typename Filtration_iterator>
struct Pos_offset_map {
  // types
  typedef Filtration_iterator              key_type;
  typedef std::size_t                      value_type;
  typedef std::size_t&                     reference;
  typedef boost::readable_property_map_tag category; 
  Pos_offset_map( const Filtration_iterator _b) : begin( _b) {}
  Filtration_iterator begin;
};
/*
template< typename Filtration_iterator>
typename Pos_offset_map< Filtration_iterator>::value_type 
get( const Pos_offset_map< Filtration_iterator>& map, const std::size_t pos) { 
	return pos; 
}*/

template< typename Filtration_iterator>
typename Pos_offset_map< Filtration_iterator>::value_type 
get( const Pos_offset_map< Filtration_iterator>& map , 
     const Filtration_iterator& cell) { 
	return std::distance(map.begin, cell);
}

template< typename Filtration_iterator, typename Coefficient>
typename Pos_offset_map< Filtration_iterator>::value_type 
get( const Pos_offset_map< Filtration_iterator>& map, 
     const ctl::Term< Filtration_iterator, Coefficient> & term) { 
	return std::distance(map.begin, term.cell());
}
/*
template< typename Cell>
struct Id_offset_map {
  // types
  typedef Cell                             key_type;
  typedef std::size_t                      value_type;
  typedef std::size_t&                     reference;
  typedef boost::readable_property_map_tag category; 
  Id_offset_map() {}
};

template< typename Cell>
typename Id_offset_map< Cell>::value_type 
get( Id_offset_map< Cell>, const Cell& cell) { return cell->second.id(); }
*/
} //end namespace ctl
#endif //CTLIB_OFFSET_MAPS_H
