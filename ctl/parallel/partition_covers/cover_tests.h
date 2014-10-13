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
*******************************************************************************
*******************************************************************************/
namespace ctl{
namespace parallel { 
template<typename Complex>
bool covered_complex(Complex & complex){
	typedef typename Complex::iterator cell_iterator;
	typedef typename Complex::Cell Cell;
	typedef typename Complex::Data Data;
        for (cell_iterator it = complex.begin(); it != complex.end(); ++it){
                const Cell & cell = it->first;
		Data & data = it->second;
                if (data.data() == Data().data()){
                        std::cerr << "Cover failed to cover cell "
                                  << cell
                                  << " of dimension "
                                  << cell.dimension()
                                  << std::endl;
                        return false;
                }
        }
        return true;
}

template<typename Complex>
bool sheets_closed(Complex & complex){
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
			bool result = std::includes(face.begin(),
				      face.end(),
				      data.begin(), data.end());
			if(!result){
				std::cerr << "Cell: " << i->first 
					  << " has data " << data
				          << std::endl;

				std::cerr << "Face " << face_cell
                                          << " has data " 
					  << face
                                          << std::endl;
				return false;
			}
		}
	} 
	return true;
}

template<typename Complex>
void run_tests(Complex & complex){
	if(!covered_complex(complex)){
		std::exit( -1);	
	}else{
		std::cout << "Test Passed! Cover is a cover!" << std::endl;
	}
	if (!sheets_closed(complex)){
		std::exit( -1);	
	}else{
		std::cout << "Test Passed! Covers are closed!" << std::endl;
	}
}

template<typename Complex, typename Blowup, typename Cover_complex>
void run_tests(Complex & complex, Blowup & blowup, Cover_complex & nerve){
	run_tests(complex);
	if(!factor_blowup(blowup,nerve,complex)){
		std::exit( -1);
	}else{
		std::cout << "Test Passed! Blowup Factors" << std::endl;
	}
}

template<typename Blowup, typename Cover_complex, typename Complex>
bool factor_blowup(const Blowup & blowup,
		   const Cover_complex & complex1, 
		   const Complex & complex2){
	//typedef typename Blowup::Cell_boundary Blowup_boundary;
	//typedef typename Blowup::Cell Blowup_cell;
	typedef typename Blowup::const_iterator Blowup_iterator;
	//typedef typename Blowup_boundary::const_iterator 
	//					Blowup_boundary_iterator;
	
	Cover_complex new_complex1;
	Complex new_complex2;
	for (Blowup_iterator product_cell = blowup.begin();
			     product_cell != blowup.end();
				++product_cell){
		//get the cell without any reference to the blowup complex
		new_complex1.insert_open_cell(
				product_cell->first.first->first);
		new_complex2.insert_open_cell(
				product_cell->first.second->first);
	}
	return (complex1 == new_complex1 && complex2 == new_complex2);
}

} //namespace parallel
} //namespace ctl

#endif //_CTL_COVER_TEST_H_
