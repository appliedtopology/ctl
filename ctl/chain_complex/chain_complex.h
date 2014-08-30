#ifndef CTL_CHAIN_COMPLEX_H
#define CTL_CHAIN_COMPLEX_H
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
*******************************************************************************/

//STL
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>

//CTL
#include <ctl/io/io.h>
#include <ctl/chain_complex/detail/complex_storage.h>
#include <ctl/hash/hash.h>


//forward declaration
namespace ctl{
template< typename Cell_, typename Boundary_,
	  typename Data_, typename Hash_, 
	  template <typename...> class Storage_>
class Chain_complex;
} //namespace ct

//non-exported functionality
namespace ctl {
namespace detail {

template< typename Data_>
class Data_wrapper : public Data_ {
   private:
   typedef Data_wrapper< Data_> Self;
   typedef std::size_t Id;
   public:
   //default
   Data_wrapper(): id_( 0) {}
   //copy
   Data_wrapper( const Id & tid):
   Data_(), id_( tid){}

   Data_wrapper( const Data_wrapper & from) : id_( from.id_){}
   //move
   Data_wrapper( const Data_wrapper && from):
   	id_( std::move( from.id_)){}

   Self& operator=( const Self & from){
   	Data_::operator=( from);
   	id_ = from.id_;
   	return *this;
   }

   Self& operator=( Self && from){
   	Data_::operator=( from);
   	id_ = std::move( from.id_);
   	return *this;
   }


   Id id() const { return id_; }
   void id( Id n){ id_ = n; }
   private:
   Id id_;
}; // class Data_wrapper

struct Default_data {}; //class Default_data for complex.
template< typename Stream>
Stream& operator<<( Stream & out, const Default_data & d){ return out; }
template< typename Stream>
Stream& operator<<( Stream & out, const Default_data && d){ return out; }
} //detail
} //ctl namespace


//exported functionality
namespace ctl{
/**
* @brief \class Chain_complex
* A structure which stores the standard cell basis for a Chain_complex
* The Cell type is specified via the template parameter Cell
* One must provide an operator which allows the boundary of a cell to be 
* enumerated.
* 
* The library currently provides two cell types, ctl::Abstract_simplex< T> 
* and ctl::Cube< T>.
*
* The Data type can be anything, and is used to associate Data to a Cell.
*
* Presently, we wrap the Data parameter transparently to associate a unique id 
* to each cell. If the cells of the complex are inserted in a filtration order 
* Then sorting the cells of the complex according to these id's produces a 
* filtration ordering. 
*
* You may optionally provide a Hash function which might be used by an internal 
* container. The Hash function hashes Cells to a std::size_t. By default we use
* ctl::Hash.
*  
* The Storage parameter controls the internal data structure used for 
* storing the complex.
* This structure depends on the Cell type.
* At the time of writing, if Cell == ctl::Cube< T> we use 
* a std::vector< Data> the cube is stored implicitly as an offset into this
* structure. If a cell is not a cube, 
* then we use a std::unordered_map< Cell, Boundary, Data, Hash>
* 
* For those power users out there, one can roll their own internal
* storage container. The container must have the Following signature:
*   Your_container< Cell, Boundary, Data, Hash>  
* In lieu of writing down the other requirements on the structure,
* we refer the reader to the implementation of the Chain_complex.
* Essentially you must export similar types and functionality.
* The authors have tried our best to minimize the number of requirements on such
* a data structure.

* @tparam Cell
* @tparam Boundary
* @tparam Data
* @tparam Hash
* @tparam Storage
*/
template< typename Cell_,
	  typename Boundary_,
	  typename Data_ = ctl::detail::Default_data,
	  typename Hash_ = ctl::Hash< Cell_>, 
	  template <typename...> class Storage_ = detail::Complex_storage> 
class Chain_complex{
public:
   /*!Describes a fundamental object,
    * e.g. Simplex, Cube, Polygon, etc.. */
   typedef Cell_ Cell; 
   //! Deprecated: Describes how to take Cell boundary
   typedef Boundary_ Boundary; 

