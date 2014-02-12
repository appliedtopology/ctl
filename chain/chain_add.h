#ifndef CTLIB_CHAIN_ADD_H
#define CTLIB_CHAIN_ADD_H

//exported functionality
namespace ctl{

template< typename Chain>
Chain& chain_add( Chain & a, Chain & b){ 

}

template< typename Chain>
Chain& chain_add( Chain & a, Term & b){ return ctl::chain_add( a, Chain( b)); }

}
#endif //CTLIB_CHAIN_ADD_H
