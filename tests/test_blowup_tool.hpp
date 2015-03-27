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
* Copyright (C) Ryan H. Lewis 2010 <me@ryanlewis.net>
*******************************************************************************
*******************************************************************************
* NOTES
*
*
*******************************************************************************/

//STL
#include <sstream>
#include <iostream>
#include <fstream>

//TBB
#include <tbb/task_scheduler_init.h>

//CTL
#include <ctl.hpp>

//Google Test Framework
#include "gtest/gtest.h"

template<typename Blowup, typename Nerve, typename Complex>
void factor_blowup(const Blowup & blowup,
		   const Complex & complex2,
		   const Nerve & complex1){ 
	typedef typename Blowup::const_iterator Blowup_iterator;
	
	Nerve new_complex1;
	Complex new_complex2;
	for (Blowup_iterator product_cell = blowup.begin();
			     product_cell != blowup.end();
				++product_cell){
	 //get the cell without any reference to the blowup complex
	 new_complex1.insert_open_cell(product_cell->first.first->first);
	 new_complex2.insert_open_cell(product_cell->first.second->first);
	}
	ASSERT_EQ(complex1, new_complex1);
	ASSERT_EQ(complex2, new_complex2);
}

TEST(Blowup, Build){
//1) Build a blowup complex
//2) Test a bunch of stuff
}
