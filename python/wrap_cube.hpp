// Creates a Python class for a `Cube.`
void wrap_cube(py::module &mod){
  using s = ctl::Cube;
  typedef s::value_type v;
  py::class_<s>(mod, std::string("Cube").c_str())
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
    .def("insert", [](s& cube, const v& interval){
        cube.insert(interval);
        return cube;
    }, "inserts a vertex", py::arg("v"))
    .def("remove", [](s& cube, const v& interval){
        cube.remove(interval);
        return cube;
     }, "removes a vertex", py::arg("v"))
    .def("reserve", &s::reserve, "reserve capacity", py::arg("n"))
    .def("clear", &s::clear, "clear the simplex")
    .def("dimension", &s::dimension, "dimension of simplex")
    .def("__contains__", [](const s &cube, const v& interval){ 
         return std::binary_search(cube.begin(), cube.end(), interval); 
     })

     // Essential: keep object alive while iterator exists  
    .def("__iter__", [](const s &cube) { return py::make_iterator(cube.begin(), cube.end());}, py::keep_alive<0, 1>()) 
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self < py::self);
    py::implicitly_convertible<std::list<v>, s >();
    py::implicitly_convertible<std::set<v>, s >();
    py::implicitly_convertible<std::vector<v>, s >();
}
