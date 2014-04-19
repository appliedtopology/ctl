#ifndef PRODUCT_CELL_H
#define PRODUCT_CELL_H
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
/* NOTES:
* This product cell design is a pair< Cell1, Cell2>
* For space concerns, we represent Cell1 and Cell2 as _iterators_
* - Of course, stylistically, it would be good for Product_cell, to allow for Cell1/Cell2 to truly be 
    the value type of these iterators
*/
#include <utility> //std::pair, and piecewise_construct
#include <tuple> //std::pair, and piecewise_construct
#include <iostream> //cout (debug only)
#include "product_boundary.h"
namespace ctl {

template< typename Cell_iterator1_, Cell_iterator2_>
class Product_cell : public std::pair< Cell_iterator1_ , Cell_iterator2_> {
	public:	
	typedef typename Cell_iterator1_ Cell_iterator1;
	typedef typename Cell_iterator2_ Cell_iterator2;
	typedef typename Cell_iterator1::value_type::first_type Cell1;
	typedef typename Cell_iterator2::value_type::first_type Cell2;
	private:
	typedef std::pair< Cell_iterator1_, Cell_iterator2_> Pair;
	typedef Product_cell< Cell_iterator1, Cell_iterator2> Self;
	public:

	//default
	Product_cell() {};

	//copy
	Product_cell( const Product_cell & from) : Pair( from) {}	

	//standard
	Product_cell( const Cell_iterator1 & cell1, 
		      const Cell_iterator2 & cell2): Pair( cell1, cell2) {}
	//initialize
	Product_cell( Cell_iterator1 && cell1, 
		      Cell_iterator2 && cell2): 
		      Pair( std::forward(cell1), std::forward(cell2)) {}
	//move
	Product_cell( Product_cell && from): Pair( std::forward( from)) {}

	//piecewise
	template< class... Args1, class... Args2>
	Product_cell( std::piecewise_construct_t pwc, 
		      std::tuple< Args1...> first_args, 
		       std::tuple<Args2...> second_args):
	Pair( pwc, first_args, second_args) {}

	const Cell1& first_cell() const  { return this->first->first;  }	
	const Cell2& second_cell() const { return this->second->first; }	
	std::size_t  dimension() const	{ return first_cell().dimension() + 
						 second_cell().dimension(); }

	//Assignment
	Self& operator=( const Self & b) { 
		Pair::operator=( b); 
		return *this;
	}

	//Move assignment
	Self& operator=( Self && b) {
		Pair::operator=( std::forward( b));
		return *this;
	}

	//Comparisons.	
	bool operator<( const Self & b) const {
		if (this->first == b.first) { 
			return second_cell() < p.second()_cell();
		}
		return first_cell() < b.first_cell();
	}

	bool operator==( const Self & b) const { 
		return (this->first == b.first()) && (this->second == p.second); 
	}
	bool operator!=( const Self & b) const { 
		return (this->first != b.first()) || (this->second != p.second); 
	}
	
	//File I/O
	template< typename Stream>
	void write( Stream & out) const {
		first_cell().write( out);
		out << " * ";
		second_cell().write( out);
	}

	template< typename Self, typename Coefficient> 
	friend class ctl::Product_boundary< Self, Coefficient>::const_iterator; 
}; //Product_cell
} // namespace ctl

template< typename Stream, typename T>
Stream& operator<<(Stream& out, const ctl::Product_cell< T> & simplex){
	out << i->first << " * " i->second;
	return out;
}

#endif //PRODUCT_CELL_H
