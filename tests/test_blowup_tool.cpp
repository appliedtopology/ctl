/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2010 <me@ryanlewis.net>
*******************************************************************************
* BSD-3
*******************************************************************************
* NOTES
*
*
*******************************************************************************/

#include <catch/catch.hpp>
//STL
#include <sstream>
#include <iostream>
#include <fstream>

//TBB
#include <tbb/task_scheduler_init.h>

//CTL
#include <ctl/ctl.hpp>



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
	REQUIRE(complex1== new_complex1);
	REQUIRE(complex2== new_complex2);
}

TEST_CASE("Build", "[blowup]"){
//1) Build a blowup complex
//2) Test a bunch of stuff
}
