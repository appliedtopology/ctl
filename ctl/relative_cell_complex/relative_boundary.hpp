#ifndef CTLIB_FILTRATION_COMPLEX_BOUNDARY_H
#define CTLIB_FILTRATION_COMPLEX_BOUNDARY_H
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

namespace ctl{
template< typename Relative_cell_complex, typename Complex_boundary>
class Relative_boundary{
	typedef Relative_boundary< Relative_cell_complex> Complex;
	typedef typename Complex_boundary::Cell_term Cell_term;
	public:
	typedef typename Complex_boundary::Coefficient Coefficient;
	typedef	Complex_boundary::Cell_boundary Cell_boundary;
	typedef typename Complex_boundary::size_type size_type;
	//Complex boundary terms are iterators
	typedef typename Complex_boundary::Term Term;

	//copy constructor
	Relative_boundary( Relative_boundary & f): 
	_filtration( f._filtration) {};

	//move constructor, we don't care since we store references
	Relative_boundary( Relative_boundary && f): 
	_filtration( f._filtration) {};
	
	const_iterator begin( const typename Term::Cell & c) const {}

	const_iterator begin( const Iterator & c) const {}

	template< typename T>
	const_iterator end( const T & c) const { return const_iterator( _filtration); }

	size_type length( const typename Term::Cell & c) const {}
	size_type length( const Iterator& c) const {}

	private:		
}; // class Relative_boundary

} //namespace ctl

#endif //CTLIB_COMPLEX_BOUNDARY_H
