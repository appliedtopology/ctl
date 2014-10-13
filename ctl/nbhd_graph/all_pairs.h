#ifndef CTLIB_PAIRWISE_DISTANCE_H
#define CTLIB_PAIRWISE_DISTANCE_H
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
* \cite{hatcher, z-fcv-10a}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
*******************************************************************************/
//NOTE: syntax in this file is C++1x style.

//exported functionality
namespace ctl{

template< typename Point_iterator, 
	  typename Output_iterator,
	  typename Coordinate, 
	  typename Distance_metric>
Output_iterator all_pairs( Points_iterator begin, Points_iterator end, 
			   Output_iterator out, const Coordinate epsilon,
			   const Distance_metric & distance){
	for( auto p1 = begin; p1 != end; ++p1){
		for( auto p2 = p1+1; p2 != end; ++p2){
		   const auto d = distance( p1, p2);
		   if (d <= epsilon) { *out++ = std::make_tuple( p1, p2, d); }
		} 
	}	
	return out;
} 

} //namespace ctl


#endif //CTLIB_PAIRWISE_DISTANCE_H
