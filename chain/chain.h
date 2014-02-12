#ifndef CHAIN_H
#define CHAIN_H

#include "chain_add.h"

//exported functionality
namespace ct{

template< typename _Term, typename _Less>
class Chain {
 public:
 	typedef _Term Term;
	typedef _Less Less;
 private:
	typedef std::vector< Term> Vector;
	typedef Chain< Term, Less> Self;
 public:
	typedef Vector::iterator iterator;
	typedef Vector::const_iterator const_iterator;
 public:
	Chain(){}
	Chain( const Chain & c): _chain( c) {}
	Chain( const Chain && c): _chain( std::move( c)) {}
	Chain( const Term & t): _chain( 1, t) {}

	template< typename Iterator>
	Chain( Iterator begin, Iterator end): 
	_chain( _create_transform_iterator( begin), 
		_create_transform_iterator( end)) {};

	iterator       begin() 	     { return _chain.begin(); }
	const_iterator begin() const { return _chain.begin(); }
		
	iterator       end() 	     { return _chain.end(); }
	const_iterator end() const   { return _chain.end(); }

	std::size_t   size() const   { return _chain.size(); }	

	Chain& operator+( const Chain & b){ return add( a, b); }
	Chain& operator*( const Term::Coefficient & scalar){ 
	  for( auto i = begin(); i != end(); ++i){ (*i)*=scalar; }
	}
 private:
	Vector _chain;
}; //class Chain
} //namespace ct

template< typename _Term, typename _Less>
ctl::Chain< _Term, _Less>& operator*( const Term::



//non-exported functionality
namespace {

} //anonymous namespace

#endif //CHAIN_H
