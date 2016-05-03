	Filtration & _filtration;
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
template< typename Complex> 
class Relative_cell_complex {
 public:
	typedef  Complex_ Complex;
	typedef  decltype(&Relative_cell_complex::begin) iterator;
	typedef  decltype(&Relative_cell_complex::rbegin) reverse_iterator;
	typedef  iterator const_iterator;
	typedef  reverse_iterator const_reverse_iterator;
 private:
	typedef std::iterator_traits<iterator>::value_type vt;
 public:
	Relative_cell_complex( const Complex& X_, const Complex& A_) X( X_), A( A_) {} 
	Relative_cell_complex( const Relative_cell_complex & f): filtration_( f), complex_( f.complex_) {}
	Relative_cell_complex( const Relative_cell_complex && f): filtration_( std::move( f)), complex_( f.complex_) {}	

        decltype(auto) begin() const { return boost::make_filter_iterator( pred,  X.begin(), X.end()); }
        decltype(auto)   end() const { return boost::make_filter_iterator( pred,  X.end(), X.end());   }
	
	decltype(auto) rbegin() const { return boost::make_filter_iterator( pred,  X.rbegin(), X.rend()); }
        decltype(auto)   rend() const { return boost::make_filter_iterator( pred,  X.rend(), X.rend());   }
	iterator find_cell( const Cell& s){
		if (A.find_cell(s) != A.end()){ return end(); } 
		auto it = X.find_cell( s);
		return  boost::make_filter_iterator( pred, it, X.end()); 
	}
	std::size_t size() { return X.size() - A.size(); }
	const Complex& super_complex() const  { return X; }
	const Complex&   sub_complex() const  { return A; }

 private:
  const Complex& X;
  const Complex& A;
  const auto pred = [&]( const vt& x){ return (A.find_cell( x.first) != A.end()); } 
}; //class Relative_cell_complex

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, const Relative_cell_complex< Complex, L>& f){
	for ( auto & i : f){
		out << i.first << std::endl;
	}
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, const Relative_cell_complex< Complex, L>&& f){
	out << f;
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, 
		    const typename Relative_cell_complex< Complex, L>::const_reverse_iterator f){
	out << f->first;
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, 
		    const typename Relative_cell_complex< Complex, L>::reverse_iterator f){
	out << f->first;
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, 
		    const typename Relative_cell_complex< Complex, L>::iterator f){
	out << f->first;
	return out;
}

template< typename Stream, typename Complex, typename L>
Stream& operator<<( Stream& out, 
		    const typename Relative_cell_complex< Complex, L>::const_iterator f){
	out << f->first;
	return out;
}

} //namespace ctl
#endif //CTL_RELATIVE_CELL_COMPLEX_H
