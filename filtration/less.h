#ifndef CTLIB_FILTRATION_LESS_H
#define CTLIB_FILTRATION_LESS_H

//exported functionality
namespace ctl{

struct Id_less{ 
      template< typename Cell_iterator>
      bool operator()( const Cell_iterator & a, const Cell_iterator & b) const {
       return a->second.id() < b->second.id();
      }
}; //struct Id_less

struct Cell_less{
      template< typename Cell_iterator>
      bool operator()( const Cell_iterator & a, const Cell_iterator & b) const {
	return a->first < b->first;
      }
}; //struct Cell_less

} //namespace ctl

//non-exported functionality
namespace {} //anonymous namespace

#endif //CTLIB_FILTRATION_LESS_H
