// clique_output_iterator.h
// Gabe Weaver
// August 20, 2008
//
// Exports Clique_output_iterator class

// Class:  Clique_output_iterator
//
// Simply couts the set representing the clique

#ifndef _CTL_CLIQUE_OUTPUT_ITERATOR_H_
#define _CTL_CLIQUE_OUTPUT_ITERATOR_H_

#include <iostream>
#include <iterator>
#include <cstdlib>
#include <set>

namespace ctl {

template< class Clique_>
class Clique_output_iterator :
  public std::iterator< std::output_iterator_tag,
                        Clique_,
                        void,
                        void,
                        void> {
public:
  Clique_output_iterator() : num_cliques_( 0) {}
  Clique_output_iterator& 
  operator=( const Clique_& clique)
  {
    ++num_cliques_;
    typename Clique_::const_iterator clique_iter;
    for ( clique_iter = clique.begin();
	  clique_iter != clique.end();
	  clique_iter++) {
      std::cout << (*clique_iter) << " ";
    }
    std::cout << std::endl;
    return *this;
  }

  Clique_output_iterator& operator*()       { return *this; }
  Clique_output_iterator& operator++()      { return *this; }
  Clique_output_iterator& operator++( int)  { return *this; }
  int num() { 
    return num_cliques_;
  }
private:
  int num_cliques_;
};

} // namespace ctl

#endif // _CTL_CLIQUE_OUTPUT_ITERATOR_H_
