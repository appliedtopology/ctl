// Creates a Python class for an `Abstract_simplex`. 
void wrap_simplex(py::module &mod){
  using s = ctl::Abstract_simplex;
  typedef s::iterator i;
  typedef s::vertex_type v;
  py::class_<s>(mod, std::string("Abstract_simplex").c_str())
    //Default no-args constructor
    .def(py::init())
    .def("size", &s::size, "size of simplex")
    .def("capacity", &s::capacity, "capacity")
    .def("insert", (i (s::*)(const v&) )&s::insert, "inserts a vertex", py::arg("v"))
    .def("remove", (i (s::*)(const v ) )&s::remove, "removes a vertex", py::arg("v"))
    .def("back", &s::back, "last vertex")
    .def("front", &s::front, "front vertex")
    .def("reserve", &s::reserve, "reserve capacity", py::arg("n"))
    .def("clear", &s::clear, "clear the simplex")
    .def("dimension", &s::dimension, "dimension of simplex");
/*  Functions left to wrap
    'begin',
    'end',
    'operator!=',
    'operator<',
    'operator=',
    'operator==',
    'rbegin',
    'read',
    'rend',
    'write'*/
}
