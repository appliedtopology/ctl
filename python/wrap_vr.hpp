#include <ctl/vr/vr.hpp>
#include "numpy_array.hpp"
template< typename T>
std::vector< std::vector< T>> 
stupid_copy( py::buffer_info info){
  std::vector< std::vector< T>> p;
  p.reserve( info.shape[0]);
  for( int i = 0; i < info.shape[0]; ++i){
  	std::vector< T> pt( (T*)info.ptr + i*info.shape[1], (T*)info.ptr + i*info.shape[1]);
  	p.emplace_back( pt);
  }
  return p;
}

template< typename T>
std::list< ctl::Abstract_simplex> 
vr_wrapper_helper(py::buffer_info& info, double epsilon, std::size_t dimension){
		auto p = stupid_copy<T>( info);
		auto c = ctl::vr( p, epsilon, dimension);
		std::list< ctl::Abstract_simplex> stupider_copy; 
		//stupider_copy.reserve(c.size());
		for( auto& s : c){
			stupider_copy.push_back(s.first);
		}
	       return stupider_copy;	
}

std::list< ctl::Abstract_simplex> 
vr_wrapper(py::buffer & b, double epsilon, std::size_t dimension){
	 py::buffer_info info = b.request();
	 //TODO: ..don't copy the dataset..
	 if (info.format == py::format_descriptor<float>::value()){
	 	return vr_wrapper_helper< float>( info, epsilon, dimension);	
	 }else if (info.format == py::format_descriptor<double>::value()){
	 	return vr_wrapper_helper< double>( info, epsilon, dimension);	
	 } 
	 throw std::runtime_error("Incompatible buffer format!"); 
}

// Creates a Python class for an `Abstract_simplex`. 
void wrap_vr(py::module &mod){
  mod.def("vr", &vr_wrapper, "compute the vr complex of a set of points. returns list of simplices");
}
