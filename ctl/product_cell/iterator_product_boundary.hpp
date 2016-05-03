#ifndef ITERATOR_PRODUCT_BOUNDARY_H
#define ITERATOR_PRODUCT_BOUNDARY_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
//STL 
#include <cassert>

//CTL
#include <ctl/term/term.hpp>
#include <ctl/finite_field/finite_field.hpp>

namespace ctl {
//non-exported functionality
namespace detail {
//TODO: Concept_check Boundary1_ and Boundary2_ should have the same coefficient 
template< typename Term_, typename Boundary1_, typename Boundary2_, 
	   typename Coefficient_>
class const_it_product_boundary_iterator : 
	public std::iterator< std::bidirectional_iterator_tag,
			      Term_,
			      std::ptrdiff_t,
			      const Term_*,
			      const Term_>{
	private:
	  typedef Boundary1_ Boundary1; 
	  typedef Boundary2_ Boundary2; 
	  typedef Term_ Term;
	  typedef Coefficient_ Coefficient;
	  typedef typename Term::Cell Product;
	  typedef const_it_product_boundary_iterator< Term_, Boundary1_, 
					   Boundary2_, Coefficient_> Self;
	  typedef typename Boundary1::const_iterator const_iterator1;
	  typedef typename Boundary2::const_iterator const_iterator2;
	public:
	//default constructor
	const_it_product_boundary_iterator() {}

	//begin constructor
	const_it_product_boundary_iterator( const Boundary1& b1, const Boundary2& b2, 
				 const Product & p): 
		cellptr( &p),
		face1( b1.begin( p.first)),
		face2( b2.begin( p.second)),
		end1( b1.end( p.first)),
		end2( b2.end( p.second)),
		//even --> 1 and odd --> -1
		sign( 2*(p.first_cell().dimension()%2!=0)-1){ next_term(); }

	const_it_product_boundary_iterator( const Boundary1& b1, 
					 const Boundary2& b2, 
					 const Product & p, const bool b): 
		end1( b1.end( p.first)),
		end2( b2.end( p.second)),
		sign( 2*(p.first_cell().dimension()%2!=0)-1){ end_term(); }
	//copy constructor
	const_it_product_boundary_iterator( const Self & f): 
		cellptr( f.cellptr),
		face1( f.face1),
		face2( f.face2),
		end1( f.end1),
		end2( f.end2),
		sign( f.sign){}

	//move constructor
	const_it_product_boundary_iterator( Self && f): 
		cellptr(std::move( f.cellptr)),
		face1(  std::move( f.face1)),
		face2(  std::move( f.face2)),
		end1(   std::move( f.end1)),
		end2(   std::move( f.end2)),
		sign(   std::move( f.sign)){ f.cellptr = 0; }

	const_it_product_boundary_iterator& operator=(const Self & b){
		cellptr = b.cellptr;
		face1 = b.face1;
		face2 = b.face2;
		end1 = b.end1;
		end2 = b.end2;
		sign = b.sign;
		face = b.face;
		return *this;
	}

	const_it_product_boundary_iterator& operator=(Self && b){
		cellptr = std::move( b.cellptr);
		face1   = std::move( b.face1);
		face2   = std::move( b.face2);
		end1    = std::move( b.end1);
		end2    = std::move( b.end2);
		sign    = std::move( b.sign);
		face    = std::move( b.face);
		b.cellptr = 0;
		return *this;
	}
	
	Term& operator*() { return face; }
	Term* operator->() { return &face; }
	
	//equality
	bool operator==( const const_it_product_boundary_iterator & b) const { 
		return (b.face == face); 
	}
	bool operator!=( const const_it_product_boundary_iterator & b) const { 
		return (b.face != face); 
	}

	const_it_product_boundary_iterator& operator++(){
		next_term();
		return *this;	
	}

	const_it_product_boundary_iterator operator++( int){
	 	const_it_product_boundary_iterator tmp( *this); 
		++(*this); //now call previous operator
		return tmp;
	}
	protected: 
	void next_term(){
		//\partial(\sigma) \times \tau 
		if( face1 != end1){
			face.cell().first = face1->cell();
			face.cell().second = cellptr->second;
			face.coefficient( face1->coefficient()); 
			++face1;
		} else if ( face2 != end2){
			face.cell().first = cellptr->first;
			face.cell().second = face2->cell();
			face.coefficient( face2->coefficient());
			face*=sign;
			++face2;
		} else {
			end_term();
		}
	}
	void end_term(){
		face.cell().first  = end1->cell();
		face.cell().second = end2->cell();
		face.coefficient( 1);
	}
	private:
	 const Product* cellptr;
	 const_iterator1 face1;
	 const_iterator2 face2;
	 const_iterator1 end1;
	 const_iterator2 end2;
	 int sign=0;
	 Term face;
}; // END const_it_product_boundary_iterator
} //END private namespace

template< typename Product_, typename Boundary1_, typename Boundary2_, 
	  typename Coefficient = typename Boundary1_::Coefficient>
class Iterator_product_boundary {
public:
	typedef Product_ Product;
	typedef Product Cell;
	typedef Boundary1_ Boundary1;
	typedef Boundary2_ Boundary2;
	typedef typename Boundary1::Term::template 
			rebind< Product, Coefficient>::T Term;
	typedef detail::const_it_product_boundary_iterator< Term, Boundary1, 
					 Boundary2, Coefficient> const_iterator;
	//default constructor
	//none since we store references.

	//initialization constructor
	Iterator_product_boundary( const Boundary1 & b1, const Boundary2 & b2): 
	bd1( b1), bd2( b2) {};	

	//copy constructor	
	Iterator_product_boundary( const Iterator_product_boundary & from): 
	bd1( from.bd1), bd2( from.bd2) {};
	
	//copy constructor	
	Iterator_product_boundary( const Iterator_product_boundary && from): 
	bd1( std::move( from.bd1)), 
	bd2( std::move( from.bd2)) {};
	
	//assignment operator
	Iterator_product_boundary& operator=( const Iterator_product_boundary & from){
		bd1 = from.bd1;
		bd2 = from.bd2;
		return *this;
	}
	//move operator
	Iterator_product_boundary& operator=( Iterator_product_boundary && from){
		std::swap( bd1, from.bd1);
		std::swap( bd2, from.bd2);
		return *this;
	}
	
	//It only makes sense for const iterators
	const_iterator begin( const Product & p) const { 
		return const_iterator( bd1, bd2, p); 
	}
	const_iterator end( const Product & p) const   { 
		return const_iterator( bd1, bd2, p, true); 
	}
	std::size_t length( const Product & p) const { 
		return bd1.length( p.first) + bd2.length( p.second); 
	}
	const Boundary1& boundary1() const { return bd1; }
	const Boundary2& boundary2() const { return bd2; }
	private:
	const Boundary1& bd1;
	const Boundary2& bd2;
}; //Iterator_product_boundary

} // namespace ctl

#endif //ITERATOR_PRODUCT_BOUNDARY_H
