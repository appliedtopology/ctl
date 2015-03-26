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
#include <boost/property_map/property_map.hpp>
#include <boost/iterator/permutation_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
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
  template< typename T>
  value_type operator[]( const T & t) const {
	return get( *this, t);
  }
};

template< typename Filtration_iterator>
typename Pos_offset_map< Filtration_iterator>::value_type 
get( const Pos_offset_map< Filtration_iterator>& map, 
     const std::size_t pos) { 
	return pos; 
}

template< typename Filtration_iterator>
struct Permutation_offset_map {
  // types
  typedef Filtration_iterator              key_type;
  typedef std::size_t                      value_type;
  typedef std::size_t&                     reference;
  typedef boost::readable_property_map_tag category; 
  Permutation_offset_map() {}
};

template< typename Filtration_iterator, typename Index_type_iterator>
typename Permutation_offset_map< Filtration_iterator>::value_type 
get( const Permutation_offset_map< Filtration_iterator> & map , 
     const boost::permutation_iterator< Filtration_iterator, 
				        Index_type_iterator> p){
	return *(p.base());
}

template< typename Filtration_iterator, typename Index_type_iterator>
typename Pos_offset_map< Filtration_iterator>::value_type 
get( const Pos_offset_map< Filtration_iterator> & map , 
     const boost::permutation_iterator< Filtration_iterator, 
				        Index_type_iterator> p){
	return *(p.base());
}

template< typename Filtration_iterator, typename Predicate>
typename Pos_offset_map< Filtration_iterator>::value_type 
get( const Pos_offset_map< Filtration_iterator> & map , 
     const boost::filter_iterator< Predicate, Filtration_iterator> p){
	return std::distance(map.begin, (p.base()));
}





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


template< typename Filtration_iterator, typename Coefficient>
typename Pos_offset_map< Filtration_iterator>::value_type 
get( const Pos_offset_map< Filtration_iterator>& map, 
     const ctl::Term< std::size_t, Coefficient> & term) { 
	return term.cell();
}



//Necessary for oldschool boundary operator
template< typename Filtration_iterator> 
struct Id_offset_map {
  // types
  typedef Filtration_iterator                key_type;
  typedef std::size_t                      value_type;
  typedef std::size_t&                     reference;
  typedef boost::readable_property_map_tag category; 
  Id_offset_map() {}
};

template< typename Cell>
typename Id_offset_map< Cell>::value_type 
get( Id_offset_map< Cell>, const Cell& cell) { return (*cell)->second.id(); }

template< typename Cell>
typename Id_offset_map< Cell>::value_type 
get( Id_offset_map< Cell>, const std::size_t pos) { return pos; }

template< typename Cell>
typename Id_offset_map< Cell>::value_type 
get( Id_offset_map< Cell>, const typename Cell::value_type& cell) { 
	return cell->second.id(); 
}

} //end namespace ctl
#endif //CTLIB_OFFSET_MAPS_H
