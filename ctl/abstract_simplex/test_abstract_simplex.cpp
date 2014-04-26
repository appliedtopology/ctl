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
//STL
#include <iostream>

//CTL
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>
#include <ctl/term/term.h>


typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Simplex_boundary< Simplex, int> Boundary;
int main( int argc, char** argv){
	Simplex s;
	std::cout << "build a simplex, vertex by vertex." << std::endl;
	s.insert( 2);
	std::cout << s << std::flush << std::endl;
	s.insert( 1);
	std::cout << s << std::flush << std::endl;
	s.insert( 4);
	std::cout << s << std::flush << std::endl;
	s.insert( 3);
	std::cout << s << std::flush << std::endl;
	Simplex c( s);
	std::cout << "(copy test) c = " << c << " does c == s?" << (c == s) << std::flush << std::endl;
	Simplex t( {1,2,5,5,4});
	std::cout << "(il constructor) t = " << t << std::flush << std::endl;
	t.insert( {8,9,10} );
	std::cout << "(insert test 8,9,10) t = " << t << std::flush << std::endl;
	t.remove( 5);
	std::cout << "t = " << t << std::flush << std::endl;
	t.remove( t.begin()+2, t.begin()+4);
	
	std::cout << "t = " << t << std::flush << std::endl;
	Boundary b;
	std::cout << "printing out the boundary of t" << std::endl;
	for (auto i = b.begin( t); i != b.end( t); ++i){
		std::cout << (*i).cell() << std::endl;
	}
	return 0;
}
