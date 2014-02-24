#ifndef CTLIB_CHAIN_H
#define CTLIB_CHAIN_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the below liscence, but please
* do not commit acts of academic dishonesty. We encourage and request that 
* for any academic use of this source code one should cite the following 
* papers:
* 
* \cite{$bibtex_names_here}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) $NAMES_OF_AUTHORS $YEARS_FIRST_CREATED <$emails>
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

//exported functionality
namespace ctl{

template< typename _Term, 
	  typename _Less = class ctl::Term_less>
class Chain {
 public:
 	typedef _Term Term;
	typedef _Less Less;
 private:
	typedef std::vector< Term> Vector;
	typedef Chain< Term, Less> Self;
 public:
	typedef typename Vector::iterator iterator;
	typedef typename Vector::const_iterator const_iterator;
	typedef typename Term::coeff_tag coeff_tag; 
public:
	Chain(){}
	Chain( const std::size_t n): _chain( n) {}
	Chain( const Chain & c): _chain( c) {}
	Chain( const Chain && c): _chain( std::move( c._chain)) {}
	Chain( const Term & t): _chain( 1, t) {}
	template< typename Iterator, typename Compare = Less>
	Chain( Iterator begin, Iterator end, Compare c = Compare()): 
	_chain(  begin, end) { 
		std::cout << "bout to sort." << std::flush << std::endl;
		std::sort( _chain.begin(), _chain.end(), c); 
	};
	template< typename Iterator>
	Chain( Iterator begin, Iterator end, const bool sorted): 
	_chain(  begin, end) { std::cout << "schwat?" << std::endl; };
	

	iterator       begin() 	     { return _chain.begin(); }
	const_iterator begin() const { return _chain.begin(); }
		
	iterator       end() 	     { return _chain.end(); }
	const_iterator end() const   { return _chain.end(); }

	std::size_t   size() const   { return _chain.size(); }	
	
	Chain& operator=( const Chain& from){ _chain = from.chain; }
	Chain& operator=( Chain&& from){ _chain = std::move( from.chain); }

	Chain operator+( const Term & b){
		Chain result( *this);
		return _ctl::chain_add( result, b, coeff_tag());
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
