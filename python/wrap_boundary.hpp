// Creates a Python class for an `Simplex_boundary`. 
void wrap_boundary(py::module &mod){
  using s = ctl::Simplex_boundary;
  typedef s::Simplex Simplex;
  typedef s::const_iterator i;
   py::class_<s>(mod, std::string("Simplex_boundary").c_str())
    //Default no-args constructor
    .def(py::init())
    .def("begin", (i (s::*)(const Simplex&) )&s::begin, "begin iterator", py::arg("s")) 
    .def("end", (i (s::*)(const Simplex& ) )&s::end, "end iterator", py::arg("s"))
    .def("length", (i (s::*)(const Simplex& ) )&s::end, "length of the boundary", py::arg("s"));
}
