#ifndef CTLIB_WEIGHT_FUNCTOR_H
#define CTLIB_WEIGHT_FUNCTOR_H
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
*******************************************************************************
* NOTES
*
*
*******************************************************************************/
//STL
#include <unordered_map>

//CTL
#include <ctl/io/io.h>
#include <sstream>

//non-exported functionality
namespace ctl {
namespace detail{} // end namespace detail
} //ctl namespace

//exported functionality
namespace ctl{

template< typename Complex>
struct Weight_data_functor {
    typedef typename Complex::Data Weight_data;
    typedef typename Weight_data::Weight Weight;
    typedef typename Complex::Cell Cell;
    typedef std::unordered_map< std::size_t, std::size_t> Map;
 
    Weight_data_functor& operator=( const Weight_data_functor & f){ 
	values = f.values; 
	return *this; 
    }

    Weight_data_functor& operator=( Weight_data_functor && f){ 
	values = std::move( f.values); 
	return *this; 
    }

    //operator to check if we should read this cell in
    //and it is optimized away
    constexpr bool operator()( const Cell & c, 
			       const std::size_t & id, 
			       bool f) const { 
	return true; 
    } 

    //the "input" operator
    Weight_data& operator()( const Cell & c, Weight_data & d) const { 
	auto i = values.find( d.id());
	assert( i != values.end());
	d.weight() = i->second;
	return d;  
    }
    
    //the output operator 
    const Weight&  operator()( const Weight_data & data) const { 
		return data.weight(); 
    }

    Weight&  operator()( Weight_data & data) const { return data.weight(); }
    
    template< typename Stream>
    Stream& read( Stream & in){
     	std::size_t line_num = 0;
        std::string line;
        std::size_t id=0;
        char the_first_character = in.peek();
        if( the_first_character == 's') {
                ctl::get_line( in, line, line_num);
                std::istringstream ss( line);
                std::string the_word_size;
                ss >> the_word_size;
                std::size_t the_number_of_cells;
                ss >> the_number_of_cells;
                values.reserve( the_number_of_cells);
        }   
        while( ctl::get_line(in, line, line_num)){
                std::istringstream ss( line);
		Weight weight;
		ss >> id; 
                ss >> weight; 
		values.emplace( id, weight);
        }
	return in;
    }
   
    template< typename Stream>
    Stream& write( Stream& out, const Complex & complex){
	out << "size " << complex.size(); 
	for( auto & cell: complex){
		out << cell.second.id() << " " << cell.second.weight() 
		    << std::endl;
	}
	return out;
    }

    private:
    Map values;
}; //end struct Weight_data_functor

template< typename Stream, typename C>
Stream& operator>>(Stream & in, Weight_data_functor< C> & f){ 
	return f.read( in); 
}

} //namespace ctl


#endif //CTLIB_WEIGHT_DATA_FUNCTOR_H
