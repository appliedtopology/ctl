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
//STL

//CTL
#include <ctl/filtration/less.h>
#include <ctl/filtration/filtration_iterator.h>

//non-exported functionality 
namespace ctl {
namespace detail{
} //anonymous namespace
}// namespace ctl

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
	typedef typename Complex::Boundary Cell_boundary;
	typedef typename Cell_boundary::Term Cell_term;
	typedef typename Cell_term::Coefficient Coefficient;
 public:
	/*
	typedef typename ctl::detail::_filtration_iterator< _viterator, 1> 
								   iterator;
	typedef typename ctl::detail::_filtration_iterator< _vciterator, 1> 
								const_iterator;
	typedef typename ctl::detail::_filtration_iterator< _viterator, -1> 
							       reverse_iterator;
	typedef typename ctl::detail::_filtration_iterator<  _vciterator, -1>
							 const_reverse_iterator;
	*/
	typedef _viterator iterator;
	typedef _vciterator const_iterator;
	typedef _vriterator reverse_iterator;
	typedef _vcriterator const_reverse_iterator;

	//A graded term! iterators themselves can represent grading via address.
	typedef typename Cell_term::template 
			rebind< iterator, Coefficient>::T Term;
	typedef typename Vector::value_type value_type;
	typedef typename Vector::reference reference;
	typedef typename Vector::const_reference const_reference;
	
private:
	//just to make typing below easier
	typedef iterator it;
	typedef const_iterator cit;
	typedef const_reverse_iterator crit;
	typedef reverse_iterator rit;
public:
	Filtration( const Filtration & f): 
		filtration_( f), complex_( f.complex_) {}
	Filtration( const Filtration && f): 
		filtration_( std::move( f)), complex_( f.complex_) {}	
	//create a noncorrect filtration first.
	//useful if we are going to create a filtration inside another
	//algorithm
	Filtration( Complex & c, bool flag): filtration_( c.size()), 
					     complex_( c) {}
	//maybe we have an empty complex, but we know its size in advance	
	Filtration( Complex & c, std::size_t s, bool flag): filtration_( s), 
							    complex_( c) {}
	//default constructor	
	Filtration( Complex & c): filtration_( c.size()), complex_( c){
		std::size_t pos = 0;
		for( auto i= c.begin(); i != c.end(); ++i, ++pos){ 
			 filtration_[ pos] = i;
		}
		std::sort( filtration_.begin(), filtration_.end(), Less());
	}
	reference operator[] ( std::size_t n) { return filtration_[ n]; }
	const_reference operator[] ( std::size_t n) const { 
		return filtration_[ n]; 
	}
	//used typedefs above since the names were getting to long
	it  begin() { return filtration_.begin();    }
	it  end()   { return filtration_.end(); }
	
	cit begin() const { return filtration_.begin();  }
	cit end() const { return filtration_.end(); }

	cit cbegin() const { return filtration_.begin();  }
	cit cend() const { return filtration_.end(); }

	rit rbegin() { return filtration_.rbegin(); }
	rit rend()   { return filtration_.rend();   }
	
	crit rbegin() const { return filtration_.rbegin(); }
	crit rend()   const { return filtration_.rend();   }
	Complex& complex() const { return complex_;}
	std::size_t size() const { return filtration_.size(); } 
 private:
	Vector filtration_;
	Complex& complex_;
}; //class Filtration

} //namespace ct
#endif //CTLIB_FILTRATION_H
