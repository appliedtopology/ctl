#ifndef _CTL_COVER_DATA_H
#define _CTL_COVER_DATA_H
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
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
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

#include <tbb/atomic.h>

namespace ctl{
namespace parallel{ 
class Nerve_data
{
private:
  typedef Nerve_data Self;
  typedef tbb::atomic< size_t> Count_t;
public:
  // default constructor
  Nerve_data( size_t count = 0) 
    : count_(){count_ = count;}
  
  // copy constructor
  Nerve_data( const Nerve_data  &from) : count_( from.count_) {}

  // assignment operator
  Nerve_data& operator=( const Nerve_data& from)
  {
    count_ = from.count_;
    return *this;
  }
  void 		pp()			{ count_++;} 
  Count_t & 	count() 	        { return count_;  }
  const Count_t  count()  const   	{ return count_;  }

private:
  Count_t count_;
}; // class Nerve_data

template< typename Data_>
class Cover_data {
private:
  typedef Cover_data< Data_> Self;
public:
  typedef Data_ Data1;

  // default constructor
  Cover_data( const Data1& data = Data1()) 
    : data_( data){}
  
  // copy constructor
  Cover_data( const Cover_data  &from): data_( from.data_){}

  // assignment operator
  Cover_data& operator=( const Cover_data& from) {
    data_ = from.data_;
    return *this;
  }
 
  Data1&        data()         { return data_; }
  const Data1&   data() const   { return data_; }

private:
  Data_ data_;
}; // class Cover_data

template< typename Data>
struct Get_cover :
public std::unary_function< Data &, 
			typename Data::Data1::value_type::first_type &>
{
	typedef typename Data::Data1 Nerve_iterator;
	typedef typename Nerve_iterator::value_type::first_type Cell;
	Get_cover(){}
	Cell& operator()( Data & data) const{
	  return data.data()->first;
	}

	const Cell& operator()( const Data & data) const{
	  return data.data()->first;
        }
};

template< typename Stream>
Stream& operator<<( Stream & out, const ctl::parallel::Nerve_data & d){
	out << d.count();
	return out;
}
template< typename Stream, typename Data>
Stream& operator<<( Stream & out, const ctl::parallel::Cover_data< Data> & d){
	return out;
}
} // namespace parallel
} //namespace ctl
#endif // _CTL_COVER_DATA_H
