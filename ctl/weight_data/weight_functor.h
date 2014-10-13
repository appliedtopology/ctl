#ifndef CTLIB_WEIGHT_FUNCTOR_H
#define CTLIB_WEIGHT_FUNCTOR_H
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
//STL
#include <unordered_map>

//CTL
#include <ctl/io/io.h>
#include <sstream>

//non-exported functionality
namespace ctl {
namespace detail{} // end namespace detail
} //ctl namespace

//exported functionality
namespace ctl{

template< typename Complex>
struct Weight_data_functor {
    typedef typename Complex::Data Weight_data;
    typedef typename Weight_data::Weight Weight;
    typedef 	     Weight result_type;
    typedef typename Complex::Cell Cell;
 
    Weight_data_functor& operator=( const Weight_data_functor & f){ 
	return *this; 
    }

    Weight_data_functor& operator=( Weight_data_functor && f){ 
	return *this; 
    }

    //the output operator 
    const Weight&  operator()( const Weight_data & data) const { 
		return data.weight(); 
    }

    Weight&  operator()( Weight_data & data) const { return data.weight(); }
}; //end struct Weight_data_functor

} //namespace ctl

#endif //CTLIB_WEIGHT_DATA_FUNCTOR_H
