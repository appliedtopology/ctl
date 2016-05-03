#ifndef CTL_RELATIVE_CELL_COMPLEX_H
#define CTL_RELATIVE_CELL_COMPLEX_H
#include <iostream>
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* Code release under BSD-3 License. See LICENSE.
*******************************************************************************/

//STL

//Boost
#include <boost/iterator/filter_iterator.hpp>

//CTL

namespace ctl{
template< typename Complex_> 
class Relative_cell_complex {
 public:
	typedef  Complex_ Complex;
	typedef  typename Complex::Cell Cell;
 private:
	typedef Relative_cell_complex< Complex> Self;
	typedef typename std::iterator_traits<typename Complex::const_iterator>::value_type vt;
 public:
	typedef std::function< bool(vt)> Predicate;
	typedef boost::filter_iterator<Predicate, typename Complex::iterator> iterator;
	typedef boost::filter_iterator<Predicate, typename Complex::const_iterator> const_iterator;
 public:
	Relative_cell_complex( Complex& X_, Complex& A_): X( X_), A( A_) {} 

        iterator begin() { return boost::make_filter_iterator( pred,  X.begin(), X.end()); }
        iterator   end() { return boost::make_filter_iterator( pred,  X.end(), X.end());   }
	
	const_iterator begin() const { return boost::make_filter_iterator( pred,  X.begin(), X.end()); }
        const_iterator   end() const { return boost::make_filter_iterator( pred,  X.end(), X.end());   }
	
	iterator find_cell( const typename Complex::Cell& s) const {
		if (A.find_cell(s) != A.end()){ return boost::make_filter_iterator( pred,  X.end(), X.end()); } 
		auto it = X.find_cell( s);
		return  make_iterator( it);
	}
	iterator make_iterator( typename Complex::iterator x) const { 
		return boost::make_filter_iterator( pred, x, X.end()); 
	}
	std::size_t size() { return X.size() - A.size(); }
	const Complex& super_complex() const  { return X; }
	const Complex&   sub_complex() const  { return A; }
	Complex& super_complex() { return X; }
	Complex&   sub_complex() { return A; }
	
	bool contains(const vt& x) const { return pred( x); }

	bool operator==( const Self & c) const { return (X == c.X) && (A == c.A); }
	bool operator!=( const Self&  c) const { return !(*this == c); }
 private:
  	Complex& X;
  	Complex& A;
  	Predicate pred = [&]( const vt& x){ return (A.find_cell( x.first) == A.end()); }; 
}; //class Relative_cell_complex

template< typename Stream, typename Complex>
Stream& operator<<( Stream& out, const Relative_cell_complex< Complex>& f){
	for ( auto & i : f){
		out << i.first << std::endl;
	}
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, const Relative_cell_complex< Complex>&& f){
	out << f;
	return out;
}

} //namespace ctl
#endif //CTL_RELATIVE_CELL_COMPLEX_H
