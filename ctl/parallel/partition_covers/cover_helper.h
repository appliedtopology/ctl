#ifndef _CTL_COVERHELPER_H_
#define _CTL_COVERHELPER_H_
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
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*******************************************************************************
*******************************************************************************/
#include "chain_complex/chain_complex.h"
namespace ctl{
namespace parallel{
//TODO: rewrite this, move it to a new place, useful for parallelism
template<typename Complex, typename Cell, typename PartMap>
bool is_cross_cell(Complex & c,Cell & simplex,PartMap & partMap, 
						int & partitionId){
        typedef typename Complex::iterator cell_it;
        typedef typename Cell::const_iterator const_cell_elt_it;
        partitionId = partMap[*(simplex.begin())];
	for (const_cell_elt_it j = simplex.begin(); j != simplex.end(); ++j){
                if (partMap[*j] != partitionId){
                        partitionId = -1;
                        return true;
                }
        }
        return false;
}

template<typename Complex>
void print_cover(Complex &cover){
	typedef typename Complex::iterator cell_it;
	typedef typename ctl::Cell_less< cell_it > Cell_order;
	typedef typename ctl::Filtration< Complex, Cell_order > Filtration;
	typedef typename Filtration::iterator Filtration_iterator;

        std::cout << "Cover: " << std::endl;
        Filtration filtered_complex(cover);
        for (Filtration_iterator it = filtered_complex.begin();
                                        it != filtered_complex.end(); ++it){
                std::cout << (*it)->first << " ---> {";
                std::cout << (*it)->second.first_set;
                if ((*it)->second.second_set != -1){
                        std::cout << "," << (*it)->second.second_set;
                }
                std::cout << "}";
                std::cout << std::endl;
        }
}

template<typename Complex>
void print_cover(Complex & cover, unsigned int limit){
	if (cover.size() <= limit){
		print_cover(cover);
	}
}
template<typename Complex>
void failure(Complex & complex){
        print_cover(complex);
        std::exit( -1);
}
} //end namespace parallel
} //end namespace ctl
#endif
