#ifndef SIMPLEX_BOUNDARY_H
#define SIMPLEX_BOUNDARY_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************/
//CLIB
#include <cassert>

//CTL
#include <ctl/abstract_simplex/abstract_simplex.hpp>

#include <ctl/term/term.hpp>
#include <ctl/finite_field/finite_field.hpp>
namespace ctl {
namespace detail {
template< typename Term_>
class const_simplex_boundary_iterator : 
	public std::iterator< std::input_iterator_tag,
			      Term_,
			      std::ptrdiff_t,
			      const Term_*,
			      const Term_>{
	private:
	  typedef const_simplex_boundary_iterator< Term_> Self;
	  typedef Term_ Term;
	  typedef typename Term::Cell Cell;
	  typedef typename Cell::value_type Vertex;
	public:
	  //default constructor
	const_simplex_boundary_iterator(): cellptr( 0), pos( 0), face(){}
	const_simplex_boundary_iterator( const Cell & s): pos( 0), face(){ 
		if( s.dimension()){
		    //begin by removing first vertex
		    cellptr = &s;
		    removed = s.front();
		    face.coefficient( 1);
		    face.cell().reserve( s.dimension());
		    face.cell().insert( s.begin()+1, s.end());  
		}else{ cellptr = 0; } //\partial(vertex) = 0
	}
	//copy constructor
	const_simplex_boundary_iterator( const Self & from): cellptr( from.cellptr), 
	pos( from.pos), face(from.face), removed( from.removed){}
	//move constructor
	const_simplex_boundary_iterator( Self && from): 
	cellptr( std::move(from.cellptr)), 
	pos( std::move(from.pos)), 
	face( std::move(from.face)), 
	removed( std::move(from.removed)){ from.cellptr = 0; }	
	//equality
	bool operator==( const Self & b) const { 
		return (b.cellptr == cellptr) && (b.pos == pos); 
	} 
	const_simplex_boundary_iterator& operator=(const Self & b){
		cellptr = b.cellptr;
		removed = b.removed;
		face = b.face;
		pos = b.pos;
		return *this;
	}
	const_simplex_boundary_iterator& operator=(Self && b){
		cellptr = std::move(b.cellptr);
		removed  = std::move(b.removed);
		face    = std::move(b.face);
		pos     = std::move(b.pos);
		return *this;
	}
	//We modify normal behavior for performance reasons.
	//When we do prefetching we wish to prefetch the next element
	//in the boundary and not experience a branch to check for the end
	//therefore we use assert here so that in debug mode it will complain
	//if your software has bugs, but in release mode we assume only
	//correct usage of this software so we will "silently fail".
	//this way the prefetch will work, and this bound checking wont
	//happen on any dereference, saving a branch. 
	Term& operator*() {
		assert( cellptr != 0); 
		return face; 
	}

	const Term* operator->() const { return &face; }
	bool operator!=( const const_simplex_boundary_iterator & b) const { 
		return !(*this == b);
	}

	const_simplex_boundary_iterator& operator++(){
		if( pos == face.cell().size()){
			cellptr = 0;
			pos = 0;
			return *this;
		}
		//return removed vertex, get rid of another one
		std::swap( *(face.cell().begin()+(pos++)), removed);
		face *= -1; 
		return *this;	
	}

	const_simplex_boundary_iterator& operator--(){
		if(pos == 0){
			cellptr = 0;
			return *this;
		}
		//return removed vertex, get rid of another one
		std::swap( *(face.cell().begin() + (--pos)), removed);
		face.coefficient( -1*face.coefficient());
		return *this;	
	}

	const_simplex_boundary_iterator operator++( int){
	 	const_simplex_boundary_iterator tmp( *this); 
		++(*this); //now call previous operator
		return tmp;
	}

	const_simplex_boundary_iterator operator--( int){
	 	const_simplex_boundary_iterator tmp( *this); 
		--(*this); //now call previous operator
		return tmp;
	}
	 private:

	const Cell* cellptr;
	typename Cell::size_t pos;
	Term face;
	Vertex removed;
}; // END const_simplex_boundary_iterator
} //END detail namespace
} //end ctl namespace

/*! \namespace ctl
Namespace where all library functionality resides
*/
namespace ctl { 

/**
* @brief Simplex boundary operator
* Defines a boundary operator on a simplex.
* The ctl::Complex_boundary is the generic linear extension of this operator
* @tparam Simplex_
* @tparam Coefficient_
*/
//template< typename Coefficient_>
class Simplex_boundary {
	typedef ctl::Finite_field<2> Coefficient_;
	typedef ctl::Abstract_simplex Simplex_;
	typedef Simplex_boundary Self;
public:
	//! Underlying simplex type
	typedef Simplex_ Simplex;
	//! Underlying cell type
	typedef Simplex_ Cell;
	//! Underlying Coefficient type
	typedef Coefficient_ Coefficient;
	//! A term of the boundary operator
	typedef ctl::Term< Simplex, Coefficient> Term;
	//! const_iterator type
	typedef detail::const_simplex_boundary_iterator< Term> const_iterator;
		
	/**
	* @brief Default constructor
	*/
	Simplex_boundary(){};	

	/**
	* @brief Equality assignment operator 
	* Does nothing.
	* @param Simplex_boundary& from
	* @return reference to this.
	*/
	Simplex_boundary& operator=( const Simplex_boundary& from){ return *this; }
	
	/**
	* @brief boundary begin
	* Returns an iterator to the first element of the boundary
	* @param const Simplex & s
	* @return const_iterator
	*/
	const_iterator begin( const Simplex & s) const { return const_iterator( s); }

	/**
	* @brief  boundary end
	* Returns an iterator to the past the end element of the boundary
	* @param const Simplex & s
	* @return const_iterator 
	*/
	const_iterator end( const Simplex & s) const   { return const_iterator(); }
	
	/**
	* @brief length of a boundary  
	* @param const Simplex & s
	* @return returns the number of elements in the boundary.
	*/
	std::size_t length( const Simplex & s) const { 
		return (s.size()>1)? s.size():0;
	}
}; //Simplex_boundary

} // namespace ctl

#endif //SIMPLEX_BOUNDARY_H
