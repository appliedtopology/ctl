#ifndef CELL_MAP_H
#define CELL_MAP_H
#include <unordered_map>
#include <sstream>

#include "hash/hash.h"
#include "io/io.h"
#include "abstract_simplex/abstract_simplex.h"

//forward declaration
namespace ct{
template< typename C, typename B, typename D, typename H> class Cell_map;
} //namespace ct 

//non-exported functionality 
namespace {

template< typename Data_>
class Data_with_id : public Data_ {
	private:
	typedef Data_with_id< Data_> Self;
	public:
	//default
	Data_with_id(): id_( 0) {};
	//copy
	Data_with_id( const Data_with_id & from) : id_( from.id_) {} 
	//move
	Data_with_id( const Data_with_id && from): id_( std::move( id_)) {} 
	
	std::size_t id() const { return id_; }
	private:
	std::size_t id_;
	//(to be read in Millhouse Van Houten's voice)
	//This lets the cell_map touch my privates ;)
	template< typename C, typename B, typename D, typename H>
	friend class ct::Cell_map;
}; // class Data_with_id


class Default_data {}; //class Default_data for complex.
template< typename Stream>
Stream& operator<<( Stream & out, const Default_data & d){ return out; }
} //anonymous namespace

//exported functionality
namespace ct{
template< typename Cell_, typename Boundary_, 
	  typename Data_ = Default_data, typename Hash_ = ct::Hash< Cell_> >
class Cell_map{
public:
	typedef Cell_ Cell; //Describes a fundamental object, 
			    //e.g. simplex, cube, etc
	typedef Boundary_ Boundary; //Describes how to take its boundary
	typedef Data_with_id< Data_> Data; //Arbitrary data associated to space.
	typedef Hash_ Hash;
private:
	typedef Cell_map< Cell, Boundary> Self;
	typedef std::unordered_map< Cell, Data, Hash> Map;
public: 
	typedef typename Map::iterator iterator;
	typedef typename Map::const_iterator const_iterator;
	typedef typename Map::value_type value_type;
public:
	//Constructors
	//Default
	Cell_map() {}; 
	//Copy
	Cell_map( const Cell_map & b): cells( b.cells), bd( b.bd),
					 max_id( b.max_id) {}; 
	//Move
	Cell_map( Cell_map && b): cells( std::move( b.cells)), 
				  bd( std::move( b.bd)), 
				  max_id( std::move(b.max_id)) {} 

	// assignment operator
	Cell_map& operator=( const Cell_map& b){
		bd = b.bd;
		max_id = b.max_id;
		cells = b.cells;
		return *this;
	}

	// move assignment operator
	Cell_map& operator=( const Cell_map&& b){
		bd = std::move(b.bd);
		max_id = std::move(b.max_id);
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
		//we have are closed then we are done.
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
	Boundary& boundary() { return bd; }
	
private:
	Map cells;
	Boundary bd;
	std::size_t max_id;
	std::size_t max_dim;
}; //cell_map
} //namespace ct

template< typename Stream, typename Cell, 
	  typename Boundary, typename Data, typename Hash>
Stream& operator>>( Stream& in, ct::Cell_map< Cell, Boundary, Data, Hash> & c){ 
	std::size_t line_num = 0;
	std::string line;
	std::size_t id=0;
	while( ct::get_line(in, line, line_num)){
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
		    const ct::Cell_map< Cell, Boundary, Data, Hash> & c){ 
	for(auto i = c.begin(); i != c.end(); ++i){
		std::cout << i->second.id() <<": " << i->first << " --> {" 
			  << i->second << "}" << std::endl;
	}
	return out;
}

#endif //CELL_MAP_H
