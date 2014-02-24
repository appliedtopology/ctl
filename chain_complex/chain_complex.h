#ifndef CELL_MAP_H
#define CELL_MAP_H
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
	Data_wrapper(): id_( 0), pos__( 0) {};
	//copy
	Data_wrapper( const Data_wrapper & from) : 
	  id_( from.id_), pos__( from.pos__) {} 
	//move
	Data_wrapper( const Data_wrapper && from): 
		id_( std::move( from.id_)), pos__( std::move( from.pos__)) {} 
	
	std::size_t id() const { return id_; }

	//a bit akward.. probably should change this later.
	std::size_t& pos_() { return pos__; }
	void pos_( const std::size_t p) { pos__ = p; }
	private:
	std::size_t id_;
	std::size_t pos__;
	//(to be read in Millhouse Van Houten's voice)
	//This lets the chain_complex & boundary touch my privates ;)
	template< typename C, typename B, typename D, typename H>
	friend class ctl::Chain_complex;
}; // class Data_wrapper


class Default_data {}; //class Default_data for complex.
template< typename Stream>
Stream& operator<<( Stream & out, const Default_data & d){ return out; }
} //anonymous namespace

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
	Chain_complex(): max_id( 0) {}; 
	//Copy
	Chain_complex( const Chain_complex & b): cells( b.cells), bd( b.bd),
					 max_id( b.max_id) {}; 
	//Move
	Chain_complex( Chain_complex && b): cells( std::move( b.cells)), 
				  bd( std::move( b.bd)), 
				  max_id( std::move(b.max_id)) {} 

	// assignment operator
	Chain_complex& operator=( const Chain_complex& b){
		bd = b.bd;
		max_id = b.max_id;
		cells = b.cells;
		return *this;
	}

	// move assignment operator
	Chain_complex& operator=( const Chain_complex&& b){
		bd = std::move(b.bd);
		max_id = std::move(b.max_id);
		cells = std::move(b.cells);
		return *this;
	}

	Term find_term( const Cell & s) const {
		return Term( cells.find( s), 1);
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
	const std::size_t dimension() const { return max_dim; }
	const std::size_t size() const { return cells.size(); } 
	Boundary& boundary() { return bd; }
	
private:
	Map cells;
	Boundary bd;
	std::size_t max_id;
	std::size_t max_dim;
}; //cell_map
} //namespace ct;

template< typename Stream, typename Cell, 
	  typename Boundary, typename Data, typename Hash>
Stream& operator>>( Stream& in, 
		    ctl::Chain_complex< Cell, Boundary, Data, Hash> & c){ 
	std::size_t line_num = 0;
	std::string line;
	std::size_t id=0;
	while( ctl::get_line(in, line, line_num)){
		std::istringstream ss( line);
		Cell cell;
		ss >> id;
		ss >> cell;
		c.insert_open_cell( cell, Data( id));
	}
	return in;
}

template< typename Stream, typename Cell, typename Boundary, 
	   typename Data, typename Hash>
Stream& operator<<( Stream& out, 
		    const ctl::Chain_complex< Cell, Boundary, Data, Hash> & c){ 
	for(auto i = c.begin(); i != c.end(); ++i){
		std::cout << i->second.id() <<": " << i->first << " --> {" 
		  	  << i->second << "}" << std::endl;
	}
	return out;
}

#endif //CELL_MAP_H
