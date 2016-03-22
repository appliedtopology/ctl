#ifndef _CTL_COVER_TEST_H_
#define _CTL_COVER_TEST_H_
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
******************************************************************************/

#include <catch/catch.hpp>
#include <ctl/ctl.hpp>

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
		REQUIRE(data.data() != Data().data());
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
			REQUIRE(std::includes(face.begin(), face.end(),
				      		  data.begin(), data.end())==true);
		}
	} 
}

} //namespace parallel
} //namespace ctl

#endif //_CTL_COVER_TEST_H_
