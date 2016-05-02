#ifndef PRODUCT_CELL_H
#define PRODUCT_CELL_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
******************************************************************************/
/* NOTES:
* This product cell design is a pair< Cell1, Cell2>
* This product_cell stores explicit factor cells, if space is a concern, consider Iterator_product_cell< Cell1, Cell2>
*/
#include <utility> //std::pair, and piecewise_construct
#include <tuple> //std::pair, and piecewise_construct
#include <iostream> //cout (debug only)
#include <ctl/product_cell/product_boundary.hpp>
#include <ctl/hash/hash.hpp>
#include <ctl/io/io.hpp>

namespace ctl {

template< typename Cell1_, typename Cell2_>
class Product_cell : public std::pair< Cell1_ , Cell2_> {
	public:	
	typedef Cell1_ Cell1;
	typedef Cell2_ Cell2;
	private:
	typedef std::pair< Cell1, Cell2> Pair;
	typedef Product_cell< Cell1, Cell2> Self;
	public:

	//default
	Product_cell() : Pair() {};

	//copy
	Product_cell( const Product_cell & from) : Pair( from) { }

	//standard
	Product_cell( const Cell1 & cell1, 
		      const Cell2 & cell2): Pair( cell1, cell2) {}
	//initialize
	Product_cell( Cell1 && cell1, Cell2 && cell2): Pair( std::forward(cell1), std::forward(cell2)) {}
	//move
	Product_cell( Product_cell && from): Pair( std::forward< Pair>( from)){}

	//piecewise
	template< class... Args1, class... Args2>
	Product_cell( std::piecewise_construct_t pwc, 
		      std::tuple< Args1...> first_args, 
		       std::tuple<Args2...> second_args):
	Pair( pwc, first_args, second_args) {}

	const Cell1& first_cell() const  { return this->first;  }	
	const Cell2& second_cell() const { return this->second; }	
	std::size_t  dimension() const	{ return first_cell().dimension() + second_cell().dimension(); }

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
	
	bool operator==( const Self & b) const { return (this->first == b.first) && (this->second == b.second); }
	bool operator!=( const Self & b) const { return (this->first != b.first) || (this->second != b.second); }

	std::size_t hash() const{
		constexpr std::size_t shift_factor = sizeof(std::size_t) >> 2;
		return ctl::hash_function( first_cell()) |
			(ctl::hash_function( second_cell()) << shift_factor);
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

template< typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const Product_cell< T1, T2> && alpha){
	out << alpha.first_cell() << ctl::otimes 
	    << alpha.second_cell();
	return out;
}
template< typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const Product_cell< T1, T2> & alpha){
	out << alpha.first_cell() << ctl::otimes 
	    << alpha.second_cell();
	return out;
}

} // namespace ctl

#endif //PRODUCT_CELL_H
