#ifndef CUBE_BOUNDARY_H
#define CUBE_BOUNDARY_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
* Released under BSD-3 License. See LICENSE
*******************************************************************************/
#include <ctl/term/term.hpp>
#include <ctl/finite_field/finite_field.hpp>
#include <cassert>
#include <ctl/cube/cube.hpp>

namespace ctl {
namespace detail {
template< typename Term_>
class const_cube_boundary_iterator : 
  public std::iterator< std::bidirectional_iterator_tag,
  		      Term_,
  		      std::ptrdiff_t,
  		      const Term_*,
  		      const Term_>{
  private:
    typedef const_cube_boundary_iterator< Term_> Self;
    typedef Term_ Term;
    typedef typename Term::Cell Cell;
    typedef typename Cell::value_type Interval;
    typedef typename Interval::first_type vertex;
  public:
    //default constructor
  const_cube_boundary_iterator(): cellptr( 0), pos( 0), face(){}
  const_cube_boundary_iterator( const Cell & c): pos( 0), face( c, 1){ 
   if( c.dimension()){
     cellptr = &c;
     face.cell().remove( std::remove_if( face.cell().begin(), face.cell().end(), 
		      []( const Interval & t){ return (t.first == t.second); }),
		       face.cell().end());
     if( face.cell().dimension()){
        face.coefficient( 1);
        removed = face.cell().begin()->first;
	face.cell().begin()->first = face.cell().begin()->second;
     } else{ cellptr = 0; } // a product of vertices has zero boundary?
   }else{ cellptr = 0; } //\partial(vertex) = 0
  }
  //copy constructor
  const_cube_boundary_iterator( const Self & from): cellptr( from.cellptr), 
  pos( from.pos), face(from.face), removed( from.removed){}
  //move constructor
  const_cube_boundary_iterator( Self && from): 
  cellptr( std::move(from.cellptr)), 
  pos( std::move(from.pos)), 
  face( std::move(from.face)), 
  removed( std::move(from.removed)){ from.cellptr = 0; }	
  //equality
  const_cube_boundary_iterator& operator==( const Self & b) const { 
  	return (b.cellptr == cellptr) && (b.pos == pos); 
  } 
  const_cube_boundary_iterator& operator=(const Self & b){
  	cellptr = b.cellptr;
  	remove = b.removed;
  	face = b.face;
  	pos = b.pos;
  	return *this;
  }
  const_cube_boundary_iterator& operator=(Self && b){
  	cellptr = std::move(b.cellptr);
  	remove  = std::move(b.removed);
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
  bool operator!=( const const_cube_boundary_iterator & b) const { 
  	return (b.cellptr != cellptr) || (b.pos != pos); 
  }
  
  const_cube_boundary_iterator& operator++(){
    if( pos == face.cell().size()){
    	cellptr = 0;
    	pos = 0;
    	return *this;
    }
    //return removed vertex, get rid of another one
    auto& s = face.cell().begin()[ pos].second;
    auto& f = face.cell().begin()[ pos].first;
  
    if(  s > removed){
       std::swap( f, removed);
       s = f;
    }else {
       s = removed;
       pos++;
       if( pos != face.cell().size()){
 	removed = face.cell().begin()[ pos].first;
        face.cell().begin()[ pos].first = face.cell().begin()[ pos].second; 
       }
    }
    face.coefficient( -1*face.coefficient());
    return *this;	
  }
  
  const_cube_boundary_iterator& operator--(){
    if(pos == 0){
    	cellptr = 0;
    	return *this;
    }
   //return removed vertex, get rid of another one
    auto& s = face.cell().intervals[ pos].second;
    auto& f = face.cell().intervals[ pos].first;
    if(  s > removed){
       std::swap( f, removed);
       s = f;
    }else {
       s = removed;
       --pos;
       if( pos != 0){
       removed = face.cell().intervals[ pos].first;
       face.cell().intervals[ pos].first = face.cell().intervals[ pos].second; 
       }
    }
   face.coefficient( -1*face.coefficient());
   return *this;	
  }
  
  const_cube_boundary_iterator operator++( int){
   	const_cube_boundary_iterator tmp( *this); 
  	++(*this); //now call previous operator
  	return tmp;
  }
  const_cube_boundary_iterator operator--( int){
   	const_cube_boundary_iterator tmp( *this); 
  	--(*this); //now call previous operator
  	return tmp;
  }
   private:
  	const Cell* cellptr;
  	typename Cell::size_t pos;
  	Term face;
  	vertex removed;
}; // END const_cube_boundary_iterator
} //END detail namespace
} //end ctl namespace

/*! \namespace ctl
Namespace where all library functionality resides
*/
namespace ctl { 

/**
* @brief Cube boundary operator
* Defines a boundary operator on a cube.
* The ctl::Complex_boundary is the generic linear extension of this operator
* @tparam Cube_
* @tparam Coefficient_
*/
//template< typename Coefficient_>
class Cube_boundary {
    typedef int Coefficient_;
public:
    //! Underlying cube type
    typedef ctl::Cube Cube;
    //! Underyling cell type
    typedef ctl::Cube Cell;
    //! Underlying Coefficient type
    typedef Coefficient_ Coefficient;
    //! A term of the boundary operator
    typedef ctl::Term< Cube, Coefficient> Term;
    //! const_iterator type
    typedef detail::const_cube_boundary_iterator< Term> const_iterator;
    	
    /**
    * @brief Default constructor
    */
    Cube_boundary(){};	
    
    /**
    * @brief Equality assignment operator 
    * Does nothing.
    * @param Cube_boundary& from
    * @return reference to this.
    */
    Cube_boundary& operator=( const Cube_boundary& from){ return *this; }
    
    /**
    * @brief boundary begin
    * Returns an iterator to the first element of the boundary
    * @param const Cube & s
    * @return const_iterator
    */
    const_iterator begin( const Cube & s) const { return const_iterator( s); }
    
    /**
    * @brief  boundary end
    * Returns an iterator to the past the end element of the boundary
    * @param const Cube & s
    * @return const_iterator 
    */
    const_iterator end( const Cube & s) const   { return const_iterator(); }
    
    /**
    * @brief length of a boundary  
    * @param const Cube & s
    * @return returns the number of elements in the boundary.
    */
    std::size_t length( const Cube & s) const { 
    	return (s.size()>1)? 2*s.dimension():0;
    }
}; //Cube_boundary

} // namespace ctl

#endif //CUBE_BOUNDARY_H
