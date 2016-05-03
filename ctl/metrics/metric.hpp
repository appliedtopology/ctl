#ifndef CTLIB_LP_H
#define CTLIB_LP_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
//exported functionality
namespace ctl{

template< typename Point, int p=2>
double lp( const Point & a, const Point & b) {
	//typedef typename Point::const_iterator iterator;
	double dist=0;
	auto i = a.begin(); 
	for(  auto j = b.begin(); i != a.end(); ++i, ++j){
		dist += std::pow((*i)-(*j),p);	
	}
	return dist;
}

template< typename Point>
double hamming( const Point & a, const Point & b) {
	//typedef typename Point::const_iterator iterator;
	double dist=0;
	auto i = a.begin();
	for( auto j = b.begin(); i != a.end(); ++i, ++j){
		dist += (*i==*j);
	}
	return dist;
}

} //namespace ctl

#endif //CTLIB_LP_H
