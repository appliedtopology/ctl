#ifndef ITERATOR_PRODUCT_CELL_H
#define ITERATOR_PRODUCT_CELL_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
******************************************************************************/
/* NOTES:
* This product cell design is a pair< Cell1, Cell2>
* For space concerns, we represent Cell1 and Cell2 as _iterators_
* - Of course, stylistically, it would be good for Iterator_product_cell, to allow for Cell1/Cell2 to truly be 
    the value type of these iterators
*/
#include <utility> //std::pair, and piecewise_construct
#include <tuple> //std::pair, and piecewise_construct
#include <iostream> //cout (debug only)
#include <ctl/product_cell/iterator_product_boundary.hpp>
#include <ctl/hash/hash.hpp>

namespace ctl {

template< typename Cell_iterator1_, typename Cell_iterator2_>
class Iterator_product_cell : public std::pair< Cell_iterator1_ , Cell_iterator2_> {
	public:	
	typedef Cell_iterator1_ Cell_iterator1;
	typedef Cell_iterator2_ Cell_iterator2;
	typedef typename Cell_iterator1::value_type::first_type Cell1;
	typedef typename Cell_iterator2::value_type::first_type Cell2;
	private:
	typedef std::pair< Cell_iterator1_, Cell_iterator2_> Pair;
	typedef Iterator_product_cell< Cell_iterator1, Cell_iterator2> Self;
	public:

	//default
	Iterator_product_cell() {};

	//copy
	Iterator_product_cell( const Iterator_product_cell & from) : Pair( from) {}	

	//standard
	Iterator_product_cell( const Cell_iterator1 & cell1, 
		      const Cell_iterator2 & cell2): Pair( cell1, cell2) {}
	//initialize
	Iterator_product_cell( Cell_iterator1 && cell1, 
		      Cell_iterator2 && cell2): 
		      Pair( std::forward(cell1), std::forward(cell2)) {}
	//move
	Iterator_product_cell( Iterator_product_cell && from): Pair( std::forward< Pair>( from)){}

	//piecewise
	template< class... Args1, class... Args2>
	Iterator_product_cell( std::piecewise_construct_t pwc, 
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
		return ctl::hash_function( this->first->first) |
			(ctl::hash_function( this->second->first) << shift_factor);
	}
	struct Hash {
  	   std::size_t operator()( const Iterator_product_cell & p) const {
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

}; //Iterator_product_cell

template< typename Stream, typename T1, typename T2>
Stream& operator<<(Stream& out, const Iterator_product_cell< T1, T2> && alpha){
	out << alpha.first_cell() << ctl::otimes 
	    << alpha.second_cell();
	return out;
}
template< typename Stream, typename T1, typename T2>
Stream& operator<<(Stream& out, const Iterator_product_cell< T1, T2> & alpha){
	out << alpha.first_cell() << ctl::otimes 
	    << alpha.second_cell();
	return out;
}

} // namespace ctl

#endif //ITERATOR_PRODUCT_CELL_H
