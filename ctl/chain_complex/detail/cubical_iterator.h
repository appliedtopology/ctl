#ifndef CTL_CUBICAL_ITERATOR_H
#define CTL_CUBICAL_ITERATOR_H
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
* modify it under the pairs of the GNU General Public License
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
* This is where some of the genericity comes into play.
* We don't just get a pointer into the complex, we also get its coordinates!
*******************************************************************************
*******************************************************************************/
//STL
#include <iterator>     // std::iterator, std::input_iterator_tag
#include <utility> //std::pair

//non exported functionality 
namespace ctl{
namespace detail{
template< typename Array_, typename Vector_, typename Iterator_> 
class cubical_iterator: 
public std::iterator< std::random_access_iterator_tag, 
		      std::pair< Vector_, Iterator_>,
		      std::ptrdiff_t,
		      const std::pair< Vector_, Iterator_>*,
		      const std::pair< Vector_, Iterator_> >{
  typedef Array_ Array;
  typedef Vector_ Vector;
  typedef Iterator_ Iterator;
  typedef cubical_iterator< Array, Vector, Iterator> Self;
  typedef std::pair< Vector, Iterator> Pair;

public:
	//default
	cubical_iterator() {}
	
	//copy
	cubical_iterator( const Self & i):
	array ( i.array),
	next( i.next),
	pair( i.pair) {}
	
	//move
	cubical_iterator( Self && i):
	array ( std::move( i.array)),
	next ( std::move( i.next)),
	pair( std::move( i.pair)) { i.array = NULL; }

	//begin constructor
	cubical_iterator( Array& _array, Iterator b):
	  array( &_array), next( b), pair() { _next(); }

        //end constructor
	cubical_iterator( Array& _array): 
	array( &_array) { _end_pair(); }

	Self& operator=( const Self& from){
	  array = from.array;
	  next = from.next;
	  pair = from.pair;
	  return *this;
	}

	Self& operator=( const Self&& from){
	  array = std::move( from.array);
	  next =  std::move( from.next);
	  pair =  std::move( from.pair);
	  from.array = NULL;
	  return *this;
	}
        Self& operator+=(std::size_t rhs) {
		next += rhs;
		_next(); 
		return *this;
	}
        Self& operator-=(std::size_t rhs) { 
		next -= rhs; 
		pair.second -= rhs;
		_prev();
		return *this;
        }

    public:
    Self operator+(const Self& rhs) {
	return Self(array, next+rhs.next);
    }
    Self operator-(const Iterator& rhs) {
	return Self(array, next-rhs.next);
    }
    Self operator+(const int& rhs) {
	return Self(array, next+rhs);
    }
    Self operator-(const int& rhs) {
	return Self(array, next-rhs);
    }

    bool operator==( const Self & i) const { return pair.second == i.pair.second;}		
    bool operator!=( const Self & i) const { return pair.second != i.pair.second;}		
    bool operator>( const Self & i) const { return pair.second > i.pair.second; }
    bool operator>=( const Self & i) const { return pair.second >= i.pair.second; }
    bool operator<( const Self & i) const { return pair.second < i.pair.second; }
    bool operator<=( const Self & i) const { return pair.second <= i.pair.second; }

    Self& operator++(){ 
    	_next(); 
    	return *this;
    }
    Self& operator--(){ 
    	_prev(); 
    	return *this;
    }
    Self operator++(int){
    	Self tmp = *this;
    	_next(); 
    	return tmp;
    }
    Self operator--(int){
    	Self tmp = *this;
    	_prev(); 
    	return tmp;
    }

    Pair& operator*(){ return pair; }
    Pair* operator->(){ return &pair; }

protected:
  void _next(){
    if( next != array->end()){
      pair.first = array->iterator_to_coordinate( next, pair.first);
      pair.second = next; 
      ++next;
      return;
    }
    _end_pair();
  }

  void _prev(){
    if( pair.second != array->begin()){
      --pair.second;
      pair.first = array->iterator_to_coordinate( pair.second, pair.first);
      --next;
      return;
    }
    _end_pair();
  }

  void _end_pair(){ pair.second = array->end(); }
  //we use a pointer since references are not default constructible
  Array* array;
  Iterator next;
  Pair pair;
}; //class cubical_iterator
} //detail namespace
} //ctl namespace

#endif //CTL_CUBICAL_ITERATOR
