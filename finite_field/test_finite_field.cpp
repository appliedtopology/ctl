#include "finite_field.h"
#include <iostream>

int main( int argc, char** argv){
	typedef ctl::Finite_field< 2> Z2;
	typedef ctl::Finite_field< 3> Z3;
	typedef ctl::Finite_field< 11> Z11;
	
	Z2  a( 1);
	std::cout << "a = " << a << " a+a = " << a+a << std::endl;	
	Z3  b( 1);	
	std::cout << "b = " << b << " b + b = " << b+b  
		  << " 3*b: " << b+b+b << std::endl;	
	Z11 c( 1);
	std::cout << "c = " << c << " 11*c  = " << 11*c << std::endl;
	for( int i = 1; i < c.prime(); ++i){
		c = i;
		std::cout << c << " inverse: " << c.inverse() << std::flush << std::endl;
	}
	for( int i = 1; i < c.prime(); ++i){
		c = i;
		std::cout << c << "/" << c << " should always be 1: " 
			  << c/c << std::flush << std::endl; 
	}
	std::cout << "div by zero: " << c/0 << std::endl;
	return 0;
}
