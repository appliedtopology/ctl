#ifndef CTLIB_LP_H
#define CTLIB_LP_H

//exported functionality
namespace ct{

template< typename Point, int p=2>
value_type lp( const Point & a, const Point & b) {
	typedef typename Point::const_iterator iterator;
	value_type dist=0;
	for( auto i = a.begin(), 
	     auto j = b.begin(); i != a.end(); ++i, ++j){
		dist += std::pow((*i)-(*j),p);	
	}
	return dist;
}

template< typename Point>
value_type hamming( const Point & a, const Point & b) {
	typedef typename Point::const_iterator iterator;
	value_type dist=0;
	for( auto i = a.begin(), 
	     auto j = b.begin(); i != a.end(); ++i, ++j){
		dist += (*i==*j);
	}
	return dist;
}

} //namespace ct

//non-exported functionality
namespace {} //anonymous namespace

#endif //CTLIB_LP_H
