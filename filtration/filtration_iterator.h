#ifndef CTLIB_FILTRATION_ITERATOR_H
#define CTLIB_FILTRATION_ITERATOR_H
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
#include <iterator>

//private functionality 
//the multiplier gracefully handles reverse iterators.
namespace _ctl{
template< typename Iterator, int multiplier>
class _filtration_iterator : 
		public std::iterator< std::random_access_iterator_tag, 
					typename Iterator::value_type> {
	public:
		
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef std::random_access_iterator_tag iterator_category;
	private:
		typedef typename Iterator::difference_type diff_type;
		typedef _filtration_iterator< Iterator, multiplier> Self;
	public:
		
	//default
	_filtration_iterator(): i(), p( 0) {}
	//copy
	_filtration_iterator( Self & f): i( f.i), p( f.p) {};
	//move
	_filtration_iterator( Self && f): i( std::move( f.i)),
					  p( std::move(f.p)) {};
	//special
	_filtration_iterator( const Iterator & i_, 
			      const std::size_t p_): i( i_), p( p_) {}    
	Self& operator++(){ 
		p+=multiplier; i++;	
		return *this; 
	}
	Self operator++(int){ 
		Self r( *this);
		++(*this);	
		return r; 
	}
	Self& operator--(){ 
		p-=multiplier; i--;	
		return *this; 
	}
	Self operator--(int){ 
		Self r( *this);
		--(*this);	
		return r; 
	}
	//deref
	pointer operator->() const { return &(*i); }
	reference operator*(){ return *i; }
	//comparisons
 	bool operator!=(const Self& r) const { return (i != r.i); }
 	bool operator==(const Self& r) const { return !operator!=(r); }
 	bool operator<(const Self& r) const { return (i < r.i); }
 	bool operator<=(const Self& r) const { return (i <= r.i); }
 	bool operator>(const Self& r) const { return (i > r.i); }
 	bool operator>=(const Self& r) const { return (i >= r.i); }
	//arithmetic
 	Self operator+(const diff_type n) const { return Self(i+n, p+n); }
	Self operator-(const diff_type n) const { return Self(i-n, p-n); }
	diff_type operator-(const Self& a) const { return i-a.i; }
	Self& operator=(const Self& r) { i = r.i; p = r.p; return *this; } 
 	Self& operator+=(const difference_type n) { i+=n; p+=n; return *this;}
 	Self& operator-=(const difference_type n) { i-=n; p-=n; return *this;}
	reference operator[](const std::size_t n) const { return *(i+n); } 	
	std::size_t pos() const { return p; } 
	//the details
	private: 
	Iterator i;
	std::size_t p;
}; //class _filtration_iterator
} //namespace ctl
#endif //CTLIB_FILTRATION_ITERATOR_H
