#ifndef CTLIB_FILTRATION_H
#define CTLIB_FILTRATION_H
#include "filtration/less.h"
//non-exported functionality 
namespace {
//needed in filtration constructor
template< typename Iterator>
class _transform_iterator : 
		public std::iterator< std::input_iterator_tag, Iterator> {
	private:
		typedef _transform_iterator< Iterator> Self;
	public:
	//default
	_transform_iterator() {}
	//copy
	_transform_iterator( Self & f): _i( f._i) {};
	//move
	_transform_iterator( Self && f): _i( std::move(f._i)) {};
	//special
	_transform_iterator( Iterator & i_): _i( i_) {};
	_transform_iterator& operator++(){ _i++; return *this; }
 	Iterator* operator->() { return &(_i); }
 	bool operator!=(const Self& r) const { return (_i != r._i); }
 	bool operator==(const Self& r) const { return !operator!=(r); }
	Self& operator=(const Self& r) { _i = r._i; return *this; } 
	//the details
	Iterator operator*(){ return _i; }
	private:
	Iterator _i;
}; //class _transform_iterator
template< typename Iterator>
inline _transform_iterator< Iterator> _create_transform_iterator ( Iterator i){
 	return _transform_iterator< Iterator>( i);
 }

} //anonymous namespace


namespace ctl{
template< typename Complex_, 
	  typename Less_ = ctl::Cell_less>
class Filtration {
 public:
	typedef  Complex_ Complex;
	typedef  Less_ Less;
 private:
	typedef typename Complex::iterator _Iterator;	
	typedef typename std::vector< _Iterator> Vector;
 public:
	typedef typename Vector::iterator iterator;
	typedef typename Vector::const_iterator const_iterator;

	typedef typename Vector::reverse_iterator riterator;
	typedef typename Vector::const_reverse_iterator const_riterator;
	
	Filtration(): _filtration() {};
	Filtration( const Filtration & f): _filtration( f) {}
	Filtration( const Filtration && f): _filtration( std::move( f)) {}	
	Filtration( _Iterator begin, _Iterator end): 
	_filtration( _create_transform_iterator(begin), 
		      _create_transform_iterator(end)){ 
		Less less;
		std::sort( _filtration.begin(), _filtration.end(), less);
	//	std::size_t count=0;
	//	for (auto cell : _filtration){ cell->second.pos = count++; }
	}

	iterator       begin()       { return _filtration.begin(); }
	iterator         end()       { return _filtration.end();   }
	
	const_iterator begin() const { return _filtration.begin(); }
	const_iterator   end() const { return _filtration.end();   }

	riterator       rbegin()       { return _filtration.rbegin(); }
	riterator         rend()       { return _filtration.rend();   }
	
	const_riterator rbegin() const { return _filtration.rbegin(); }
	const_riterator   rend() const { return _filtration.rend();   }

	const std::size_t size() const { _filtration.size(); } 
 private:
	Vector _filtration;
}; //class Filtration

} //namespace ct
#endif //CTLIB_FILTRATION_H
