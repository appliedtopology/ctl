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
#include "chain/chain_add.h"
//STL
#include <iostream>
#include <sstream>

//CTL
//abstract_simplex
#include "abstract_simplex/abstract_simplex.h"
#include "abstract_simplex/simplex_boundary.h"

//chain_complex
#include "chain_complex/chain_complex.h"
#include "chain_complex/chain_complex_io.h"
#include "chain_complex/complex_boundary.h"

//chain
#include "chain/chain.h"

//delta
#include "io/io.h"

//filtration
#include "filtration/filtration.h"
#include "filtration/less.h"

//Term
#include "term/term_less.h"

//We build a simplicial chain complex with Z2 coefficients
typedef ctl::Abstract_simplex< int> Simplex;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Simplex, Z2 > Boundary;
typedef ctl::Chain_complex< Simplex, Boundary> Complex;
typedef Complex::Cell Cell;
typedef ctl::Complex_boundary< Complex> Complex_boundary;
typedef Complex_boundary::Term Term;
typedef ctl::Filtration< Complex, ctl::Id_less> Filtration;
typedef ctl::Chain< Term, ctl::Term_cell_less< ctl::Id_less> > Chain;
 
std::string print_chain( const Chain & a){
	std::stringstream out;
	if (a.size() == 0) { return out.str(); }
	out << a.begin()->coefficient() 
	    << "*" << a.begin()->cell()->first << " "; 
	for( auto i = ++(a.begin()); i != a.end(); ++i){
		out << i->coefficient() <<  "*" << i->cell()->first << " ";
	}
	return out.str();
}

int main( int argc, char** argv){
	Complex complex;
	Cell s( {1,2,3,4} );
	auto cell_and_count = complex.insert_closed_cell( s);
	std::cout << cell_and_count.second << " cells inserted!" << std::endl;
	std::cout << complex << std::endl;
	Complex_boundary bd( complex);	
	std::cout << "created complex boundary" << std::endl;
	auto complex_cell = complex.begin();
	auto begin_iterator = bd.begin( complex_cell);
	auto end_iterator = bd.end( complex_cell);

	std::cout << "explicitify chain" << std::endl;
	Chain bda( begin_iterator, end_iterator);
	std::cout << "chain made." << std::endl;
	std::cout << "a = " << complex_cell->first << std::endl; 
	std::cout << ctl::delta << "(a) = " << print_chain( bda) << std::endl;
	complex_cell++;
	Chain bdb( bd.begin( complex_cell), bd.end( complex_cell));
	std::cout << ctl::delta << "(b) = " << print_chain( bdb) << std::endl;
	std::cout << "b = " << complex_cell->first << std::endl; 
	Chain bdp = bda+bdb;
	std::cout << ctl::delta << "(a+b) = " << print_chain( bdp) << std::endl;
	std::cout << "Youngest["<<ctl::delta<<"(a+b)] = "
		  << bdp.youngest().coefficient() << "*" 
		  << bdp.youngest().cell()->first << std::endl;
	return 0;
}
