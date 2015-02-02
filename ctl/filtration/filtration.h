#ifndef CTLIB_FILTRATION_H
#define CTLIB_FILTRATION_H
#include <iostream>
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
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
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
*******************************************************************************
*******************************************************************************/
//STL

//CTL
#include <ctl/filtration/less.h>
#include <ctl/filtration/filtration_iterator.h>

//BOOST
#include <boost/serialization/base_object.hpp>

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
     friend class boost::serialization::access; 

     template <typename Archive> 
     friend void serialize(Archive &ar, const unsigned int version){
		ar & filtration_;
		ar & complex_;
     }

     Vector filtration_;
     Complex& complex_;
}; //class Filtration

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, const Filtration< Complex, L>& f){
	for ( auto & i : f){
		out << i->second.id() << ": " << i->first << std::endl;
	}
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, const Filtration< Complex, L>&& f){
	out << f;
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, 
		    const typename Filtration< Complex, L>::const_reverse_iterator f){
	out << (*f)->first;
	return out;
}
template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, 
		    const typename Filtration< Complex, L>::reverse_iterator f){
	out << (*f)->first;
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, 
		    const typename Filtration< Complex, L>::iterator f){
	out << (*f)->first;
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, 
		    const typename Filtration< Complex, L>::const_iterator f){
	out << (*f)->first;
	return out;
}

} //namespace ctl
#endif //CTLIB_FILTRATION_H
