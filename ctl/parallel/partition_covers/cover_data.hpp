#ifndef _CTL_COVER_DATA_H
#define _CTL_COVER_DATA_H
/*******************************************************************************
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
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
  //dont enforce anything extra on order
  inline const bool operator<( const Self & f) const { return false; }

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

  //dont enforce covering relation..
  constexpr bool operator<( const Self & f) const { return false; }

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
template< typename Stream>
Stream& operator<<( Stream & out, ctl::parallel::Nerve_data && d){
	out << d;
	return out;
}



template< typename Stream, typename Data>
Stream& operator<<( Stream & out, const ctl::parallel::Cover_data< Data> & d){
	return out;
}
template< typename Stream, typename Data>
Stream& operator<<( Stream & out, ctl::parallel::Cover_data< Data> && d){
	out << d;
	return out;
}


} // namespace parallel
} //namespace ctl
#endif // _CTL_COVER_DATA_H
