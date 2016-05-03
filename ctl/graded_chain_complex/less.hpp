#ifndef CTLIB_FILTRATION_LESS_H
#define CTLIB_FILTRATION_LESS_H
/*******************************************************************************
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
//exported functionality
namespace ctl{

struct Id_less{
      constexpr Id_less(){}
      template< typename Cell_iterator>
      bool operator()( const Cell_iterator & a, const Cell_iterator & b) const {
       return a->second.id() < b->second.id();
      }
}; //struct Id_less

//Order by data, break ties by cell order
struct Cell_less{
      constexpr Cell_less(){}

      template< typename Cell_iterator>
      bool operator()( const Cell_iterator & a, const Cell_iterator & b) const {
	return (a->second < b->second) || 	
		(!(b->second < a->second) && (a->first < b->first));
      }
}; //struct Cell_less

} //namespace ctl

#endif //CTLIB_FILTRATION_LESS_H
