#ifndef CTL_DATA_WRAPPER_H
#define CTL_DATA_WRAPPER_H
#include <utility>
/*******************************************************************************
* BSD-3
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* NOTES:
* We use this to associate a single number to every cell opaquely.
*******************************************************************************/

namespace ctl {
namespace detail {

struct Default_data {
constexpr bool operator==( const Default_data & d) const{ return true; }
constexpr bool operator<( const Default_data & d) const { return false; }
}; //class Default_data for complex.

template< typename Data_>
class Data_wrapper : public Data_ {
   private:
   typedef Data_wrapper< Data_> Self;
   public:
   typedef std::size_t Id;
   //default
   Data_wrapper(): Data_(), id_( 0) {}
   //id 
   Data_wrapper( const Id & tid): Data_(), id_( tid){}
   //copy
   Data_wrapper( const Data_wrapper & from) : Data_( from), id_( from.id_){}
   //move
   Data_wrapper( Data_wrapper && from): Data_( std::forward<Data_wrapper>( from)),
   	id_( std::move( from.id_)){
   }

   Self& operator=( const Self & from){
   	Data_::operator=( from);
   	id_ = from.id_;
   	return *this;
   }

   Self& operator=( Self && from){
   	Data_::operator=( from);
   	id_ = std::move( from.id_);
   	return *this;
   }

   bool operator<( const Self & from) const{
	return Data_::operator<( from);
   }

   bool operator==( const Self & b) const {
	return (id_ == b.id_) && Data_::operator==( b);
   }
   bool operator!= (const Self & b) const { return !((*this)==b); }
   Id id() const { return id_; }
   void id( Id n){ id_ = n; }
   private:
   Id id_;
}; // class Data_wrapper

template< typename Stream>
Stream& operator<<( Stream & out, const Default_data & d){ return out; }
template< typename Stream>
Stream& operator<<( Stream & out, const Default_data && d){ return out; }
} //namespace detail

} //namespace ctl

#endif //CTL_DEFAULT_WRAPPER
