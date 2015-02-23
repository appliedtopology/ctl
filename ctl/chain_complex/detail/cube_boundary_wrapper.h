#ifndef CTL_CUBICAL_BOUNDARY_WRAPPER_H
#define CTL_CUBICAL_BOUNDARY_WRAPPER_H
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
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program in a file entitled COPYING; if not, write to the 
* Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************************
*******************************************************************************
* NOTES
*
*
*******************************************************************************/
//STL
//#include < .. >

//CTL
//#include <ctl/.../...h>

//non-exported functionality
namespace ctl {
namespace detail {
template< typename Complex_, typename Term_>
class const_cube_boundary_wrapper_iterator:
    public std::iterator< std::forward_iterator_tag, 
    		      Term_,
    		      std::ptrdiff_t,
    		      const Term_*,
    		      const Term_>{
    
    private:
      typedef const_cube_boundary_wrapper_iterator< Complex_, Term_> Self;
      typedef Term_ Term;
      typedef Complex_ Complex;
      typedef typename Term::Cell Cell;

     public:
     const_cube_boundary_wrapper_iterator(): 
       c( nullptr), cellptr( nullptr), 
       face(), boundary_pos( 0), bit_index( 0), dim( 0) {}

     const_cube_boundary_wrapper_iterator( const Self & f): 
       c( f.c), cellptr( f.cellptr), face( f.face), 
       boundary_pos( f.boundary_pos), 
	bit_index( f.bit_index), 
       dim( f.dim) {}

     const_cube_boundary_wrapper_iterator( const Self && f): 
      c( std::move( f.c)), cellptr( f.cellptr), 
      face( std::move( f.face)),
      boundary_pos( std::move( f.boundary_pos)), 
      bit_index( std::move( f.bit_index)),
      dim( std::move( f.dim)){}

     
    /**
    * @brief begin constructor
    *
    * @param c_
    * @param cell):
    */
     const_cube_boundary_wrapper_iterator( const Complex& c_, const Cell& cell): 
     c( &c_), cellptr( &cell), face(), boundary_pos( 0), 
	bit_index( 0), 
     dim( dimension( cell)){
       if( dim){
	//since dim > 0 this implies c_ must be nonzero
	//therefore this is correct.
	bit_index = __builtin_ctz( cell);
       	c = &c_;
	cellptr = &cell;
	//initially we remove the first set bit
	face.cell() = cell ^ (1 << bit_index); 
	face.coefficient( 1);
	++boundary_pos;
      } else{ cellptr = nullptr;  c = nullptr; }
     }

     bool operator==(const Self&b) const{ 
	return (b.cellptr == cellptr) && (b.boundary_pos == boundary_pos); 
     }

     bool operator!=(const Self&b) const{ 
	return (b.cellptr != cellptr) || (b.boundary_pos != boundary_pos); 
     }

     Self& operator=( const Self&b){
       c = b.c;
       cellptr = b.cellptr;
       boundary_pos = b.boundary_pos;
       dim = b.dim;
       bit_index = b.bit_index;
       face = b.face; 
       return *this;
     }

     Self& operator=( Self&&b){
       c = std::move( b.c);
       cellptr = std::move( b.cellptr);
       b.c = nullptr;
       b.cellptr = nullptr;
       boundary_pos = std::move( b.boundary_pos);
       bit_index = std::move( b.bit_index);
       dim = std::move( b.dim);
       face = std::move( b.face); 
       return *this;
     }
     Term& operator*() {
        assert( c != nullptr); 
     	return face; 
     }
     const Term* operator->() const { return &face; }

     Self& operator++(){
      if( boundary_pos >= 2*dim){ 
      	cellptr = nullptr;
      	boundary_pos = 0;
      	return *this;
      }
      //set previously flipped bit
      //flip the coefficient
      face.coefficient( -1*face.coefficient());
      std::size_t shift=2*c->offset( bit_index);
      if( boundary_pos%2 == 0){
        face.cell() |= (1 << bit_index);
      	face.cell() -= (shift << c->dimension()); 
       //all but the lowest bits are set
        std::size_t mask = (1 << (c->dimension()))-1;
	bit_index = find_next_set_bit( mask&face.cell(), bit_index);
        face.cell() ^= (1 << bit_index); //unset new bit
       }else{
       face.cell() += (shift << c->dimension());
       }
      //if we are in the second half of boundary computation
      //the vertex id which owns a cell changes.
      //we add an offset, given by the index of the bit we just turned
      //off.
      ++boundary_pos;
      return *this;	
     }

     std::size_t find_next_set_bit( const Cell & c, char bit_index){
	std::size_t wtf= ((c >> (bit_index+1)) << (bit_index+1));
      return __builtin_ctz( wtf);
     }

     Self operator++( int){
      	Self tmp( *this); 
     	++(*this); //now call previous operator
     	return tmp;
     }

   private:
     std::size_t dimension( const std::size_t c_) const{ 
	std::size_t mask = (1 << (c->dimension() +1))-1;
     	return __builtin_popcount( c_ & mask);
     }

     const Complex* c;
     const Cell* cellptr;
     Term face; 
     std::size_t boundary_pos;
     //purposively a bit bigger than necessary.
     //this really only needs to be d bits..
     short bit_index;
     std::size_t dim;
}; //end const_cube_boundary_wrapper_iterator

template< typename Cell_boundary_, typename Complex_>
class Cube_boundary_wrapper: Cell_boundary_{
    //! Underlying cube term type
    typedef typename Cell_boundary_::Term _Cell_term;
    typedef Complex_ Complex; 
    /*! A term of the boundary wrapper.
     * The begin() takes a coordinate as input
     * and returns an iterator over coordinates 
     */
   public:
    typedef typename Cell_boundary_::Coefficient Coefficient;
    typedef typename _Cell_term::template 
			rebind< std::size_t, Coefficient>::T Term;
    typedef std::size_t Cell;
    //! const_iterator type
    typedef detail::const_cube_boundary_wrapper_iterator< Complex, Term> 
		const_iterator;
    	
