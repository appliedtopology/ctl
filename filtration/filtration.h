#ifndef CTLIB_FILTRATION_H
#define CTLIB_FILTRATION_H

//exported functionality
namespace ct{

template< typename _Complex, typename _Less>
class Filtration {
 public:
	typedef typename _Complex Complex;
	typedef typename _Less Less;
 private:
	typedef typename Complex::iterator _Iterator;	
	typedef typename std::vector< _Iterator> Vector;
 public:
	typedef typename Vector::iterator iterator;
	typedef typename Vector::const_iterator const_iterator;

	typedef typename Vector::riterator riterator;
	typedef typename Vector::const_riterator const_riterator;
	
	Filtration(): _filtration() {};
	Filtration( const Filtration & f): _filtration( f) {}
	Filtration( const Filtration && f): _filtration( std::move( f)) {}	
	Filtration( const _Iterator & begin, const _Iterator & end):
	_filtration( begin, end){ 
		std::sort( _filtration.begin(), _filtration.end());
	}

	iterator       begin() const { return _filtration.begin(); }
	const_iterator begin() const { return _filtration.begin(); }
	iterator         end() const { return _filtration.end();   }
	const_iterator   end() const { return _filtration.end();   }

	riterator       rbegin() const { return _filtration.rbegin(); }
	const_riterator rbegin() const { return _filtration.rbegin(); }
	riterator         rend() const { return _filtration.rend();   }
	const_riterator   rend() const { return _filtration.rend();   }

	const std::size_t size() const { _filtration.size(); } 
 private:
	Vector _filtration;
}

} //namespace ct

//non-exported functionality
namespace {} //anonymous namespace

#endif //CTLIB_FILTRATION_H
