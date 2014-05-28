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
//Project
#include <algorithm>
#include <functional>

//CTL
#include <ctl/chain/chain_add.h>
#include <ctl/term/term_less.h>
#include <ctl/term/term_tags.h>


namespace ctl{
//exported functionality
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
	typedef typename Vector::reverse_iterator iterator;
	typedef typename Vector::const_reverse_iterator const_iterator;
	typedef typename Vector::iterator reverse_iterator;
	typedef typename Vector::const_iterator const_reverse_iterator;
	
	typedef typename Term::coeff_tag coeff_tag; 
public:
	Chain(){}
	Chain( const std::size_t n): _chain( n) {}
	Chain( const Chain & c): _chain( c._chain) {}
	Chain( const Chain && c): _chain( std::move( c._chain)) {}
	Chain( const Term & t): _chain( 1, t) {}
	template< typename Iterator, typename Compare = Less>
	Chain( Iterator begin, Iterator end, Compare c = Compare()): 
	_chain( begin, end){ 
		std::sort( _chain.begin(), _chain.end(), std::bind(c, std::placeholders::_2, std::placeholders::_1)); 
	}
	template< typename Iterator>
	Chain( Iterator begin, Iterator end, const bool sorted): 
	_chain(  begin, end) {}
	
	bool 		  empty()    const { return _chain.empty(); }
	Term&	       youngest() 	   { return _chain.front();  }
	const Term&    youngest() const	   { return _chain.front();  }

	iterator       begin() 	     	   { return _chain.rbegin(); }
	const_iterator begin() const 	   { return _chain.rbegin(); }
	iterator       end() 	     	   { return _chain.rend(); }
	const_iterator end() const   	   { return _chain.rend(); }

	//needed for scaled_add below..	
	const_iterator cbegin() const 	   { return _chain.rbegin(); }
	const_iterator cend() const   	   { return _chain.rend(); }

	reverse_iterator       rbegin() 	   { return _chain.begin(); }
	const_reverse_iterator rbegin() const 	   { return _chain.begin(); }
	reverse_iterator       rend() 	     	   { return _chain.end(); }
	const_reverse_iterator rend() const   	   { return _chain.end(); }

	//needed for scaled_add below..	
	const_reverse_iterator crbegin() const 	   { return _chain.begin(); }
	const_reverse_iterator crend() const   	   { return _chain.end(); }

	template< class... Args>	
	void emplace( Args&&... args){ _chain.emplace_back( std::forward< Args>( args)...); }
	void swap( Chain & from){ _chain.swap( from._chain); }
	template< typename Compare = Less>
 	void sort( Compare c = Less()){
		std::sort( _chain.begin(), _chain.end(), 
			   std::bind(c, std::placeholders::_2, 
					std::placeholders::_1));
	}

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
		Chain result;
		return scaled_add( a, rhs, result, c);
	}
	
	template< typename Chain, typename Compare = Less>
	Chain& scaled_add( const Coefficient & a, const Chain& rhs, 
			   Chain & result, Compare c = Compare()){
		const std::size_t n = size() + rhs.size();	
		if( result._chain.size() < n){ result._chain.resize( n); }
		auto _end = ctl::detail::chain_add( _chain.cbegin(), 
						    _chain.cend(),
				 	a, rhs.crbegin(), rhs.crend(), 
				 	result.rbegin(), std::bind(c, std::placeholders::_2, std::placeholders::_1), 
				 	coeff_tag());
		result._chain.erase( _end, result.rend());
		_chain.swap( result._chain);
		return *this;
	}

	template< typename Term, typename Compare = Less>
	Chain& add( const Term& rhs, Compare c = Compare() ){
		ctl::detail::chain_term_add( _chain, rhs, c, coeff_tag());
		return *this;
	}
	
	Chain& operator+=( const Term & b){
		ctl::detail::chain_term_add( _chain, b, 
			std::bind(Less(), std::placeholders::_2, std::placeholders::_1), coeff_tag());
		return *this;
	}

	Chain operator+( const Term & b){
		Chain result( *this);
		ctl::detail::chain_term_add( result._chain, b,
					     std::bind(Less(), std::placeholders::_2, std::placeholders::_1), 
					     coeff_tag());
		return result;
	}

	Chain operator+( const Chain & b) const {
	   Chain result( size()+b.size());
	   auto leftover = ctl::detail::chain_add( rbegin(), rend(), 
	   			            b.rbegin(), b.rend(),
	   			            result.rbegin(),
	   			            std::bind(Less(), std::placeholders::_2, std::placeholders::_1),
	   			            Chain::coeff_tag());
	   result._chain.erase( leftover, result.rend());
	   return result;
	}
 private:
   Coefficient& normalize( ctl::detail::term_z2_tag) const { 
	return Coefficient( 1); 
   }
   Coefficient& normalize( ctl::detail::term_z2_tag, bool) const { 
	return Coefficient( 1); 
   }
   Coefficient& normalize( ctl::detail::term_non_z2_tag, bool) const  {
    	if (_chain.empty()) { return Coefficient( 0); }
    	return this->youngest.coefficient().inverse();
   }
   Coefficient& normalize( ctl::detail::term_non_z2_tag)  {
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

} //namespace ctl

//\alpha*[chain] = [chain]*\alpha
template< typename T, typename L>
inline typename ctl::Chain< T, L>& operator*( const typename T::Coefficient & s, 
			      ctl::Chain< T, L>& c){ return c*s; }

#endif //CTL_CHAIN_H