#ifndef CTLIB_CONCURRENT_FILTRATION_H
#define CTLIB_CONCURRENT_FILTRATION_H
/*******************************************************************************
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/

#include <tbb/parallel_sort.h>
#include <vector>
#include <tbb/concurrent_vector.h>

#include <ctl/graded_chain_complex/less.hpp>
#include <ctl/graded_chain_complex/filtration_iterator.hpp>

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
