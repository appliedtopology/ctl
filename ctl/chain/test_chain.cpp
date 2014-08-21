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
#include <sstream>
#include <iomanip>

//CTL

//abstract_simplex
#include <ctl/abstract_simplex/abstract_simplex.h>
#include <ctl/abstract_simplex/simplex_boundary.h>

//Chain_complex
#include <ctl/chain_complex/chain_complex.h>
#include <ctl/chain_complex/complex_boundary.h>

//chain
#include <ctl/chain/chain_add.h>
#include <ctl/chain/chain.h>

//delta
#include <ctl/io/io.h>

//filtration
#include <ctl/filtration/filtration.h>
#include <ctl/filtration/less.h>

//Term
#include <ctl/term/term_less.h>
#include <ctl/utility/timer.h>

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
	ctl::Timer timer;
	Cell s( {1,2,3,4,5,6,7,8,9,10} );
	Cell t( {1,2,3,4,5,11,12,13,14,15} );

	std::cout  << std::setprecision( 20) << std::endl;
	
	timer.start();	
	auto cell_and_count = complex.insert_closed_cell( s);
	timer.stop();
	std::cout << cell_and_count.second 
		  << " cells inserted! in " << timer.elapsed() << std::endl;

	timer.start();	
	cell_and_count = complex.insert_closed_cell( t);
	timer.stop();
	std::cout << cell_and_count.second 
		  << " cells inserted! in " << timer.elapsed() << std::endl;

	Complex_boundary bd( complex);	
	auto complex_s = complex.find_cell( s);
	auto complex_t = complex.find_cell( t);
	Chain bds( bd.begin( complex_s), bd.end( complex_s));
	Chain bdt( bd.begin( complex_t), bd.end( complex_t));
	constexpr int N = 600000;
	Chain temp;
	ctl::Timer l;
	l.start();
	timer.start();
	for( int i = 0; i < N; ++i){	
	bds.scaled_add( 1, bdt, temp);
	timer.stop();
	Chain bds( bd.begin( complex_s), bd.end( complex_s));
	Chain bdt( bd.begin( complex_t), bd.end( complex_t));
	}
	l.stop();
	std::cout << timer.elapsed()/N << std::endl;
	std::cout << l.elapsed() << std::endl;
	std::cout << print_chain( bds+bdt) << std::endl;
	return 0;

}
