#ifndef _CTL_COVER_TEST_H_
#define _CTL_COVER_TEST_H_
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
*******************************************************************************/
#include "gtest/gtest.h"

namespace ctl{
namespace parallel { 
template<typename Complex>
void covered_complex(Complex & complex){
	typedef typename Complex::iterator cell_iterator;
	typedef typename Complex::Cell Cell;
	typedef typename Complex::Data Data;
        for (cell_iterator it = complex.begin(); it != complex.end(); ++it){
                const Cell & cell = it->first;
		Data & data = it->second;
		ASSERT_NEQ(data.data(), Data().data());
        }
}

template<typename Complex>
void sheets_closed(Complex & complex){
        typedef typename Complex::iterator cell_iterator;
        typedef typename Complex::Data Data;
        typedef typename Complex::Cell Cell;
        typedef typename ctl::Complex_boundary< Complex, cell_iterator> 
								Boundary;
	Boundary boundary(complex);
        typedef typename Boundary::const_iterator Boundary_iterator;
	for (cell_iterator i = complex.begin(); i != complex.end(); ++i){
		const Cell & data = i->second.data()->first;	
		for(Boundary_iterator j = boundary.begin( i); 
				      j != boundary.end( i); ++j){
			const Data & face_data = j->get_cell()->second;
			const Cell & face = face_data.data()->first;
			const Cell & face_cell = j->get_cell()->first;
			ASSERT_TRUE(std::includes(face.begin(), face.end(),
				      		  data.begin(), data.end()));
		}
	} 
}

} //namespace parallel
} //namespace ctl

#endif //_CTL_COVER_TEST_H_
