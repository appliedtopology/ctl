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
*******************************************************************************/
#include <ctl/finite_field/finite_field.h>
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
	Z3 bneg( -1); 
	std::cout << "bneg = " << bneg << std::endl;
	Z11 c( 1);
	std::cout << "c = " << c << " 11*c  = " << 11*c << std::endl;
	for( std::size_t i = 1; i < c.prime(); ++i){
		c = i;
		std::cout << c << " inverse: " << c.inverse() << std::flush << std::endl;
	}
	for( std::size_t i = 1; i < c.prime(); ++i){
		c = i;
		std::cout << c << "/" << c << " should always be 1: " 
			  << c/c << std::flush << std::endl; 
	}
	std::cout << "div by zero: " << c/0 << std::endl;
	return 0;
}
