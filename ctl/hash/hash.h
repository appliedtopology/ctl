#ifndef CTLIB_HASH_H
#define CTLIB_HASH_H
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
*******************************************************************************/

//STL
#include <cstring>

//CTL
namespace ctl{
namespace hash{
#include <ctl/hash/city.h>
#include <ctl/hash/MurmurHash3.h>
} //end namespace hash
} //end namespace ctl
//non-exported functionality 
namespace ctl{
namespace detail{ 
namespace cth = ctl::hash;
template< typename Container>
inline std::size_t murmur3_hash( const Container & key){
	typedef typename Container::value_type T;
	std::size_t out;
	//MurmurHash3_x86_32 
	//MurmurHash3_x86_128
	cth::MurmurHash3_x64_128 ( (const char*)&(*(key.begin())), 
				    sizeof(T)*key.size(), key.size(), &out );
	return out;
}

template< typename Container>
inline std::size_t city_hash( const Container & key){
	typedef typename Container::value_type T;
	return cth::CityHash64WithSeed( (const char*)&(*(key.begin())), 
					 sizeof(T)*key.size(),key.size()); 
}

template< typename T>
inline std::size_t pjw_hash( const T & key){
    std::size_t h, g;
    h = 0;
    for( auto i : key){
      h = (h << 4) + i;
      if ((g = h & 0xf0000000)) {
	h = h^(g >> 24);
	h = h^g;
      }
    }
   return h;
} 
template< typename T>
inline std::size_t jenkins_hash( const T & key){
    std::size_t hash=0;
    for(auto i : key){
        hash += i;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

} //detail namespace
} //ctl namespace

#define CTL_USE_CITY
namespace ctl {
	template< typename T>
	struct Hash{
		std::size_t operator()( const T & key) const{
			#ifdef  CTL_USE_MURMUR
				return detail::murmur3_hash( key);
			#elif defined( CTL_USE_CITY)
				return detail::city_hash( key);
			#elif defined( CTL_USE_JENKINS)
				return detail::jenkins_hash( key);
			#else 
				return detail::pjw_hash( key);
			#endif
					
		}
	}; //class Hash
} //namespace ctl
#endif //CTLIB_HASH_H
