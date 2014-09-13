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
namespace detail{

template< typename Term_>
class const_cube_boundary_wrapper_iterator:
    public std::iterator< std::bidirectional_iterator_tag,
    		      Term_,
    		      std::ptrdiff_t,
    		      const Term_*,
    		      const Term_>{
    
    private:
      typedef const_cube_boundary_wrapper_iterator< Term_> Self;
      typedef Term_ Term;
      typedef typename Term::Cell Cell;

     public:
     const_cube_boundary_wrapper_iterator(): 
       c( nullptr), cellptr( nullptr), 
       boundary_pos( 0), bit_index( 0), dim( 0), face() {}

     const_cube_boundary_wrapper_iterator( const Self & f): 
       c( f.c), cellptr( f.cellptr), 
       boundary_pos( f.boundary_pos), bit_index( f.bit_index), 
       dim( f.dim), face( f.face) {}

     const_cube_boundary_wrapper_iterator( const Self && f): 
      c( std::move( f.c)), cellptr( f.cellptr), 
      boundary_pos( std::move( f.boundary_pos)), 
      bit_index( f.bit_index),
      dim( std::move( f.dim)), face( std::move( f.face)) { f.c = nullptr; }

     const_cube_boundary_wrapper_iterator( const Complex& c_, const Cell& cell): 
     c( nullptr), cellptr( nullptr), boundary_pos( 0), bit_index( 0), 
     dim( dimension( cell)), face( c_){
       if( dim){
       	c = &c_;
	cellptr = &cell;
       	face.coefficient( 1);
	bit_index = find_next_set_bit( cell, bit_index);
	bit_index += (bit_index==0)+1;
	face.cell() &= (1 << bit_index);
	++boundary_pos;
      }
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
         if( boundary_pos == 2*dim){ 
         	c = nullptr;
         	boundary_pos = 0;
         	return *this;
         }
         face.coefficient( -1*face.coefficient());
         ++boundary_pos;
	 face.cell() &= (1 << bit_index); //set previous flipped bit
	 bit_index *= (boundary_pos < dim);
	 bit_index = find_next_set_bit( face.cell(), bit_index);
	 face.cell() &= (1 << bit_index); //unset new bit.
	 face.cell() += (boundary_pos >= dim)*
		        ((c->index_data[ bit_index])*(1 << c->dimension()));
         return *this;	
     }
     std::size_t find_next_bit_set( const Cell & c, std::size_t & bit_index){
		c >> bit_index;
		static const int MultiplyDeBruijnBitPosition[32] = 
		{
  		0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
  		31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9};
    bit_index += MultiplyDeBruijnBitPosition[((uint32_t)((c & -c) * 0x077CB531U)) >> 27];
	return bit_index;
     }
     Self operator++( int){
      	Self tmp( *this); 
     	++(*this); //now call previous operator
     	return tmp;
     }

     Self operator--( int){
      	Self tmp( *this); 
     	--(*this); //now call previous operator
     	return tmp;
     }
	
   private:
     std::size_t dimension( const std::size_t c) const{ 
     	std::size_t d = 0;
     	for( const auto i = 0; i < c->dimension(); ++i; ){ d+= (c&(i+1)); }
     	return d;
     }

     Complex* c;
     Cell* celltpr;
     std::size_t boundary_pos;
     std::size_t bit_index;
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
			rebind< Coordinate, Coefficient>::T Term;
	
    //! const_iterator type
    typedef detail::const_cube_boundary_wrapper_iterator< Term> 
		const_iterator;
    	
    /**
    * @brief Default constructor
    */
    Cube_boundary_wrapper( const Complex & c_): Cell_boundary_() c( c_){};	
    
    /**
    * @brief Equality assignment operator 
    * @param Cube_boundary_wrapper& from
    * @return reference to this.
    */
    Cube_boundary_wrapper& operator=( const Cube_boundary_wrapper& from){ 
	c = from.c;
	Cell_boundary_::operator=( from);
	return *this; 
    }
    
    /**
    * @brief boundary begin
    * Returns an iterator to the first element of the boundary
    * @param const Cube & s
    * @return const_iterator
    */
    const_iterator begin( const std::size_t s) const { 
	return const_iterator( c, s); 
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
    	return (s.size()>1)? 2*dimension( s):0;
    }
    /**
    * @brief dimension of a cube represented by coordinates  
    * @param const Coordinate & s
    * @return returns the number of elements in the boundary.
    */

    std::size_t dimension( const Coordinate & c) const{ 
    	std::size_t d = 0;
    	for( const auto&i : c){ d+= i%2; }
    	return d;
    }

    private:
    const Complex & c;
}; //end class Cube_boundary_wrapper

} // end namespace detail
} //ctl namespace

//exported functionality
namespace ctl{} //namespace ctl


#endif //CTL_CUBE_BOUNDARY_WRAPPER_H
