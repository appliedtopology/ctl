#ifndef CTLIB_UNORDERED_MAP_H
#define CTLIB_UNORDERED_MAP_H
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
#include <unordered_map>

//exported functionality
namespace ctl{

template < class Key,                                    
           class T,                                      
           class Hash = std::hash<Key>,                       
           class Pred = std::equal_to<Key>,                   
           class Alloc = std::allocator< std::pair<const Key,T> >  
           > class unordered_map : private std::unordered_map< Key, T, Hash, Pred, Alloc>{ 
	typedef std::unordered_map< Key, T, Hash, Pred, Alloc> Base;
	public:
	typedef Base::key_type                  key_type;
	typedef Base::mapped_type               mapped_type;
	typedef Base::mapped_type               mapped_type;
	typedef Base::value_type                value_type;
	typedef Base::mapped_type               mapped_type;
	typedef Base::hasher                    hasher;
	typedef Base::key_equal                 key_equal;
	typedef Base::allocator_type            allocator_type;
	typedef Base::reference                 reference;
	typedef Base::const_reference           const_reference;
	typedef Base::pointer                   pointer;
	typedef Base::const_pointer             const_pointer;
	typedef Base::iterator                  iterator;
	typedef Base::const_iterator            const_iterator;
	typedef Base::local_iterator            local_iterator;
	typedef Base::const_local_iterator      const_local_iterator;
	typedef Base::size_type                 size_type;
	typedef Base::difference_type           difference_type;
	explicit unordered_map ( size_type n = 12,
	                         const hasher& hf = hasher(),
	                         const key_equal& eql = key_equal(),
	                         const allocator_type& alloc = allocator_type()):  Base( n, hf, eql, alloc){} 
	unordered_map ( const unordered_map& ump ): Base( ump) {}
	unordered_map ( unordered_map&& ump ):Base::unordered_map( std::forward( ump)) {}
	explicit unordered_map ( const allocator_type& alloc ): Base( alloc) {} 
	unordered_map ( const unordered_map& ump, const allocator_type& alloc ): Base( ump, alloc) {}
	unordered_map ( unordered_map&& ump, const allocator_type& alloc ): Base( std::forward( ump), alloc) {}
	template <class InputIterator>
	unordered_map ( InputIterator first, InputIterator last,
	                size_type n = 12,
	                const hasher& hf = hasher(),
	                const key_equal& eql = key_equal(),
	                const allocator_type& alloc = allocator_type()) : Base( first, last, n, hf, eql, alloc) {}
	unordered_map ( initializer_list<value_type> il,
	                size_type n = 12,
			const hasher& hf = hasher(),
	                const key_equal& eql = key_equal(),
	                const allocator_type& alloc = allocator_type() ) : Base( il, n, hf, eql, alloc) {}
};
	private:
	public:
	void rehash( size_type n){
		constexpr num_bits = sizeof( std::size_t)*8;
		std::size_t bucket_count = bucket_count();
		Base::rehash( n);
		if( bucket_count() > bucket_count){
			bucket_count = bucket_count();
			p = 0;
			std::size_t log_index = bucket_count;
			while( log_index >>=1){ ++p; }
			(1<<32)*(1<<p)
			m = 
				
		}
	}

	std::size_t p = 4;
	std::size_t m = 1431655766;

} //namespace ctl

//non-exported functionality
namespace {
} //anonymous namespace

#endif //CTLIB_UNORDERED_MAP_H
