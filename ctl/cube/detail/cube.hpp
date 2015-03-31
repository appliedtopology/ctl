#ifndef CTL_DETAIL_CUBE_H
#define CTL_DETAIL_CUBE_H
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
* Released under BSD-3 License. See LICENSE
*******************************************************************************
* Notes: 
* August 27, 2014
* Instead of thinking of a cube as as a product of intervals -- the std. def.
* I use the approach of Hubert Wagner et. all in:
* Efficient Computation of Persistent Homology for Cubical Data
* In particular a cubical complex on $n$ vertices in $d$ dimensions is an array 
* of size m = (2n-1)^d
*******************************************************************************/
#include <vector> //std::vector
#include <utility> //std::pair
#include <initializer_list>  // std::initializer_list
#include <iostream> //cout (debug only)
#include <algorithm> //swap, sort, unique
#include <ctl/cube/cube_boundary.hpp>

/*! \namespace ctl
Namespace where all library functionality resides
*/
namespace ctl {
namespace detail{
/**
* \class detail::Cube< T>
* This class describes an cube for a cubical chain complex.
* While a cube is defined as a product of intervals, a cubical complex
* does not store a vector of these. A cube is infact stored as a single integer.
* Below is the class which encodes this word.
*/
template< typename C, std::size_t D=3>
class Cube {
	private:
	typedef typename C::vertex_type T;

	typedef Cube< C, D> Self;
	public:
	//! \typedef  
	typedef std::size_t size_t;
	//! \typedef synonomous with vertex_type  
	typedef typename std::size_t value_type;
	//! \typedef The vertex type of the cube  
	typedef value_type vertex_type;
	//TODO: add begin() and end() iterators to iterate over the vertices of a cube..
	public:
	//! Default constructor
	Cube(): data_( 0){}
	Cube( size_t data__): data_( data__){}
	//! Copies the data_ from one cube to another
	Cube( const Self & from): data_( from.data_) {};
	//! Moves the data_ from one cube to another
	Cube( Self && from): data_( std::move( from.data_)){}

	/*! 
	* Returns the total number of vertices in the cube.
	* @return size_t 
	*/
	size_t  size() const	{ return std::pow(2, dimension()+1); }
	/*! Returns the dimension of the cube
	 * @return size_t
	 */
	size_t  dimension() const	{ 
	   std::size_t low_bits = data_ & ( (1 << D) -1);
	   if( low_bits){ return __builtin_popcount( low_bits); }
	   return 0;
	}
	/*! Returns the capacity of the cube
	* @return size_t
	*/
	size_t   capacity() const  { return 1; }
	void clear() { data_ = 0; }

	//TODO: THIS IS A TOTAL HACK	
	const std::size_t* begin() const { return &data_; }
	//TODO: IMPLEMENT insert/remove?
	
	Self lower_left_vertex() const{ return Self( data_ & ~( (1 << D)-1));}
	std::size_t lower_left_vertex_id() const{ return data_ >> D; }
	/**
	* @brief Copy assignment operator
	* @param Cube & b
	* @return Reference to the assigned to cube
	*/
	Self& operator=( const Self & b) { 
		data_ = b.data_;
		return *this;
	}

	/**
	* @brief Move assignment operator 
	* @param Cube && b
	* @return Reference to the assigned to cube
	*/
	Self& operator=( Self && b) {
		data_ = std::move( b.data_);
		return *this;
	}
	template< typename T>
	std::size_t operator&( const T & t) const{ return data_&t;}

	template< typename T>
	Self operator+( const T & t) const{return Self(data_+t);}
	template< typename T>
	Self operator-( const T & t) const{return Self(data_-t);}
	Self operator~() const {return Self(~data_);} 
	//Self& operator~=() {data_ = ~data; return *this;}
	template< typename T>
	Self& operator+=( const T & t) {data_+=t; return *this;}
	template< typename T>
	Self& operator-=( const T & t) {data_-=t; return *this;}
	template< typename T>
	Self& operator*=( const T & t) {data_*=t; return *this;}
	template< typename T>
	Self& operator/=( const T & t) {data_/=t; return *this;}
	template< typename T>
	Self& operator&=( const T & t) {data_&=t; return *this;}
	template< typename T>
	Self& operator|=( const T & t) {data_|=t; return *this;}
	template< typename T>
	Self operator^(const T & t)const {return Self( data_^t);}
	template< typename T>
	Self& operator^=( const T & t) {data_^=t; return *this;}
	Self& operator++() { data_+=1; return *this;}
	Self operator++( int) { Self ans = *this; ++(*this); return ans;}
	Self& operator--() { data_-=1; return *this;}
	Self operator--( int) { Self ans = *this; --(*this); return ans;}
	template< typename T>
	Self operator>>( const T & t) const { return Self(data_ >> t);}
	template< typename T>
	Self operator<<( const T & t) const { return Self(data_ << t); }
	template< typename T>
	Self& operator>>=( const T & t) const { data_ >>= t; return *this;}
	template< typename T>
	Self& operator<<=( const T & t) const { data_ <<= t; return *this;}
	template< typename T>
	std::size_t operator&&( const T & t){ return data_ && t;}
	template< typename T>
	std::size_t operator||( const T & t){ return data_ || t;}
	bool operator!()const{ return (data_==0); }
	//template< typename T>
	//Self& operator&&=( const T & t){ data_ &&= t; return *this; }
	//template< typename T>
	//Self& operator||=( const T & t){ data_ ||= t; return *this;}
	
	/**
	* @brief Comparator
	* Compares two simplices first by size, breaking ties lexicographically.
	* @param const Cube & b
	* @return true if a < b, false otherwise. 
	*/
	bool operator<( const Self & b) const {
	return (dimension() < b.dimension()) || 
	  ((dimension() == b.dimension()) && ((data_ >> D) < (b.data_ >> D)));
	}
	void reserve( std::size_t n){} 
	void resize( std::size_t n){}
	void swap( Cube & r){ std::swap( data_, r.data_);}
	/**
	* @brief Equality operator
	* @param const Cube & b
	* @return is equivalent to !(a < b) && !(b < a)
	*/
	bool operator==( const Self & b) const { return (data_ == b.data_); }
	
	/**
	* @brief Non equality operator
	* @param const Cube & b
	* @return true iff (*this) == b is false
	*/
	bool operator!=( const Self & b) const{ 
		return (data_ != b.data_);
	}

	std::size_t& data()	 { return data_; }
	std::size_t data() const { return data_; }
	private:
	std::size_t data_;

}; //detail::Cube
template< typename Stream, typename T, std::size_t D>
Stream& operator<<( Stream & out, const ctl::detail::Cube< T, D> & c){
	out << c.data(); 
	return out;
}
} //namespace detail
 
} //namespace ctl

#endif //CUBE_H
