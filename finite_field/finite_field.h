#ifndef FINITE_FIELD_H
#define FINITE_FIELD_H
#include <iostream>

//TODO: Ensure things here are efficient.
//TODO: possibly provide a generic multiplication table 

//forward declarations
namespace ctl {
template< int N> class Finite_field; 
}

//non-exported functionality
namespace {
//Extended Euclidean Algorithm 
int _inverse(const int a, const int prime){
    if (a == 0){ std::cerr << std::endl << "!!! divide by 0. !!!" << std::endl; }
    int b = prime-2;
    int x = 1, y = a;
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
template< int N>
int _inverse( const ctl::Finite_field< N> & x, const int prime ){ 
	return _inverse( x.value(), prime); 
}
template< typename T>
int get_number_data(const T & rhs){ return rhs;}
template< int N>
int get_number_data( const ctl::Finite_field< N> & rhs){ return rhs.value(); }

} //anon. namespace

//exported functionality
namespace ctl{
template< int _prime>
class Finite_field{
	private:
	typedef Finite_field< _prime> Self;
	public:
	Finite_field(){}
	Finite_field( const unsigned int n): x( n % _prime){};

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
		x+=get_number_data(rhs); 
		x=x%_prime; 
		return *this; 
	}

	template< typename T>	
	Self operator-( const T& rhs) const { 
		return Self( x-get_number_data(rhs)); 
	}
	template< typename T>
	Self& operator-=( const T& rhs){ 
		x-=get_number_data(rhs); 
		x=x%_prime; 
		return *this; 
	}

	template< typename T>
	Self operator*( const T& rhs) const { 
		return Self( x*get_number_data(rhs)); 
	}
	template< typename T>
	Self& operator*=( const T& rhs){ 
		x*=get_number_data(rhs, _prime); 
		x%_prime; 
		return *this; 
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

	const unsigned int prime() const { return _prime; }
	const unsigned int value() const { return x; } 
	private:
	unsigned int x;	
}; //class Finite_field

} //namespace ctl

template< typename Stream, int N>
Stream& operator<<( Stream & out, const ctl::Finite_field< N> & x ){ 
	out << x.value();
	return out;
}

template<typename T, int N> 
ctl::Finite_field< N> operator* (T k, const ctl::Finite_field< N> &m) { 
	return m * k; 
}

template<typename T, int N> 
ctl::Finite_field< N> operator/ (T k, const ctl::Finite_field< N> &m) { 
	return m / k; 
}


template<typename T, int N> 
ctl::Finite_field< N> operator+ (T k, const ctl::Finite_field< N> &m) { 
	return m + k; 
}

#endif //FINITE_FIELD_H
