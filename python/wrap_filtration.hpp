
#include <boost/iterator/indirect_iterator.hpp>

// Creates a Python class for an `Simplex_boundary`. 
template< typename Complex>
void wrap_filtration(py::module &mod, std::string complex_name){
  using s = ctl::Graded_cell_complex<Complex>;
  std::string gcc = "Graded_chain_complex_";
  py::class_<s>(mod, (gcc+complex_name).c_str())
    .def("__init__",
      [](s &instance, Complex& c) {
          new (&instance) s(c);
     })
    .def("__init__",
      [](s &instance, Complex& c, std::function<bool(const typename Complex::Cell&, 
						     const typename Complex::Cell&)>& f) {
          new (&instance) s(c, f);
     })
    .def("__iter__", [](const s &c) { 
        return py::make_iterator(boost::make_indirect_iterator(c.begin()), boost::make_indirect_iterator(c.end()));
    }, py::keep_alive<0, 1>())
    .def("__str__", stream_to_string<s>());
}

void wrap_filtration(py::module& mod){
	wrap_filtration<ctl::Simplicial_complex<>>(mod, "sc");	
	wrap_filtration<ctl::Cubical_complex<>>(mod, "cc");	
	wrap_filtration<ctl::Prod_simplicial_complex>(mod, "psc");
	//wrap_filtration<ctl::Cube_simplicial_complex>(mod, "Cube_simplicial_complex");	
}
