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
* ********** BSD-3 License ****************
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software without 
* specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
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
