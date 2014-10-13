#ifndef CTLIB_FILTRATION_LESS_H
#define CTLIB_FILTRATION_LESS_H
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

struct Cell_less{
      constexpr Cell_less(){}
      template< typename Cell_iterator>
      bool operator()( const Cell_iterator & a, const Cell_iterator & b) const {
	return a->first < b->first;
      }
}; //struct Cell_less

} //namespace ctl

//non-exported functionality
namespace {} //anonymous namespace

#endif //CTLIB_FILTRATION_LESS_H
