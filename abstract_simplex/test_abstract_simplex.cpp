//#include "term.h"
#include "abstract_simplex.h"
#include <iostream>
#include "simplex_boundary.h"
#include "term.h"

#include "simplex_boundary.h"

typedef ct::Abstract_simplex< int> Simplex;
typedef ct::Simplex_boundary< Simplex, int> Boundary;
int main( int argc, char** argv){
	Simplex s;
	std::cout << "build a simplex, vertex by vertex." << std::endl;
	s.insert( 2);
	std::cout << s << std::flush << std::endl;
	s.insert( 1);
	std::cout << s << std::flush << std::endl;
	s.insert( 4);
	std::cout << s << std::flush << std::endl;
	s.insert( 3);
	std::cout << s << std::flush << std::endl;
	Simplex c( s);
	std::cout << "(copy test) c = " << c << " does c == s?" << (c == s) << std::flush << std::endl;
	Simplex t( {1,2,5,5,4});
	std::cout << "(il constructor) t = " << t << std::flush << std::endl;
	t.insert( {8,9,10} );
	std::cout << "(insert test 8,9,10) t = " << t << std::flush << std::endl;
	t.remove( 5);
	std::cout << "t = " << t << std::flush << std::endl;
	t.remove( t.begin()+2, t.begin()+4);
	
	std::cout << "t = " << t << std::flush << std::endl;
	Boundary b;
	std::cout << "printing out the boundary of t" << std::endl;
	for (auto i = b.begin( t); i != b.end( t); ++i){
		std::cout << (*i).cell() << std::endl;
	}
	return 0;
}
