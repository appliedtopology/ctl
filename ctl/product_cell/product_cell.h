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
*******************************************************************************
* ********** BSD-3 License ****************
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software without 
* specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
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
#include <ctl/product_cell/product_boundary.h>
namespace ctl {

template< typename Cell_iterator1_, typename Cell_iterator2_>
class Product_cell : public std::pair< Cell_iterator1_ , Cell_iterator2_> {
	public:	
	typedef Cell_iterator1_ Cell_iterator1;
	typedef Cell_iterator2_ Cell_iterator2;
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
	Product_cell( Product_cell && from): Pair( std::forward< Pair>( from)){}

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
			return second_cell() < b.second_cell();
		}
		return first_cell() < b.first_cell();
	}

	bool operator==( const Self & b) const { 
		return (this->first == b.first) && (this->second == b.second); 
	}
	bool operator!=( const Self & b) const { 
		return (this->first != b.first) || (this->second != b.second); 
	}
	std::size_t hash() const{
		constexpr std::size_t shift_factor = sizeof(std::size_t) >> 2;
		return this->second->second.id() | 
			(this->first->second.id() << shift_factor);
	}
	struct Hash {
  	   std::size_t operator()( const Product_cell & p) const {
  		  return p.hash();
  	   }
	};
	//File I/O
	template< typename Stream>
	void write( Stream & out) const {
		first_cell().write( out);
		out << ctl::otimes;
		second_cell().write( out);
	}

}; //Product_cell

template< typename Stream, typename T1, typename T2>
Stream& operator<<(Stream& out, const Product_cell< T1, T2> && alpha){
	out << alpha.first_cell() << ctl::otimes 
	    << alpha.second_cell();
	return out;
}
} // namespace ctl

#endif //PRODUCT_CELL_H
