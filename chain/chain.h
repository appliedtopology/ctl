#ifndef CTLIB_CHAIN_H
#define CTLIB_CHAIN_H
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
#include "chain/chain_add.h"
#include "term/term_less.h"
#include "term/term_tags.h"
#include <algorithm>

//exported functionality
namespace ctl{

template< typename _Term, 
	  typename _Less = std::less< _Term> >
class Chain {
 public:
 	typedef _Term Term;
	typedef _Less Less;
	typedef typename Term::Coefficient Coefficient; 
 private:
	typedef std::vector< Term> Vector;
	typedef Chain< Term, Less> Self;
 public:
	typedef typename Vector::value_type value_type;
	typedef typename Vector::iterator iterator;
	typedef typename Vector::const_iterator const_iterator;
	typedef typename Term::coeff_tag coeff_tag; 
public:
	Chain(){}
	Chain( const std::size_t n): _chain( n) {}
	Chain( const Chain & c): _chain( c._chain) {}
	Chain( const Chain && c): _chain( std::move( c._chain)) {}
	Chain( const Term & t): _chain( 1, t) {}
	template< typename Iterator, typename Compare = Less>
	Chain( Iterator begin, Iterator end, Compare c = Compare()): 
	_chain( begin, end){ std::sort( _chain.begin(), _chain.end(), c); }
	template< typename Iterator>
	Chain( Iterator begin, Iterator end, const bool sorted): 
	_chain(  begin, end) {}
	
	bool 		  empty()    const { return _chain.empty(); }
	Term&	       youngest() 	   { return _chain.back();  }
	const Term&    youngest() const	   { return _chain.back();  }
	iterator       begin() 	     	   { return _chain.begin(); }
	const_iterator begin() const 	   { return _chain.begin(); }
		
	iterator       end() 	     	   { return _chain.end(); }
	const_iterator end() const   	   { return _chain.end(); }

	//needed for scaled_add below..	
	const_iterator cbegin() const 	   { return _chain.begin(); }
	const_iterator cend() const   	   { return _chain.end(); }
	

	std::size_t   size() const   	   { return _chain.size(); }	
	void reserve( const std::size_t n) { _chain.reserve( n); } 
	Chain& operator=( const Chain& from){ 
		_chain = from._chain; 
		return *this;
	}
	Chain& operator=( Chain&& from){ 
		_chain = std::move( from._chain); 
		return *this;
	}
	
	Coefficient& normalize() { return normalize( coeff_tag()); }
	Coefficient& normalize( bool) const { 
		return normalize( coeff_tag(), 1); 
	}

	template< typename Chain, typename Compare = Less>
	Chain& scaled_add( const Coefficient & a, const Chain& rhs, 
					      Compare c = Compare()){
		Chain result( size() + rhs.size());	 
		const_iterator 
		_end = _ctl::chain_add( _chain.cbegin(), _chain.cend(),
				 	a, rhs.begin(), rhs.end(), 
				 	result.begin(), c, 
				 	coeff_tag());
		//hopefully should cause no deallocation
		//investigate this
		result._chain.erase( _end, result.end());
		//currently investigating if this is faster than swap.
		_chain = std::move( result._chain);  
		return *this;
	}

	template< typename Term, typename Compare = Less>
	Chain& add( const Term& rhs, Compare c = Compare() ){
		_ctl::chain_term_add( _chain, rhs, c, coeff_tag());
		return *this;
	}
	
	Chain& operator+=( const Term & b){
		_ctl::chain_term_add( _chain, b, Less(), coeff_tag());
		return *this;
	}

	Chain operator+( const Term & b){
		Chain result( *this);
		_ctl::chain_term_add( result._chain, b, Less(), coeff_tag());
		return result;
	}

	Chain operator+( const Chain & b) const {
	   Chain result( size()+b.size());
	   auto leftover = _ctl::chain_add( begin(), end(), 
	   			            b.begin(), b.end(),
	   			            result.begin(),
	   			            Less(),
	   			            Chain::coeff_tag());
	   result._chain.erase( leftover, result.end());
	   return result;
	}
 private:
   Coefficient& normalize( _ctl::term_z2_tag) const { return Coefficient( 1); }
   Coefficient& normalize( _ctl::term_z2_tag, bool) const { 
	return Coefficient( 1); 
   }
   Coefficient& normalize( _ctl::term_non_z2_tag, bool) const  {
    	if (_chain.empty()) { return Coefficient( 0); }
    	return this->youngest.coefficient().inverse();
   }
   Coefficient& normalize( _ctl::term_non_z2_tag)  {
    	if (_chain.empty()) { return Coefficient( 0); }
    	auto inverse = this->youngest.coefficient().inverse();
    	if ( inverse != 1){ (*this)*=inverse; }
    	return inverse;
    }
private:
	Vector _chain;
}; //class Chain

//allow term+Chain = Chain+term
template< typename Chain>
Chain& operator+( const typename Chain::Term & b, Chain & a){ return a+b; }

} //namespace ct

//\alpha*[chain] = [chain]*\alpha
template< typename T, typename L>
inline typename ctl::Chain< T, L>& operator*( const typename T::Coefficient & s, 
			      ctl::Chain< T, L>& c){ return c*s; }

//non-exported functionality
namespace {} //anonymous namespace

#endif //CTLIB_CHAIN_H
