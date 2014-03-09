#ifndef CTLIB_PROPERTY_MAPS_H
#define CTLIB_PROPERTY_MAPS_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the below liscence, but please
* do not commit acts of academic dishonesty. We encourage and request that 
* for any academic use of this source code one should cite the following 
* papers:
* 
* \cite{$bibtex_names_here}
* 
* See ct.bib for the corresponding bibtex entries. 
****************************************************
* !!! DO NOT CITE THE USER MANUAL !!!
****************************************************
* NOTES: Code in this file has been modelled after:
* http://www.boost.org/doc/libs/1_55_0/libs/property_map/doc/property_map.html
* -- see #sec:property-map-types
*******************************************************************************
* Copyright (C) $NAMES_OF_AUTHORS $YEARS_FIRST_CREATED <$emails>
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

template< typename Cell>
struct Pos_property_map {
  // types
  typedef Cell                             key_type;
  typedef std::size_t                      value_type;
  typedef std::size_t&                     reference;
  typedef boost::readable_property_map_tag category;
  Pos_property_map() {}
};

template< typename Cell>
typename Pos_property_map< Cell>::value_type 
get( Pos_property_map< Cell>, const Cell& cell) { return cell->second.pos(); }

template< typename Cell>
struct Id_property_map {
  // types
  typedef Cell                             key_type;
  typedef std::size_t                      value_type;
  typedef std::size_t&                     reference;
  typedef boost::readable_property_map_tag category;
  Id_property_map() {}
};

template< typename Cell>
typename Id_property_map< Cell>::value_type 
get( Id_property_map< Cell>, const Cell& cell) { return cell->second.id(); }

} //end namespace ctl
#endif //CTLIB_PROPERTY_MAPS_H
