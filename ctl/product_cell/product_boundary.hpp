#ifndef PRODUCT_BOUNDARY_H
#define PRODUCT_BOUNDARY_H
/*******************************************************************************
* -Academic Honesty-
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
//STL 
#include <cassert>

//CTL
#include <ctl/term/term.hpp>
#include <ctl/finite_field/finite_field.hpp>
#include <ctl/product_cell/product_cell.hpp>

//non-exported functionality
namespace ctl {
namespace detail {
//TODO: Concept_check Boundary1_ and Boundary2_ should have the same coefficient 
template< typename Term_, typename Boundary1_, typename Boundary2_, 
	   typename Coefficient_>
class const_product_boundary_iterator : 
	public std::iterator< std::input_iterator_tag,
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
	  typedef const_product_boundary_iterator< Term_, Boundary1_, 
					   Boundary2_, Coefficient_> Self;
	  typedef typename Boundary1::const_iterator const_iterator1;
	  typedef typename Boundary2::const_iterator const_iterator2;
	public:
	const_product_boundary_iterator() = default;
	//copy constructor
	const_product_boundary_iterator(const Self& s) = default;

	//move constructor
	const_product_boundary_iterator(Self&& s) = default;

	//begin constructor
	const_product_boundary_iterator( const Boundary1& b1, const Boundary2& b2, const Product & p): 
		cellptr( &p),
		face1( b1.begin( p.first)),
		face2( b2.begin( p.second)),
		end1( b1.end( p.first)),
		end2( b2.end( p.second)),
		//even --> 1 and odd --> -1
		sign( 2*(p.first_cell().dimension()%2!=0)-1){ next_term(); }

	//end constructor
	const_product_boundary_iterator( const Boundary1& b1, 
					 const Boundary2& b2, 
					 const Product & p, const bool b): 
		face1(), face2(),
		end1( b1.end( p.first)),
		end2( b2.end( p.second)),
		sign( 0){ end_term(); }
	
	Term& operator*() { 
		return face;
	}
	Term* operator->() { 
		return &face; 
	}
  	Self& operator=(const Self&) = default;	
  	Self& operator=(Self&&) = default;	
	//equality
	bool operator==( const const_product_boundary_iterator & b) const { 
		return (sign == b.sign) && (b.face1 == face1) && (b.face2 == face2);
	}
	bool operator!=( const const_product_boundary_iterator & b) const { 
		return !(*this == b);
	}

	const_product_boundary_iterator& operator++(){
		next_term();
		return *this;	
	}

	const_product_boundary_iterator operator++( int){
	 	const_product_boundary_iterator tmp( *this); 
		++(*this); //now call previous operator
		std::cout << &(tmp->cell().first_cell()) << " " << &(tmp->cell().second_cell()) << std::endl;
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
			face *= sign;
			++face2;
		} else {
			end_term();
		}
	}
	void end_term(){
		cellptr = 0;
		face1 = end1;
		face2 = end2;
		face.cell().first.clear();
		face.cell().second.clear();
		sign = 0;
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
}; // END const_product_boundary_iterator
} //END private namespace

template< typename Boundary1_, typename Boundary2_, 
	  typename Coefficient = typename Boundary1_::Coefficient>
class Product_boundary {
public:
	typedef Boundary1_ Boundary1;
	typedef Boundary2_ Boundary2;
	typedef ctl::Product_cell< typename Boundary1::Cell, 
				   typename Boundary2::Cell> Product;
	typedef typename Product::Hash Hash;

	typedef Product Cell;
	typedef typename Boundary1::Term::template 
			rebind< Product, Coefficient>::T Term;
	typedef detail::const_product_boundary_iterator< Term, Boundary1, Boundary2, Coefficient> const_iterator;
	Product_boundary() : bd1(), bd2() {}
	//initialization constructor
	Product_boundary( const Boundary1 & b1, const Boundary2 & b2): 
	bd1( b1), bd2( b2) {}

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
	const Boundary1 bd1;
	const Boundary2 bd2;
}; //Product_boundary

} // namespace ctl

#endif //PRODUCT_BOUNDARY_H
