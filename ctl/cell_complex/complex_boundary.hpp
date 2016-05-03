#ifndef CTL_COMPLEX_BOUNDARY_H
#define CTL_COMPLEX_BOUNDARY_H
/*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
//STL
#include <iterator>     // std::iterator, std::input_iterator_tag

//non exported functionality 
namespace ctl{
namespace detail{
template< typename Complex_, typename Term_, typename Cell_boundary_> 
class _const_boundary_iterator: 
public std::iterator< std::input_iterator_tag,
			      Term_,
			      std::ptrdiff_t,
			      const Term_*,
			      const Term_>{
  typedef _const_boundary_iterator< Complex_, Term_, Cell_boundary_> Self;
  typedef Complex_ Complex;
  typedef Term_ Term;
  typedef Cell_boundary_ Cell_boundary;

public:
	//default
	_const_boundary_iterator() {}
	
	//copy
	_const_boundary_iterator( const Self & i):
	complex ( i.complex),
	next_term( i.next_term),
	//future_term( i.future_term),
	end_term( i.end_term),
	term( i.term) {}
	
	//move
	_const_boundary_iterator( Self && i):
	complex ( std::move( i.complex)),
	next_term ( std::move( i.next_term)),
	end_term ( std::move( i.end_term)),
	term( std::move( i.term)) { i.complex = NULL; }

	//begin constructor
	_const_boundary_iterator( Complex& _complex, 
				  Cell_boundary & _bd,
				  const typename Complex::Cell& cell):
	  complex( &_complex), 
	  next_term ( _bd.begin( cell)), 
	  end_term( _bd.end( cell)){ 
		_next_term();
	}
        //end constructor
	_const_boundary_iterator( Complex & _complex): complex( &_complex){ 
	  	_end_term(); 
	}
	Self& operator=( const Self& from){
		complex = from.complex;
		next_term = from.next_term;
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
		term.cell() = complex->find_cell( next_term->cell());
		term.coefficient( next_term->coefficient());
		++next_term;
		return;
	}
	_end_term();
  }

  void _end_term(){ term.cell() = complex->end(); }

  Complex* complex;
  typename Cell_boundary::const_iterator next_term;
  typename Cell_boundary::const_iterator end_term;
  Term term;
}; //class _const_boundary_iterator
} //detail namespace
} //ctl namespace

//exported functionality
namespace ctl{

template< typename Complex_,
	  typename Cell_boundary_ = typename Complex_::Cell_boundary,
	  typename Iterator_ = typename Complex_::iterator >
class Complex_boundary {
	typedef Complex_boundary< Complex_> Self;
	typedef typename Cell_boundary_::Term Cell_term;
	public:
	//export types 
	typedef typename Cell_term::Coefficient Coefficient;
	typedef	Complex_ Complex; 
	typedef	Cell_boundary_ Cell_boundary;
	typedef Iterator_ Iterator;
	typedef typename Complex::size_type size_type;
	//Complex boundary terms are iterators
	typedef typename Cell_term::template 
			rebind< Iterator, Coefficient>::T Term;
	typedef ctl::detail::_const_boundary_iterator< Complex, 
							Term, 
							Cell_boundary> 
							const_iterator;
	//copy constructor
	Complex_boundary( Complex_boundary & f): _complex( f._complex) {};
	//move constructor, we don't care since we store references
	Complex_boundary( Complex_boundary && f): _complex( f._complex) {};

	Complex_boundary( Complex & complex): _complex( complex) {};
	
	const_iterator begin( const typename Term::Cell & c) const {
	  return const_iterator( _complex, _complex.cell_boundary(), c->first);
	}
	const_iterator end( const typename Term::Cell & c) const {
	  return const_iterator( _complex);
	}
	size_type length( const typename Term::Cell & c) const {
	  return _complex.cell_boundary().length( c->first);
	}
		
	private:		
	Complex & _complex;
}; // class Complex_boundary

} //namespace ctl

#endif //CTLIB_COMPLEX_BOUNDARY_H
