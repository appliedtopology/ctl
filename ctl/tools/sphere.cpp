 //STL
 #include <random>

template< typename OutputIterator>
void nsphere( std::size_t n, std::size_t k, 
	      double variance_of_noise, OutputIterator out){

 std::default_random_engine generator;
 std::normal_distribution<double> distribution(0.0,1.0);
 distribution(generator)

} 
