//#include "term.h"
#include "abstract_simplex.h"
#include <iostream>
#include "simplex_boundary.h"

//#include "simplex_boundary.h"

typedef ct::Abstract_simplex< std::size_t> Simplex;
int main( int argc, char** argv){
	Simplex s;
	s.insert( 2);
	std::cout << s << std::flush << std::endl;
	s.insert( 1);
	std::cout << s << std::flush << std::endl;
	s.insert( 4);
	std::cout << s << std::flush << std::endl;
	s.insert( 3);
	std::cout << s << std::flush << std::endl;
	Simplex c( s);
	std::cout << "(copy test) c = " << c << " " << (c == s) << std::flush << std::endl;
	Simplex t( {1,2,5,5,4});
	std::cout << "t = " << t << std::flush << std::endl;
	t.insert( {8,9,10} );
	std::cout << "t = " << t << std::flush << std::endl;
	t.remove( 5);
	std::cout << "t = " << t << std::flush << std::endl;
	t.remove( t.begin()+2, t.begin()+4);
	std::cout << "t = " << t << std::flush << std::endl;
	return 0;
}

