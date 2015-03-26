#ifndef CTLIB_CONCURRENT_FILTRATION_H
#define CTLIB_CONCURRENT_FILTRATION_H
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

#include <tbb/parallel_sort.h>
#include <vector>
#include <tbb/concurrent_vector.h>

#include <ctl/filtration/less.hpp>
#include <ctl/filtration/filtration_iterator.hpp>

namespace ctl{
//non-exported functionality 
namespace _filtration{
} //anonymous namespace

namespace parallel{
template< typename Complex_, 
	  typename Less_ = ctl::Cell_less,
	  typename Iterator_ = typename Complex_::iterator, 
	  typename Vector_ = tbb::concurrent_vector< Iterator_> >
class Filtration {
 public:
	typedef  Complex_ Complex;
	typedef  Less_ Less;
 private:
	typedef Vector_ Vector;
	typedef typename Vector::iterator _viterator;
	typedef typename Vector::const_iterator _vciterator;
	typedef typename Vector::reverse_iterator _vriterator;
	typedef typename Vector::const_reverse_iterator _vcriterator;
 	typedef typename Complex::Cell_boundary Cell_boundary;
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
	typedef typename Vector::iterator iterator;
	typedef typename Vector::const_iterator const_iterator;
	typedef typename Vector::reverse_iterator reverse_iterator;
	typedef typename Vector::const_reverse_iterator const_reverse_iterator;
 
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
	filtration_( std::move( f)), complex_( std::move( f.complex_)) {}	
	//these are constructors used to hack together a filtration
	Filtration( Complex & c, bool flag): 
	filtration_( c.size()), complex_( c) {}	
	Filtration( Complex & c, std::size_t size, bool flag): 
	filtration_( size), complex_( c) {}	
	//this is a default filtration
	Filtration( Complex & c): filtration_( c.size()), complex_( c){
	   std::size_t pos = 0;
	   for( auto i= c.begin(); i != c.end(); ++i, ++pos){ 
	   	 filtration_[ pos] = i;
	   }
	   Less less;
	   tbb::parallel_sort( filtration_.begin(), filtration_.end(), less);
	}

	reference operator[] (std::size_t n) { return filtration_[ n]; }
	const_reference operator[] (std::size_t n) const { 
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
	rit rend()   { return filtration_.rend();          }
	
	crit rbegin() const { return filtration_.rbegin(); }
	crit rend()   const { return filtration_.rend();   }

	Complex& complex() const { return complex_; }

	std::size_t size() const { return filtration_.size(); } 
 private:
	Vector filtration_;
	Complex& complex_;
}; //class Filtration
} //namespace parallel
} //namespace ct
#endif //CTLIB_CONCURRENT_FILTRATION_H
