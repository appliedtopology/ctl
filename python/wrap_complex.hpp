// Creates a Python class for an `Simplex_boundary`. 
void wrap_complex(py::module &mod){
  using s = ctl::Simplicial_complex<>;
  typedef typename s::Cell c;
  py::class_<typename s::Data>(mod, std::string("Cell_data").c_str())
   	.def(py::init())
        .def(py::init< typename s::Data>())
	.def("__str__", stream_to_string< typename s::Data>());	
 
  py::class_<s>(mod, std::string("Simplicial_complex").c_str())
    //Default no-args constructor
    .def(py::init())
    .def("__init__",
      [](s &instance, std::list<c> arg) {
          new (&instance) s(std::begin(arg), std::end(arg));
     })
    .def("__init__",
      [](s &instance, std::vector<c> arg) {
          new (&instance) s(std::begin(arg), std::end(arg));
     })
     .def("__init__",
      [](s &instance, std::set<c> arg) {
          new (&instance) s(std::begin(arg), std::end(arg));
     })

    //.def("find_cell", &s::find_cell)
    .def("insert_open_cell", [](s& cmplx, const s::Cell & c){
        cmplx.insert_open_cell( c); 
        return cmplx;
    }, "insert simplex")
    .def("insert_closed_cell", [](s& cmplx, const s::Cell & c){
        cmplx.insert_closed_cell( c); 
        return cmplx;
    }, "insert simplex and its boundary")
    .def("dimension", &s::dimension, "dimension of complex")
    .def("size", &s::size, "number of simplices")
    .def("__len__", &s::size)
     .def("__contains__", [](const s &cmplx, const s::Cell& c){ 
         return (cmplx.find_cell( c) != cmplx.end()); 
     })
    .def("is_closed", &s::is_closed, "check if all faces of all simplices exist")
    .def("__iter__", [](const s &c) { 
        return py::make_iterator(c.begin(), c.end());
    }, py::keep_alive<0, 1>())
    .def("__str__", stream_to_string<s>())
    .def(py::self == py::self)
    .def(py::self != py::self);
    py::implicitly_convertible<std::list<c>, s >();
    py::implicitly_convertible<std::set<c>, s >();
    py::implicitly_convertible<std::vector<c>, s >();
}
