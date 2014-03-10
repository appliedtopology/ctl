#ifndef CTLIB_FILTRATION_H
#define CTLIB_FILTRATION_H
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

#include "filtration/less.h"
#include "filtration/filtration_iterator.h"

//non-exported functionality 
namespace {
//needed in filtration constructor
template< typename Iterator>
class _transform_iterator : 
		public std::iterator< std::input_iterator_tag, Iterator> {
	private:
		typedef _transform_iterator< Iterator> Self;
	public:
	//default
	_transform_iterator() {}
	//copy
	_transform_iterator( Self & f): _i( f._i) {};
	//move
	_transform_iterator( Self && f): _i( std::move(f._i)) {};
	//special
	_transform_iterator( Iterator & i_): _i( i_) {};
	_transform_iterator& operator++(){ _i++; return *this; }
 	Iterator* operator->() { return &(_i); }
 	bool operator!=(const Self& r) const { return (_i != r._i); }
 	bool operator==(const Self& r) const { return !operator!=(r); }
	Self& operator=(const Self& r) { _i = r._i; return *this; } 
	//the details
	Iterator operator*(){ return _i; }
	private:
	Iterator _i;
}; //class _transform_iterator
template< typename Iterator>
inline _transform_iterator< Iterator> _create_transform_iterator ( Iterator i){
 	return _transform_iterator< Iterator>( i);
 }

} //anonymous namespace


namespace ctl{
template< typename Complex_, 
	  typename Less_ = ctl::Cell_less>
class Filtration {
 public:
	typedef  Complex_ Complex;
	typedef  Less_ Less;
 private:
	typedef typename Complex::iterator _Iterator;	
	typedef typename std::vector< _Iterator> Vector;
	typedef typename Vector::iterator _viterator;
	typedef typename Vector::const_iterator _vciterator;
	typedef typename Vector::reverse_iterator _vriterator;
	typedef typename Vector::const_reverse_iterator _vcriterator;
 public:
	typedef typename _ctl::_filtration_iterator< _viterator, 1> iterator;
	typedef typename _ctl::_filtration_iterator< _vciterator, 1> 
							const_iterator;
	typedef typename _ctl::_filtration_iterator< _viterator, -1> 
							reverse_iterator;
	typedef typename _ctl::_filtration_iterator<  _vciterator, -1>
							const_reverse_iterator;
 private:
	//just to make typing below easier
	typedef iterator it;
	typedef const_iterator cit;
	typedef const_reverse_iterator crit;
	typedef reverse_iterator rit;
public:
	Filtration(): _filtration() {};
	Filtration( const Filtration & f): _filtration( f) {}
	Filtration( const Filtration && f): _filtration( std::move( f)) {}	
	Filtration( Complex & complex): 
	_filtration( _create_transform_iterator(complex.begin()), 
		      _create_transform_iterator( complex.end())){ 
		Less less;
		std::sort( _filtration.begin(), _filtration.end(), less);
	}

	//used typedefs above since the names were getting to long
	it  begin() { return it( _filtration.begin(), 0);    }
	it  end()   { return it( _filtration.end(), size()); }
	
	cit begin() const { return cit( _filtration.begin(), 0);  }
	cit end() const { return cit( _filtration.end(), size()); }

	rit rbegin() { return rit( _filtration.rbegin(), size()-1); }
	rit rend()   { return rit( _filtration.rend(), 0);          }
	
	crit rbegin() const { return crit( _filtration.rbegin(), size()-1); }
	crit rend()   const { return crit( _filtration.rend(), 0);          }

	std::size_t size() const { return _filtration.size(); } 
 private:
	Vector _filtration;
}; //class Filtration

} //namespace ct
#endif //CTLIB_FILTRATION_H
