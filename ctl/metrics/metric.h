#ifndef CTLIB_LP_H
#define CTLIB_LP_H
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
*******************************************************************************
*******************************************************************************
* NOTES
*
*
*******************************************************************************/
/
//exported functionality
namespace ctl{

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

} //namespace ctl

//non-exported functionality
namespace {} //anonymous namespace

#endif //CTLIB_LP_H
