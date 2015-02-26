#ifndef CTLIB_FILTRATION_COMPLEX_BOUNDARY_H
#define CTLIB_FILTRATION_COMPLEX_BOUNDARY_H
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
#include <ctl/io/io.h> //ctl::identity

//non exported functionality 
namespace ctl{
namespace detail{
template< typename Filtration_, typename Term_, typename Cell_boundary_, typename Iterator_> 
class _const_filtration_boundary_iterator: 
public std::iterator< std::input_iterator_tag,
			      Term_,
			      std::ptrdiff_t,
			      const Term_*,
			      const Term_>{
  typedef Iterator_ Iterator;
  typedef _const_filtration_boundary_iterator< Filtration_, Term_, Cell_boundary_, Iterator_> Self;
  typedef Filtration_ Filtration;
  typedef typename Filtration::Complex Complex;
  typedef Term_ Term;
  typedef Cell_boundary_ Cell_boundary;

public:
	//default
	_const_filtration_boundary_iterator() {}
	
	//copy
	_const_filtration_boundary_iterator( const Self & i):
	filtration( i.filtration),
	next_term( i.next_term),
	//future_term( i.future_term),
	end_term( i.end_term),
	term( i.term) {}
	
	//move
	_const_filtration_boundary_iterator( Self && i):
	filtration ( std::move( i.filtration)),
	complex( std::move( i.complex)),
	next_term ( std::move( i.next_term)),
	//future_term ( std::move( i.future_term)),
	end_term ( std::move( i.end_term)),
	term( std::move( i.term)) { i.filtration = NULL; i.complex = NULL;}

	//begin constructor
	_const_filtration_boundary_iterator( Filtration& _f, 
				  Cell_boundary & _bd,
				  const Iterator& cell):
	  filtration( &_f),
	  complex( &(_f.complex())),
	  //begin ( _f.begin()),
	  next_term ( _bd.begin( (*cell)->first)), 
	  //future_term( next_term), 
	  end_term( _bd.end( (*cell)->first)){ 
		_next_term();
	}
        //end constructor
	_const_filtration_boundary_iterator( Filtration & _f): filtration( &_f), complex( &(_f.complex())){ 
	  	_end_term(); 
	}
	Self& operator=( const Self& from){
		filtration = from.filtration;
		complex = from.complex;
		//begin = from.begin;
		next_term = from.next_term;
		//future_term = from.future_term;
		end_term = from.end_term;
		term = from.term;
		return *this;
	}
	bool operator==( const Self & i) const { return term == i.term;}		
	bool operator!=( const Self & i) const { return term != i.term;}		
	Self& operator++(){ 
		_next_term(); 
		return *this;
	}
	Self operator++(int){
		Self tmp = *this;
		_next_term(); 
		return tmp;
	}
	Term& operator*(){ return term; }
	Term* operator->(){ return &term; }
protected:
  void _next_term(){
	if( next_term != end_term){
		const auto & cell = complex->find_cell( next_term->cell());
		term.cell() = cell->second.id();
		term.coefficient( next_term->coefficient());
		++next_term;
		return;
	}
	_end_term();
  }

  void _end_term(){ term.cell() = filtration->size(); }
  //we use a pointer since references are not default constructible
  Filtration* filtration;
  Complex* complex;
  //typename Filtration::iterator begin;
  //if we want to define operator-- 
  //typename Cell_boundary::const_iterator begin_term; 
  typename Cell_boundary::const_iterator next_term;
  //typename Cell_boundary::const_iterator future_term;
  typename Cell_boundary::const_iterator end_term;
  Term term;
}; //class _const_filtration_boundary_iterator
} //detail namespace
} //ctl namespace

//exported functionality
namespace ctl{

template< typename Filtration_,
	  typename Cell_boundary_ = typename Filtration_::Complex::Cell_boundary,
	  typename Iterator_ = typename Filtration_::iterator >
class Filtration_boundary{
	typedef Filtration_boundary< Filtration_, 
				     Cell_boundary_, Iterator_> Self;
	typedef typename Cell_boundary_::Term Cell_term;
	public:
	typedef typename Cell_term::Coefficient Coefficient;
	typedef Filtration_ Filtration;
	typedef typename Filtration::Complex Complex;
	typedef	Cell_boundary_ Cell_boundary;
	typedef Iterator_ Iterator;
	typedef typename Complex::size_type size_type;
	//Complex boundary terms are iterators
	typedef typename Cell_term::template 
			rebind< std::size_t, Coefficient>::T Term;
	typedef ctl::detail::_const_filtration_boundary_iterator< Filtration, 
							Term, 
							Cell_boundary, 
							Iterator> 
							const_iterator;
	//This is an abstraction used in persistence algorithm.
	//This way an iterator over a filtration can be handed to algorithm
	//or iterator to complex, or permutation iterator over filtration, etc.
	typedef ctl::identity Filtration_map;

	//copy constructor
	Filtration_boundary( Filtration_boundary & f): 
	_filtration( f._filtration) {};

	//move constructor, we don't care since we store references
	Filtration_boundary( Filtration_boundary && f): 
	_filtration( f._filtration) {};
	
	Filtration_boundary( Filtration & f): 
	_filtration( f) {};

	const_iterator begin( const Iterator & c) const {
		const std::size_t pos = std::distance( _filtration.begin(), c);
		(*c)->second.id( pos);
		return const_iterator( _filtration, 
				      _filtration.complex().cell_boundary(), c);
	}

	template< typename T>
	const_iterator end( const T & c) const { return const_iterator( _filtration); }

	size_type length( const typename Term::Cell & c) const {
		return _filtration.complex().cell_boundary().length( (*(_filtration+c))->first);
	}
	size_type length( const Iterator & c) const {
		return _filtration.complex().cell_boundary().length( (*(c))->first);
	}


	private:		
	Filtration & _filtration;
}; // class Filtration_boundary

} //namespace ctl

#endif //CTLIB_COMPLEX_BOUNDARY_H
