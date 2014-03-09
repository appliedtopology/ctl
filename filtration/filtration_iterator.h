#ifndef CTLIB_FILTRATION_ITERATOR_H
#define CTLIB_FILTRATION_ITERATOR_H

//private functionality 
//the multiplier gracefully handles reverse iterators.
namespace _ctl{
template< typename Iterator, int multiplier>
class _filtration_iterator : 
		public std::iterator< std::random_access_iterator_tag, 
					typename Iterator::value_type> {
	public:
		
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef std::random_access_iterator_tag iterator_category;
	private:
		typedef typename Iterator::difference_type diff_type;
		typedef _filtration_iterator< Iterator, multiplier> Self;
	public:
		
	//default
	_filtration_iterator(): i(), p( 0) {}
	//copy
	_filtration_iterator( Self & f): i( f.i), p( f.p) {};
	//move
	_filtration_iterator( Self && f): i( std::move( f.i)),
					  p( std::move(f.p)) {};
	//special
	_filtration_iterator( const Iterator & i_, 
			      const std::size_t p_): i( i_), p( p_) {}    
	Self& operator++(){ 
		p+=multiplier; i++;	
		return *this; 
	}
	Self operator++(int){ 
		Self r( *this);
		++(*this);	
		return r; 
	}
	Self& operator--(){ 
		p-=multiplier; i--;	
		return *this; 
	}
	Self operator--(int){ 
		Self r( *this);
		--(*this);	
		return r; 
	}
	//deref
	pointer operator->() const { return &(*i); }
	reference operator*(){ return *i; }
	//comparisons
 	bool operator!=(const Self& r) const { return (i != r.i); }
 	bool operator==(const Self& r) const { return !operator!=(r); }
 	bool operator<(const Self& r) const { return (i < r.i); }
 	bool operator<=(const Self& r) const { return (i <= r.i); }
 	bool operator>(const Self& r) const { return (i > r.i); }
 	bool operator>=(const Self& r) const { return (i >= r.i); }
	//arithmetic
 	Self operator+(const diff_type n) const { return Self(i+n, p+n); }
	Self operator-(const diff_type n) const { return Self(i-n, p-n); }
	Self& operator=(const Self& r) { i = r.i; p = r.p; return *this; } 
 	Self& operator+=(const difference_type n) { i+=n; p+=n; return *this;}
 	Self& operator-=(const difference_type n) { i-=n; p-=n; return *this;}
	reference operator[](const std::size_t n) const { return *(i+n); } 	
	std::size_t pos() const { return p; } 
	//the details
	private: 
	Iterator i;
	std::size_t p;
}; //class _filtration_iterator
} //namespace ctl
#endif //CTLIB_FILTRATION_ITERATOR_H