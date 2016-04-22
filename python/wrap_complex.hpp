// Creates a Python class for an `Simplex_boundary`. 
void wrap_complex(py::module &mod){
  using s = ctl::Simplicial_complex<>;
   py::class_<s>(mod, std::string("Simplicial_complex").c_str())
    //Default no-args constructor
    .def(py::init())
    //.def("find_cell", &s::find_cell)
    .def("insert_open_cell", [](s& cmplx, const s::Cell & c){
        cmplx.insert_open_cell( c); 
        return cmplx;
    }, "insert simplex")
    .def("insert_closed_cell", [](s& cmplx, const s::Cell & c){
        cmplx.insert_closed_cell( c); 
        return cmplx;
    }, "insert simplex and its boundary")
    .def("dimension", &s::dimension, "")
    .def("size", &s::size, "")
    .def("__len__", &s::size)
     .def("__contains__", [](const s &cmplx, const s::Cell& c){ 
         return (cmplx.find_cell( c) == cmplx.end()); 
     })
    .def("is_closed", &s::is_closed, "")
    .def("__iter__", [](const s &c) { 
        return py::make_iterator(c.begin(), c.end());
    }, py::keep_alive<0, 1>())
    .def(py::self == py::self)
    .def(py::self != py::self);
}
