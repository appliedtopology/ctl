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
	Filtration(): filtration_() {};
	Filtration( const Filtration & f): filtration_( f) {}
	Filtration( const Filtration && f): filtration_( std::move( f)) {}	
	Filtration( Complex & complex): 
	filtration_( complex.size()){
		auto i = filtration_.begin();
		for( auto j= complex.begin(); j != complex.end(); ++i, ++j){ 
			 *i = j;
		}
		std::sort( filtration_.begin(), filtration_.end(), Less());
	}

	//used typedefs above since the names were getting to long
	it  begin() { return it( filtration_.begin(), 0);    }
	it  end()   { return it( filtration_.end(), size()); }
	
	cit begin() const { return cit( filtration_.begin(), 0);  }
	cit end() const { return cit( filtration_.end(), size()); }

	rit rbegin() { return rit( filtration_.rbegin(), size()-1); }
	rit rend()   { return rit( filtration_.rend(), 0);          }
	
	crit rbegin() const { return crit( filtration_.rbegin(), size()-1); }
	crit rend()   const { return crit( filtration_.rend(), 0);          }

	std::size_t size() const { return filtration_.size(); } 
 private:
	Vector filtration_;
}; //class Filtration

} //namespace ct
#endif //CTLIB_FILTRATION_H