    /**
    * @brief Default constructor
    */
    Cube_boundary_wrapper( const Complex & c_): Cell_boundary_(), c( &c_){};	
    
    Cube_boundary_wrapper( const Complex & c_, Cell_boundary_ & b): Cell_boundary_( b), c( &c_){};	
    /**
    * @brief Equality assignment operator 
    * @param Cube_boundary_wrapper& from
    * @return reference to this.
    */
    Cube_boundary_wrapper& operator=( const Cube_boundary_wrapper& from){ 
	Cell_boundary_::operator=( from);
	c = from.c;
	return *this; 
    }

    Cube_boundary_wrapper& operator=( Cube_boundary_wrapper&& from){ 
	Cell_boundary_::operator=( std::move( from));
	//c = std::move( from.c);
	from.c = nullptr;
	return *this; 
    }
 
    
    /**
    * @brief boundary begin
    * Returns an iterator to the first element of the boundary
    * @param const Cube & s
    * @return const_iterator
    */
    const_iterator begin( const std::size_t s) const { 
	return const_iterator( *c, s); 
   }
    
    /**
    * @brief  boundary end
    * Returns an iterator to the past the end element of the boundary
    * @param const Cube & s
    * @return const_iterator 
    */
    const_iterator end( const std::size_t s) const   { 
	return const_iterator(); 
    }
    
    /**
    * @brief length of a boundary  
    * @param const Coordinate & s
    * @return returns the number of elements in the boundary.
    */
    std::size_t length( const std::size_t s) const { 
    	return 2*dimension( s);
    }
    /**
    * @brief dimension of a cube represented by coordinates  
    * @param const Coordinate & s
    * @return returns the number of elements in the boundary.
    */

    std::size_t dimension( const std::size_t & c_) const{ 
     	std::size_t d = 0;
     	for( auto i = 0; i < c->dimension(); ++i){ d+= (c_&(i+1)); }
     	return d;
    }

    private:
    const Complex* c;
}; //end class Cube_boundary_wrapper

} // end namespace detail
} //ctl namespace


#endif //CTL_CUBE_BOUNDARY_WRAPPER_H
