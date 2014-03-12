#ifndef CTLIB_CELL_MAP_H
#define CTLIB_CELL_MAP_H
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
#include <unordered_map>
#include <sstream>

#include "hash/hash.h"
#include "io/io.h"
#include "abstract_simplex/abstract_simplex.h"

//forward declaration
namespace ctl{
template< typename Cell, typename Boundary, typename Data, typename Hash> 
class Chain_complex;
} //namespace ct 

//non-exported functionality 
namespace {

template< typename Data_>
class Data_wrapper : public Data_ {
	private:
	typedef Data_wrapper< Data_> Self;
	public:
	//default
	Data_wrapper(): id_( 0), pos_( 0) {}
	//copy
	Data_wrapper( const std::size_t & tid, const std::size_t p=0): 
	Data_(), id_( tid), pos_( p) {}
 
	Data_wrapper( const Data_wrapper & from) : 
	  id_( from.id_), pos_( from.pos_) {} 
	//move
	Data_wrapper( const Data_wrapper && from): 
		id_( std::move( from.id_)), pos_( std::move( from.pos_)) {} 
	
	Self& operator=( const Self & from){
		Data_::operator=( from);
		id_ = from.id_;
		pos_ = from.pos_;
		return *this;
	}
	Self& operator=( const Self && from){
		Data_::operator=( from);
		id_ = std::move( from.id_);
		pos_ = std::move( from.pos_);
		return *this;
	}


	std::size_t id() const { return id_; }
	//a bit akward.. probably should change this later.
	std::size_t pos() const { return pos_; }
	void pos( const std::size_t p) { pos_= p; }
	private:
	std::size_t id_;
	std::size_t pos_;
	//(to be read in Millhouse Van Houten's voice)
	//This lets the chain_complex & boundary touch my privates ;)
	template< typename C, typename B, typename D, typename H>
	friend class ctl::Chain_complex;
}; // class Data_wrapper

struct Default_data {}; //class Default_data for complex.
} //anonymous namespace

template< typename Stream>
Stream& operator<<( Stream & out, const Default_data & d){ return out; }

//exported functionality
namespace ctl{

template< typename Cell_, typename Boundary_, 
	  typename Data_ = Default_data, typename Hash_ = ctl::Hash< Cell_> >
class Chain_complex{
public:
	typedef Cell_ Cell; //Describes a fundamental object, 
			    //e.g. simplex, cube, etc
	typedef Boundary_ Boundary; //Describes how to take its boundary
	typedef Data_wrapper< Data_> Data; //Arbitrary data associated to space.
	typedef Hash_ Hash;
private:
	typedef Chain_complex< Cell, Boundary> Self;
	typedef std::unordered_map< Cell, Data, Hash> Map;
	typedef typename Boundary::Coefficient _Coefficient;
public: 
	typedef typename Map::size_type size_type;
	typedef typename Map::iterator iterator;
	typedef typename Map::const_iterator const_iterator;
	typedef typename Map::value_type value_type;

	//A term in the chain complex is a pointer to a cell, with a coefficient
	typedef typename ctl::Term< iterator,_Coefficient> Term;
public:
	//Constructors
	//Default
	Chain_complex(): max_id( 0), max_dim( 0) {}; 
	//Copy
	Chain_complex( const Chain_complex & b): cells( b.cells), bd( b.bd),
					 max_id( b.max_id), max_dim( b.max_dim) {}; 
	//Move
	Chain_complex( Chain_complex && b): cells( std::move( b.cells)), 
				  bd( std::move( b.bd)), 
				  max_id( std::move(b.max_id)), 
				  max_dim( std::move( b.max_dim)) {} 

	// assignment operator
	Chain_complex& operator=( const Chain_complex& b){
		bd = b.bd;
		max_id = b.max_id;
		max_dim = b.max_dim;
		cells = b.cells;
		return *this;
	}

	// move assignment operator
	Chain_complex& operator=( const Chain_complex&& b){
		bd = std::move(b.bd);
		max_id = std::move(b.max_id);
		max_dim = std::move(b.max_dim);
		cells = std::move(b.cells);
		return *this;
	}

 	iterator find_cell( const Cell & s) { 
		return cells.find( s); 
	}
	
 	const_iterator find_cell( const Cell & s) const { 
		return cells.find( s); 
	}

	iterator       begin()       { return cells.begin(); }
	iterator         end()       { return cells.end();   }
	
	const_iterator begin() const { return cells.begin(); }
	const_iterator   end() const { return cells.end();   }

	std::pair< iterator, bool> insert_open_cell( const Cell & s, 
						     const Data& data=Data()){
	  std::pair< iterator, bool> c =  cells.emplace( s, data);
	  if( c.second) { //this outer if is probably unnecessary
	    max_dim = std::max( max_dim, s.dimension());
	    if( c.first->second.id_ == 0){ 
	     c.first->second.id_ = ++max_id;
	    } else{ 
	     max_id=std::max( max_id, c.first->second.id_);
	    }
	  }
	  return c;	
	}

	std::pair< iterator, std::size_t> 
	insert_closed_cell( const Cell & s, const bool closed=false, 
			    const Data&  data = Data()){
		typedef typename std::pair< iterator, std::size_t> Pair;
		iterator iter = cells.find( s);	
		std::size_t num_faces_inserted=0;
		//if cell exists, and we assume 
		//we are closed then we are done.
		if( closed && iter != cells.end()){ 
		 return std::make_pair( iter, num_faces_inserted); 
		}
		
		//first you add the boundary
		Data face_data( data);
		face_data.id_ = 0;
		for( auto face = bd.begin( s); face != bd.end( s); ++face){
		 const Pair & p = insert_closed_cell( face->cell(), 
						      closed, face_data);
		 num_faces_inserted+=p.second;	
		}

		//ugly, but then you add yourself.	
		const std::pair< iterator, bool> p(insert_open_cell( s, data));
		return std::make_pair( p.first, p.second+num_faces_inserted);
	}
	template< typename Stream>
	void write( Stream& out) const {
		for( auto cell: cells){
		  out << cell.second.id() << " ";
		  cell.first.write( out);
		  out << std::endl; 
		}
	}
	void reserve( const std::size_t n) { cells.reserve( n); }
	const std::size_t dimension() const { return max_dim; }
	const std::size_t size() const { return cells.size(); } 
	Boundary& boundary() { return bd; }
	bool is_closed() const{ 
		for( auto sigma : cells){
			for( auto tau = bd.begin( sigma.first); 
				  tau != bd.end( sigma.first); ++tau){
				if( find_cell( tau->cell()) == end()){ 
					return false; 
				}			
			}
		}
		return true;
	}
private:
	Map cells;
	Boundary bd;
	std::size_t max_id;
	std::size_t max_dim;
}; //cell_map
} //namespace ct;

#endif //CTL_CHAIN_COMPLEX_MAP_H