   //! Describes how to take Cell boundary
   typedef Boundary_ Cell_boundary; 
   //! Arbitrary data associated to space.
   typedef detail::Data_wrapper< Data_> Data;
private:
    //! Internal complex storage
    typedef Storage_< Cell, Boundary, Data, Hash_> Complex;
   
public:
   //!Internal hash function used by underlying storage container
   typedef typename Complex::Hash Hash;
   //! size_type used to store complex
   typedef typename Complex::size_type size_type;
   //! iterator type
   typedef typename Complex::iterator iterator;
   //! const iterator type
   typedef typename Complex::const_iterator const_iterator;
   //! Value type
   typedef typename Complex::value_type value_type;

public:
   //Constructors
   //! Default constructor
   Chain_complex()  {}

 /*! Cell_boundary constructor 
   * You may optionally provide a container hint as to the total number of 
   * complex. 
   * The underlying container can choose to preallocate this much memory.
   */
   Chain_complex( Cell_boundary & bd_, const std::size_t num_complex=1): 
	complex( bd_, num_complex){}
 
 /*! Cell_boundary constructor 
   * You may optionally provide a container hint as to the total number of 
   * complex in each dimension
   * The functionality of the hint depends on the underlying container
   * For example if the underlying Cell type is Simplicial then   
   * The underlying container can choose to preallocate this much memory.
   * However if the underlying Cell type is Cubical then this is used to
   * inform the container of the number of vertices in each dimension. 
   */
   template< typename Size_by_dimension>
   Chain_complex( Cell_boundary & bd_, const Size_by_dimension& d): 
   complex( bd_, d) {} 

   //! Copy constructor
   Chain_complex( const Chain_complex & b): complex( b.complex){} 

   //! Move constructor
   Chain_complex( Chain_complex && b): complex( std::move( b.complex)){}

   //! Assignment operator
   Chain_complex& operator=( const Chain_complex& b){
   	complex = b.complex;
   	return *this;
   }

   // move assignment operator
   Chain_complex& operator=( Chain_complex&& b){
   	complex   = std::move( b.complex);
   	return *this;
   }

   //! given a Cell s return an iterator to that cell in the complex. 
   iterator       find_cell( const Cell & s)       { return complex.find( s); }

   //! given a Cell s return a const_iterator to that cell in the complex
   const_iterator find_cell( const Cell & s) const { return complex.find( s); }

   //! Begin iterator over complex
   iterator       begin()       { return complex.begin(); }
   //! End iterator over complex
   iterator         end()       { return complex.end();   }

   //! Begin const_iterator over complex
   const_iterator begin() const { return complex.begin(); }

   //! End const_iterator over complex
   const_iterator   end() const { return complex.end();   }

   /*! Inserts the cell s into the complex with associated data d
    */
   std::pair< iterator, bool> insert_open_cell( const Cell & s,
   					        const Data& data=Data()){
	return complex.insert_open_cell( s, data);
   }

   /*! Inserts Closure(s) into the complex such that
   *  Internal id respects filtration property
   *  setting closed_construction = true 
   *  is a hint to the structure that the complex has 
   *  been constructed closed. For example if only calls to 
   *  insert_closed_cell() have been used to built the complex. 
   */  
   std::pair< iterator, std::size_t>
   insert_closed_cell( const Cell & s, 
		       const bool closed_construction=false,
   		       const Data&  data = Data()){
   	return complex.insert_closed_cell( s, closed_construction, data);
   }

   //! write complex out to stream applying functor
   template< typename Stream, typename Functor>
   Stream& write( Stream& out, const Functor & f) const { 
	return complex.write( out, f);
   }

   //! write complex out to stream
   template< typename Stream>
   inline Stream& write( Stream& out) const { 
	return complex.write( out);
   }

   //! read complex in from stream   
   template< typename Stream> 
   Stream& read( Stream & in){ return complex.read( in); }

