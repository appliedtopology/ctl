#ifndef FINITE_FIELD_H
#define FINITE_FIELD_H
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
#include <iostream>

//TODO: Ensure things here are efficient.
//TODO: possibly provide a generic multiplication table 
//TODO: Specialize for N=2

//forward declarations
namespace ctl {
template< std::size_t N> class Finite_field; 
}

//non-exported functionality
namespace ctl {
namespace detail{
//Extended Euclidean Algorithm 
inline std::size_t inverse(const std::size_t a, const std::size_t prime){
    if (a == 0){ std::cerr << std::endl << "!!! divide by 0. !!!" << std::endl;}
    std::size_t b = prime-2;
    std::size_t x = 1, y = a;
    while(b > 0) {
        if(b%2 == 1) {
            x=(x*y);
            if(x>prime) x%=prime;
        }
        y = (y*y);
        if(y>prime) y%=prime;
        b /= 2;
    }
    return x;
}

template< std::size_t N>
std::size_t inverse( const ctl::Finite_field< N> & x, const std::size_t prime){ 
	return inverse( x.value(), prime); 
}

} //detail namespace
} //ctl namespace

//exported functionality
namespace ctl{
template< std::size_t _prime>
class Finite_field{
	private:
	typedef Finite_field< _prime> Self;
	public:
	Finite_field(){}

	template< typename T>
	Finite_field( const T n): x( mod( n) ){}

	//mod avoid branch when possible.
	template <typename T>
	typename std::enable_if< std::is_unsigned<T>::value, 
				 std::size_t>::type mod(T n) const {
		return n%_prime;
	}
	
	template <typename T>
	typename std::enable_if< !std::is_unsigned<T>::value, 
				 std::size_t>::type mod(T n) const {
		return (n>=0)? (n % _prime) : (_prime-((-1*n)%_prime)); 
	}

	//given an arbitrary number get a number between [0, prime)
	template< typename T>
	std::size_t get_number_data(const T & rhs) const { return mod( rhs); }
	
	template< std::size_t N>
	std::size_t get_number_data( const ctl::Finite_field< N> & rhs) const {
		return mod( rhs.value());
	}

	template< typename T>
	Self& operator=( const T& from){ 
		x = get_number_data( from);
		return *this;
	}

	template< typename T>
	Self operator+( const T& rhs) const { 
		return Self( x+get_number_data(rhs)); 
	}

	template< typename T>
	Self& operator+=( const T& rhs){ 
		x+=get_number_data( rhs); 
		x=mod( x); 
		return *this; 
	}

	template< typename T>	
	Self operator-( const T& rhs) const { 
		const std::size_t lhs = x  + (_prime - get_number_data(rhs));
		return Self( lhs); 
	}
	template< typename T>
	Self& operator-=( const T& rhs){ 
		x = mod(x  + (_prime - get_number_data(rhs)));
		return *this; 
	}

	template< typename T>
	Self operator*( const T& rhs) const { 
		return Self( x*get_number_data(rhs)); 
	}
	template< typename T>
	Self& operator*=( const T& rhs){ 
		x*=get_number_data(rhs); 
		x = mod(x); 
		return *this; 
	}
	
	template< typename T>
	bool operator==( const T& rhs) const {
		return (x == get_number_data( rhs));
	}

	template< typename T>
	bool operator!=( const T& rhs) const {
		return (x != get_number_data( rhs));
	}



	template< typename T>
	Self operator/(const T& rhs) const{ 
		return *this*ctl::detail::inverse( rhs, _prime); 
	}

	template< typename T>
	Self& operator/=(const T& rhs){ 
		*this= *this*ctl::detail::inverse( rhs, _prime); 
		return *this;
	}
	
	Self inverse() const{ return Self( ctl::detail::inverse( x, _prime)); }

	const std::size_t prime() const { return _prime; }
	const std::size_t value() const { return x; } 
	private:
	std::size_t x;	
}; //class Finite_field


template< typename Stream, std::size_t N>
Stream& operator<<( Stream & out, const Finite_field< N> && x ){
	int value = x.value();
	if( x.prime()>2 && value >= x.prime()/2.0){ 
		value = -1*(x.prime() -value);  
	}
	out << value; 
	return out;
}

} //namespace ctl

template<typename T, std::size_t N> 
ctl::Finite_field< N> operator* (T k, const ctl::Finite_field< N> &m) { 
	return m * k; 
}

#endif //FINITE_FIELD_H
