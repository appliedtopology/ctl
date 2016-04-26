// Creates a Python class for an `Abstract_simplex`. 
void wrap_simplex(py::module &mod){
  using s = ctl::Abstract_simplex;
  typedef s::vertex_type v;
  py::class_<s>(mod, std::string("Abstract_simplex").c_str())
    //Default no-args constructor
    .def(py::init())
    .def("__init__",
      [](s &instance, std::list<v> arg) {
          new (&instance) s(std::begin(arg), std::end(arg));
     })
    .def("__init__",
      [](s &instance, std::vector<v> arg) {
          new (&instance) s(std::begin(arg), std::end(arg));
     })
     .def("__init__",
      [](s &instance, std::set<v> arg) {
          new (&instance) s(std::begin(arg), std::end(arg));
     })
    //.def(py::init<std::initializer_list<int>>())
    .def("__len__", &s::size)
    .def("size", &s::size, "size of simplex")
    .def("capacity", &s::capacity, "capacity")
    .def("insert", [](s& smplx, const v& vtx){
        smplx.insert(vtx);
        return smplx;
    }, "inserts a vertex", py::arg("v"))
    .def("remove", [](s& smplx, const v& vtx){
        smplx.remove(vtx);
        return smplx;
     }, "removes a vertex", py::arg("v"))
    .def("back", &s::back, "last vertex")
    .def("front", &s::front, "front vertex")
    .def("reserve", &s::reserve, "reserve capacity", py::arg("n"))
    .def("clear", &s::clear, "clear the simplex")
    .def("dimension", &s::dimension, "dimension of simplex")
    .def("__contains__", [](const s &smplx, const v& vtx){ 
         return std::binary_search(smplx.begin(), smplx.end(), vtx); 
     })
     .def("__str__", stream_to_string<s>())
     // Essential: keep object alive while iterator exists  
    .def("__iter__", [](const s &smplx) { return py::make_iterator(smplx.begin(), smplx.end());}, py::keep_alive<0, 1>()) 
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self < py::self);
    py::implicitly_convertible<std::list<v>, s >();
    py::implicitly_convertible<std::set<v>, s >();
    py::implicitly_convertible<std::vector<v>, s >();

    wrap_term<s, ctl::Finite_field<2>>(mod, "Term_Simplex_Z2");
    wrap_term<s, ctl::Finite_field<3>>(mod, "Term_Simplex_Z3");
    wrap_term<s, ctl::Finite_field<5>>(mod, "Term_Simplex_Z5");
    
    using b = ctl::Simplex_boundary;
    py::class_<b>(mod, std::string("Simplex_boundary").c_str())
    //Default no-args constructor
    .def(py::init())
    .def("__call__", [](const b &bd, const s& smplx) { 
        return py::make_iterator(bd.begin(smplx), bd.end(smplx)); 
    }, py::keep_alive<0, 1>())
    .def("length", [](const b& bd, const s& smplx){
        return bd.length( smplx);
    }, "length of the boundary");
}
