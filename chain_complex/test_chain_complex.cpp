/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the below liscence, but please
* do not commit acts of academic dishonesty. We encourage and request that 
* for any academic use of this source code one should cite the following 
* papers:
* 
* \cite{$bibtex_names_here}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) $NAMES_OF_AUTHORS $YEARS_FIRST_CREATED <$emails>
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
#include "chain_complex/chain_complex_io.h"
#include "chain_complex/complex_boundary.h"

//We build a simplicial chain complex with Z2 coefficients
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 3> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2 > Cell_boundary;
typedef ctl::Chain_complex< Simplex, Cell_boundary> Complex;
typedef Complex::Cell Cell;
typedef ctl::Complex_boundary< Complex> Complex_boundary;
typedef Complex_boundary::const_iterator boundary_iterator;
int main( int argc, char** argv){
	Complex complex;
	Cell s( {1,2,3,4} );
	auto pair = complex.insert_closed_cell( s);
	std::cout << pair.second << " cells inserted!" << std::endl;
	std::cout << complex << std::endl;
	Complex_boundary b( complex);
	Complex_boundary copyd( b);
	Complex_boundary moved( std::move( copyd));
	std::cout << "boundary test: " 
		  << complex.begin()->first << std::endl;
	auto wtf = complex.begin();
	std::cout << "boundary length: " << b.length( wtf) << std::endl;
	for( boundary_iterator i = b.begin( wtf, 0); i != b.end( wtf); ++i){
		std::cout << i->cell()->first 
			  << " --> {" 
			  << i->cell()->second  
			  << "}"<< std::endl;
	}
	std::ofstream out( "test.asc");
	complex.write( out); 
	return 0;
}