   //! read in data associated to the complex
   template< typename Stream, typename Functor>
   Stream& read_data( Stream & in, Functor & f){ complex.read_data( in, f); }

   //! Reserves space for n complex in dimension d
   void reserve( const std::size_t n, const std::size_t dim) { 
	complex.reserve( n, dim); 
   }

   //! Reserves space for n complex 
   void reserve( const std::size_t n) { complex.reserve( n); }

   //! Returns the dimension of the complex
   const std::size_t dimension() const { return complex.dimension(); }

   //! Return the number of complex in the complex
   const std::size_t size() const { return complex.size(); }

   //! Returns the number of complex in dimension i 
   const std::size_t size( std::size_t i) const { return complex.size( i); }


   //! Returns the cell boundary operator
   Cell_boundary& cell_boundary() { return complex.cell_boundary(); }
   
   //! Deprecated, returns the cell boundary operator
   Cell_boundary& boundary() { return cell_boundary(); } 

   //! Returns true if the complex is closed under the subset relation
   bool is_closed() const{ return complex.is_closed(); }
private:
   Complex complex;
}; //chain_complex
} //namespace ctl

//! Output stream operator for a Chain_complex iterator
template< typename Stream, typename C, typename B, 
	   typename D, typename H>
Stream& operator<<( Stream& out, 
		    const typename ctl::Chain_complex< C, B, D, H>::iterator c){ 
	out << c->first;
	return out;	
}

//! Output stream operator for a Chain_complex const_iterator
template< typename Stream, typename C, typename B, 
	   typename D, typename H>
Stream& operator<<( Stream& out, 
              const typename ctl::Chain_complex< C, B, D, H>::const_iterator c){ 
	out << c->first;
	return out;	
}

//! Output stream operator for a Chain complex 
template< typename Stream, typename Cell, typename Boundary, 
	   typename Data, typename Hash>
Stream& operator<<( Stream& out, 
		    const ctl::Chain_complex< Cell, Boundary, Data, Hash> & c){ 
	for(auto i = c.begin(); i != c.end(); ++i){
		      const std::size_t id = i->second.id();
		      out << id; 
		      out << ": ";
		      out << i->first << std::endl; 
	}
	return out;
}

//! Output stream operator for a Chain complex x-value
template< typename Stream, typename Cell, typename Boundary, 
	   typename Data, typename Hash>
Stream& operator<<( Stream& out, 
		    const ctl::Chain_complex< Cell, Boundary, Data, Hash> && c){ 
	out << c;
	return out;
}

//! Input stream operator for a Chain complex 
template< typename Stream, typename Cell, 
	  typename Boundary, typename Data_, typename Hash>
Stream& operator>>( Stream& in, 
		    ctl::Chain_complex< Cell, Boundary, Data_, Hash> & c){  
	return c.read( in); 
}

namespace ctl{

//! Utility to read a complex and any associated data 
template<typename String, typename Complex, typename Functor>
void read_complex_and_data(String & complex_name, String & data_file, 
			   Complex & complex, Functor & f){
	std::ifstream in;
	std::cout << "File IO ..." << std::flush;
	//first read the complex in
	ctl::open_file( in, complex_name.c_str());
	complex.read( in);
	ctl::close_file( in);
	//then read the data file in, e.g. weights, cover, etc..
	ctl::open_file( in, data_file.c_str());
	complex.read_data( in, f);	
	ctl::close_file( in);
	std::cout << "completed!" << std::endl;
}

//! Utility to read a complex 
template<typename String, typename Complex>
void read_complex(String & complex_name, Complex & complex){
	std::ifstream in;
	std::cout << "File IO ..." << std::flush;
	ctl::open_file( in, complex_name.c_str());
	complex.read( in);
	ctl::close_file( in);
	std::cout << "completed!" << std::endl;
}

} //namespace ctl

#endif //CTL_CHAIN_COMPLEX_H
