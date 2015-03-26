#ifndef CTL_DATA_WRAPPER_H
#define CTL_DATA_WRAPPER_H
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
* NOTES:
* We use this to associate a single number to every cell opaquely.
*******************************************************************************/

namespace ctl {
namespace detail {

struct Default_data {
constexpr bool operator==( const Default_data & d) const{ return true; }
constexpr bool operator<( const Default_data & d) const { return false; }
}; //class Default_data for complex.

//template< typename Data1_, typename Data2_=Default_data>
//class Data_wrapper : public Data1_ , public Data2_ {
//   private:
//   typedef Data_wrapper< Data1_, Data2_> Self;
//   public:
//   //default
//   Data_wrapper(): Data1_(), Data2_(){}
//   //copy
//   Data_wrapper( const Data_wrapper & from) : Data1_( from), Data2_( from){} 
//   //move
//   Data_wrapper( const Data_wrapper && from): Data1_( std::forward( from)), 
//					      Data2_( std::forward( from)){}
//
//   Self& operator=( const Self & from){
//   	Data1_::operator=( from);
//   	Data2_::operator=( from);
//   	return *this;
//   }
//
//   Self& operator=( Self && from){
//   	Data1_::operator=( from);
//   	Data2_::operator=( from);
//   	return *this;
//   }
//
//   bool operator==( const Self & b) const {
//	return     Data1_::operator==( b)
//		&& Data2_::operator==( b);
//   }
//   bool operator!= (const Self & b) const { return !((*this)==b); }
//}; // class Data_wrapper< D1, D2> 

template< typename Data_>
//class Data_wrapper< Data_, Default_data> : public Data_ {
class Data_wrapper : public Data_ {
   private:
  // typedef Data_wrapper< Data_, Default_data> Self;
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
   Data_wrapper( const Data_wrapper && from): Data_( std::forward( from)),
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
