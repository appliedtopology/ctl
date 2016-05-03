#ifndef CTLIB_RELATIVE_COMPLEX_BOUNDARY_H
#define CTLIB_RELATIVE_COMPLEX_BOUNDARY_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************
* NOTES
* For now this is an input_iterator, cell boundary operations could be 
* bidirectional so we could easily make this a bidirectional iterator
* The ramifications of this could imply faster/slower algorithms, 
* need to check this.
*******************************************************************************
*******************************************************************************/
//STL
#include <iterator>     // std::iterator, std::input_iterator_tag
#include <ctl/io/io.hpp> //ctl::identity
#include <boost/iterator/filter_iterator.hpp> 

namespace ctl{
template< typename Relative_cell_complex, typename Complex_boundary>
class Relative_complex_boundary{
	typedef Relative_complex_boundary< Relative_cell_complex, Complex_boundary> Complex;
	public:
	typedef typename Complex_boundary::Coefficient Coefficient;
	typedef	typename Complex_boundary::Cell_boundary Cell_boundary;
	typedef typename Complex_boundary::size_type size_type;
	//Complex boundary terms are iterators
	typedef typename Complex_boundary::Term Underlying_term;
	typedef typename Complex_boundary::Term::template 
			rebind< typename Relative_cell_complex::iterator, Coefficient>::T Term;
 	
	typedef std::function<bool(const Underlying_term& t)> Predicate;

	Relative_complex_boundary( Relative_cell_complex& c_, Complex_boundary& bd_): 
		_complex( c_), bd( bd_) {}


	decltype(auto) begin( typename Relative_cell_complex::iterator c) const {
	  return boost::make_filter_iterator( predicate,  bd.begin( c.base()), bd.end( c.base())); 
	}

	decltype(auto) end( typename Relative_cell_complex::iterator c) const {
	  return boost::make_filter_iterator( predicate,  bd.end( c.base()), bd.end( c.base())); 
	}

	/**
	 * Not so efficient.
	 */
	size_type length( const typename Term::Cell & c) const { return std::distance( begin( c), end(c)); }
	decltype(auto) complex() { return _complex; }

	private:
	 Relative_cell_complex& _complex;
	 Complex_boundary& bd;
	 Predicate predicate = [&](const Underlying_term& t){ return _complex.contains(*(t.cell())); }; 
}; // class Relative_complex_boundary

} //namespace ctl

#endif //CTLIB_COMPLEX_BOUNDARY_H
