#ifndef SIMPLEX_BOUNDARY_H
#define SIMPLEX_BOUNDARY_H
#include "term.h"

//non-exported functionality
namespace {

template< typename Term_>
class const_boundary_iterator : 
	public std::iterator< std::input_iterator_tag,
			      Term_,
			      std::ptrdiff_t,
			      const Term_*,
			      const Term_>{
	private:
	  typedef const_boundary_iterator< Term_> Self;
	  typedef Term_ Term;
	  typedef typename Term::Cell Cell;
	  typedef typename Cell::value_type Vertex;
	public:
	  //default constructor
	  const_boundary_iterator(): cellptr( 0), pos( 0){}
	  const_boundary_iterator( const Cell & s): pos( 0){
	  	if( s.dimension()){
		    //begin by removing first vertex
		    cellptr = &s;
		    removed = s.vertices[ 0];
		    face.cell().vertices.resize( s.dimension());
		    face.set_coefficient( 1);
		    std::copy( s.begin()+1, s.end(), face.cell().begin());
		}else{ cellptr = 0; } //\partial(vertex) = 0
	  }
	  //copy constructor
	  const_boundary_iterator( const Self & from): cellptr( from.cellptr), 
	  face(from.face), pos( from.pos), removed( from.removed){}
	  const_boundary_iterator& operator=( const_boundary_iterator & from){
	  	cellptr = from.cellptr;
		pos = from.pos;
		face = from.face;
		removed = from.removed;
		return &(*this);
	 }
	 const Term& operator*() const {
	 	if( cellptr){ return face; }
		std::cerr << "Error! Attempting to Dereference the end" 
			  << std::flush << std::endl;
		std::exit( EXIT_FAILURE);
	 }
	 const Term* operator->() const { return &face; }
	 bool operator==( const_boundary_iterator & b) const { 
		return (b.cellptr == cellptr) && (b.pos == pos); 
	 }
	 bool operator!=( const_boundary_iterator & b) const { 
		return (b.cellptr != cellptr) || (b.pos != pos); 
	 }

	 const_boundary_iterator& operator++(){
		if( pos == face.cell().size()){
			cellptr = 0;
			pos = 0;
		}
		//return removed vertex, get rid of another one
		std::swap( face.cell().vertices[ pos++], removed);
		face.set_coefficient( -face.get_coefficient());
		return *this;	
	 }

	 const_boundary_iterator operator++( int){
	 	const_boundary_iterator tmp( *this); 
		++(*this); //now call previous operator
		return tmp;
	 }

	 private:
		const Cell* cellptr;
		typename Cell::size_type pos;
		Term face;
		Vertex removed;
			
	}; // END const_boundary_iterator
} //END private namespace

namespace ct { 
template< typename Simplex_, typename Coefficient, 
	  template<typename, typename> class Term_ >
class Simplex_boundary {
public:
	typedef Simplex_ Simplex;
	typedef Term_< Simplex, Coefficient> Term;
	typedef const_boundary_iterator< Term> const_iterator;
	//default constructor
	Simplex_boundary(){};	

	//It only makes sense for const iterators
	const_iterator begin( const Simplex & s){ return const_iterator( s); }
	const_iterator end( const Simplex & s){};
}; //Simplex_boundary

} // end namespace ct

#endif //SIMPLEX_BOUNDARY_H
