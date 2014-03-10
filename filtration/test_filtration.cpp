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
//abstract_simplex
#include "abstract_simplex/abstract_simplex.h"
#include "abstract_simplex/simplex_boundary.h"

//chain_complex
#include "chain_complex/chain_complex.h"

//filtration
#include "filtration/filtration.h"
#include "filtration/less.h"
//timer
#include "utility/timer.h"

//We build a simplicial chain complex with Z2 coefficients
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2 > Boundary;
typedef ctl::Chain_complex< Simplex, Boundary> Complex;
typedef Complex::Cell Cell;
typedef ctl::Filtration< Complex> Filtration_cell_less;
typedef ctl::Timer Timer;
typedef ctl::Filtration< Complex, ctl::Id_less> Filtration_id_less;

template< typename Filtration>
bool is_sorted( const Filtration & f){
	typedef typename Filtration::Less Less;
	Less less;
	auto last = f.begin();
	for( auto i = ++f.begin(); i < f.end(); ++i, ++last){
		if (!less( *last, *i)){ return false; }
	}
	return true;
}

int main( int argc, char** argv){
	Complex c;
	Timer t;
	t.start();
	Cell s( {1,2,3,4,5,6,8,9,10} );
	c.insert_closed_cell( s);
	t.stop();
	std::cout << "construction: " << t.elapsed() << std::endl;
	std::cout << "size: " << c.size() << std::endl;
	t.start();
	Filtration_cell_less f1( c);
	t.stop();
	std::cout << "f1 is sorted " << is_sorted( f1) 
		  << " " << t.elapsed() << std::endl; 
	t.start();
	Filtration_id_less f2( c);
	t.stop();
	std::cout << "f2 is sorted " << is_sorted( f2) 
		  << " " << t.elapsed() << std::endl; 
	for( auto i = f2.begin(); i != f2.end(); ++i){
		std::cout << i.pos() << ": " << (*i)->first << std::endl;
	}
	return 0;
}
