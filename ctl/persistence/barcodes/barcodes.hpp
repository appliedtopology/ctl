#ifndef CTLIB_BARCODE_H
#define CTLIB_BARCODE_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
******************************************************************************
* NOTES
* We use the type alias in C++11. This is ok for the definitions, but, we might
* be muddying the waters by defining operator<< and operator>> on them.
* on the other hand it doesn't seem like this should really cause an issue here.
*******************************************************************************/
//STL
#include <iostream>
#include <set>
#include <vector>
#include <utility>
#include <functional>
#include <ctl/hash/hash.hpp>

//exported functionality
namespace ctl{
using Barcodes = std::vector< 
std::multiset< std::pair< double, double> >> ;
} //namespace ctl

#endif //CTLIB_BARCODES_H
