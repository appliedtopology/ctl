#ifndef CHAIN_H
#define CHAIN_H

#include "chain_add.h"

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

template< typename Term, typename Iterator>
inline _transform_iterator< Iterator> _create_transform_iterator ( Iterator i){
 	return _transform_iterator< Iterator>( i);
}

} //anonymous namespace


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
	typedef Term::coeff_tag coeff_tag; 
public:
	Chain(){}
	Chain( const Chain & c): _chain( c) {}
	Chain( const Chain && c): _chain( std::move( c)) {}
	Chain( const Term & t): _chain( 1, t) {}
	//_transform_iterator fakes a level of indirection.
	template< typename Iterator>
	Chain( Iterator begin, Iterator end): 
	_chain( _create_transform_iterator( begin), 
		_create_transform_iterator( end)) {};
	
	iterator       begin() 	     { return _chain.begin(); }
	const_iterator begin() const { return _chain.begin(); }
		
	iterator       end() 	     { return _chain.end(); }
	const_iterator end() const   { return _chain.end(); }

	std::size_t   size() const   { return _chain.size(); }	

	//coeff_tag is just because C++11 doesn't allow
	//template partial specialization of *functions*
	Chain& operator+( const Chain & b) { 
		return chain_add( _chain, b, coeff_tag);
	}
	Chain& operator+( const Term & b){
		return chain_add( _chain, b, coeff_tag);
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
inline ctl::Chain< T, L>& operator*( const typename T::Coefficient & s, 
			      ctl::Chain< T, L>& c){ return c*s; }


//non-exported functionality
namespace {

} //anonymous namespace

#endif //CHAIN_H
