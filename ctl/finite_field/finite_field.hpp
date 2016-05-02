#ifndef FINITE_FIELD_H
#define FINITE_FIELD_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* Released under BSD-3 License. See LICENSE
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


template <typename T>
constexpr T sqrt_helper(T x, T lo, T hi)
{
  if (lo == hi)
    return lo;

  const T mid = (lo + hi + 1) / 2;

  if (x / mid < mid)
    return sqrt_helper<T>(x, lo, mid - 1);
  else
    return sqrt_helper(x, mid, hi);
}

template <typename T>
constexpr T sqrt(T x) { return sqrt_helper<T>(x, 0, x / 2 + 1); }


constexpr bool is_prime_number(std::size_t i){
    if (i == 1)
        return false;
    else if (i == 2)
        return true;
    else if (i % 2 == 0)
        return false;
    for (std::size_t div = 3; div <= ctl::detail::sqrt(i); div += 2)
        if (i % div == 0)
            return false;
    return true;
}

} //detail namespace
template< std::size_t prime> 
ctl::Finite_field< prime>
inverse(const ctl::Finite_field< prime>& y){ return detail::inverse(y.to_integral(), prime); }

} //ctl namespace

//exported functionality
namespace ctl{
template< std::size_t _prime> //requires detail::is_prime_number( _prime)
class Finite_field{
	private:
	typedef Finite_field< _prime> Self;
	public:
	Finite_field(){}

	template< typename T>
	Finite_field( const T n): x( mod( n) ){}

	private:
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
		return mod( rhs.to_integral());
	}

	public:
	template< typename T>
	Self& operator=( const T& from){ 
		x = get_number_data( from);
		return *this;
	}

	
	Self& operator++(){
        	return ((*this)+=1);
	}

        Self& operator--(){
        	return ((*this)-=1);
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
		return this->operator*(ctl::inverse( rhs)); 
	}

        bool operator<(const Self& rhs) const {
            return x < rhs.x;
        }
        bool operator<=(const Self& rhs) const {
            return x < rhs.x;
        }
        bool operator>(const Self& rhs) const {
            return x > rhs.x;
        }
        bool operator>=(const Self& rhs) const {
            return x >= rhs.x;
        }


	template< typename T>
	Self& operator/=(const T& rhs){ 
		*this= this->operator*(ctl::inverse( rhs)); 
		return *this;
	}
	std::size_t to_integral() const { return x; }	
	private:
	std::size_t x;	
}; //class Finite_field

template< typename Stream, std::size_t N>
Stream& operator<<( Stream & out, const Finite_field< N> & x ){
	std::size_t value = x.to_integral();
	if(N>(std::size_t)2 && (std::size_t)value >= N/2){ 
		value = -1*(N -value);  
	}
	out << value; 
	return out;
}

template< typename Stream, std::size_t N>
Stream& operator<<( Stream & out, const Finite_field< N> && x ){
	out << x; 
	return out;
}



} //namespace ctl

template<typename T, std::size_t N, 
	 typename = std::enable_if_t<!std::is_same< T, ctl::Finite_field<N>>::value> >
decltype(auto) operator*(const T k, ctl::Finite_field<N>& rhs) { return rhs *= k;  }


#endif //FINITE_FIELD_H
