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
//STL
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

//CTL
#include <ctl/chain/chain_add.h>
#include <ctl/term/term_less.h>
#include <ctl/term/term_tags.h>
#include <ctl/io/io.h>

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
	Chain( const std::size_t n, Term t = Term()): _chain( n, t) {}
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
	reverse_iterator erase( reverse_iterator b, reverse_iterator e){
		//since we are lying in this datastructure and reverse
		//iterators really are iterators we can erase from begin --> erase
		return _chain.erase( b, e);
	}
	reverse_iterator erase( reverse_iterator b){
		return _chain.erase( b);
	}

	iterator erase( iterator b){
		return iterator(  _chain.erase( std::next( b).base() ));
	}

	void swap( Chain & from){ _chain.swap( from._chain); }
	void clear(){ _chain.clear(); }

	template< typename Compare = Less>
 	void sort( Compare c = Less()){
		std::sort( _chain.begin(), _chain.end(), 
			   std::bind(c, std::placeholders::_2, 
					std::placeholders::_1));
	}

	std::size_t   size() const   	   { return _chain.size(); }	
	void reserve( const std::size_t n) { _chain.reserve( n); } 
	void resize( const std::size_t n) { _chain.resize( n); } 
	bool operator==( const Chain & from) const { return std::equal( rbegin(), rend(), from.rbegin()); }
	bool operator!=( const Chain & from) const { return !std::equal( rbegin(), rend(), from.rbegin()); }
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
	private:
	template< typename Iterator, typename Persistence_data, 
					typename Compare>
	inline void mismatch_prefetch( Iterator& f1, Iterator& e1,
			      Iterator& f2, Iterator& e2,
			      Compare & c,
			      Persistence_data & d,
			      std::size_t& n, 
			      ctl::detail::term_non_z2_tag t) const {}
		
	template< typename Iterator, typename Persistence_data, 
		  typename Compare>
	inline void 
	mismatch_prefetch( Iterator& f1, Iterator& e1, 
			   Iterator& f2, Iterator& e2,
			   Compare & c,
			   Persistence_data & d,
			   std::size_t& n, ctl::detail::term_z2_tag t) const {
		while( f1 != e1 && f2 != e2){ 
		if( c(*f1, *f2)){
		    const auto i = d.cascade_boundary_map[ f1->cell()].rbegin();
		    __builtin_prefetch( std::addressof( *i));
		    break;
		}
		if( c( *f2, *f1)){
		    const auto i = d.cascade_boundary_map[ f2->cell()].rbegin();
		     __builtin_prefetch( std::addressof( *i));
		     break;
		}
                   ++f1, ++f2;
		   n -= 2;
                }
	} 
		
	public:
	template< typename Chain, 
		  typename Persistence_data, 
		  typename Compare = Less>
	Chain& scaled_add( const Coefficient & a, const Chain& rhs,
			   Persistence_data & data, 
			   Chain & result, Compare c = Compare()){
		typedef typename Chain::const_reverse_iterator Iterator_;
		Iterator_ first = crbegin();
		Iterator_ rhs_first = rhs.crbegin();
		Iterator_ end =   crend();
		Iterator_ rhs_end = rhs.crend();
		std::size_t n = size() + rhs.size();
		//Z2 only:
		//Walk up the chain from bottom to top looking for 
		//the first mismatch. 
		//If it exists the younger of the two is the next 
		//youngest element.
		mismatch_prefetch( first, end, rhs_first, rhs_end, c, 
				   data, n, coeff_tag());
		if( result._chain.size() < n){ result._chain.resize( n); }
		auto _end = 
		ctl::detail::chain_add( first, end, a, rhs_first, rhs_end, 
				 	result.rbegin(), 
					std::bind(c, std::placeholders::_2, 
						     std::placeholders::_1), 
				 	coeff_tag());
		result._chain.erase( _end, result.rend());
		_chain.swap( result._chain);
		return *this;
	}
	template< typename Chain, typename Compare = Less>
	Chain& scaled_add( const Coefficient & a, const Chain& rhs, 
			   Chain & result, Compare c = Compare()){
		const std::size_t n = size() + rhs.size();
		if( result._chain.size() < n){ result._chain.resize( n); }
		auto _end = 
		ctl::detail::chain_add( _chain.cbegin(), _chain.cend(), a, 
					rhs.crbegin(), rhs.crend(),
				 	result.rbegin(), 
					std::bind(c, std::placeholders::_2, 
						     std::placeholders::_1), 
				 	coeff_tag());
		result._chain.erase( _end, result.rend());
		_chain.swap( result._chain);
		return *this;
	}

	template< typename Term, typename Compare = Less>
	Chain& add( const Term& rhs, Compare c = Compare() ){
		ctl::detail::chain_term_add( _chain, rhs, std::bind(c, std::placeholders::_2, std::placeholders::_1), 
					      coeff_tag());
		return *this;
	}
	
	Chain& operator+=( const Term & b){
		ctl::detail::chain_term_add( _chain, b, 
			std::bind(Less(), std::placeholders::_2, 
				  std::placeholders::_1), coeff_tag());
		return *this;
	}

	Chain operator+( const Term & b){
		Chain result( *this);
		ctl::detail::chain_term_add( result._chain, b,
					     std::bind(Less(), 
					     std::placeholders::_2, 
					     std::placeholders::_1), 
					     coeff_tag());
		return result;
	}

	Chain operator+( const Chain & b) const {
	   Chain result( size()+b.size());
	   auto leftover = ctl::detail::chain_add( rbegin(), rend(), 
	   			            b.rbegin(), b.rend(),
	   			            result.rbegin(),
	   			            std::bind(Less(), 
						      std::placeholders::_2, 
						      std::placeholders::_1),
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
template< typename T, typename L>
Chain< T, L>& operator+( const typename Chain< T, L>::Term & b, 
				Chain< T, L> & a){ return a+b; }

} //namespace ctl

//\alpha*[chain] = [chain]*\alpha
template< typename T, typename L>
inline ctl::Chain< T, L>& operator*( const typename T::Coefficient & s, 
			   	ctl::Chain< T, L>& c){ return c*s; }
namespace ctl{
template< typename Stream, typename T, typename L>
Stream& operator<<( Stream& out, const Chain< T, L> & c){
	if( c.empty()){
		out << "0";
		return out;
	}
	for( auto i = c.rbegin(); i != c.rend(); ++i){
		out << *i; 
		if( (i+1) != c.rend()){ out << " " << ctl::oplus << " " ; }
	}
	return out;
}
} //namespace ctl

#endif //CTL_CHAIN_H
