//STL
#include <iostream>

//Project
#include "abstract_simplex/abstract_simplex.h"
#include "abstract_simplex/simplex_boundary.h"
#include "cell_map.h"

typedef ct::Abstract_simplex< int> Simplex;
typedef ct::Simplex_boundary< Simplex, int> Boundary;
typedef ct::Cell_map< Simplex, Boundary> Complex;
typedef Complex::Cell Cell;
int main( int argc, char** argv){
	Complex c;
	Cell s( {1,2,3,4});
	c.insert_closed_cell( s);
	std::cout << c << std::endl;
	return 0;
}
