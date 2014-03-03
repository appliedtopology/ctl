#ifndef FINITE_FIELD_H
#define FINITE_FIELD_H
#include <iostream>

//TODO: Ensure things here are efficient.
//TODO: possibly provide a generic multiplication table 
//TODO: Specialize for N=2

//forward declarations
namespace ctl {
template< std::size_t N> class Finite_field; 
}

//non-exported functionality
namespace {

//Extended Euclidean Algorithm 
std::size_t _inverse(const std::size_t a, const std::size_t prime){
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
std::size_t _inverse( const ctl::Finite_field< N> & x, const std::size_t prime){ 
	return _inverse( x.value(), prime); 
}

} //anon. namespace

//exported functionality
namespace ctl{
template< std::size_t _prime>
class Finite_field{
	private:
	typedef Finite_field< _prime> Self;
	public:
	Finite_field(){}

	template< typename T>
	Finite_field( const T n): x( mod( n) ){};

	//mod avoid branch when possible.
	template <typename T>
	typename std::enable_if< std::is_unsigned<T>::value, 
				 std::size_t>::type mod(T n) const {
		return n%_prime;
	}
	
	template <typename T>
	typename std::enable_if< !std::is_unsigned<T>::value, 
				 std::size_t>::type mod(T n) const {
		return (n>=0)? n % _prime: _prime-(std::abs(n)%_prime); 
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
		return *this*_inverse( rhs, _prime); 
	}

	template< typename T>
	Self& operator/=(const T& rhs){ 
		*this= *this*_inverse( rhs, _prime); 
		return *this;
	}
	
	Self inverse() const{ return Self( _inverse( x, _prime)); }

	const std::size_t prime() const { return _prime; }
	const std::size_t value() const { return x; } 
	private:
	std::size_t x;	
}; //class Finite_field

} //namespace ctl

template< typename Stream, std::size_t N>
Stream& operator<<( Stream & out, const ctl::Finite_field< N> & x ){
	int value = x.value();
	if( x.prime()>2 && value >= x.prime()/2.0){ 
		value = -1*(x.prime() -value);  
	}
	out << std::showpos << value << std::noshowpos;
	return out;
}

template<typename T, std::size_t N> 
ctl::Finite_field< N> operator* (T k, const ctl::Finite_field< N> &m) { 
	return m * k; 
}

template<typename T, std::size_t N> 
ctl::Finite_field< N> operator/ (T k, const ctl::Finite_field< N> &m) { 
	return m / k; 
}


template<typename T, std::size_t N> 
ctl::Finite_field< N> operator+ (T k, const ctl::Finite_field< N> &m) { 
	return m + k; 
}

#endif //FINITE_FIELD_H
