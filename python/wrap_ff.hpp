// Creates a Python class for an `Simplex_boundary`. 
template< std::size_t prime>
void wrap_ff(py::module &mod){
   typedef ctl::Finite_field<prime> FF;
   py::class_<FF>(mod, (std::string("Z")+std::to_string(prime)).c_str())
    .def(py::init())
    .def(py::init<int>())
    //.def(py::self++)
    //.def(py::self--)
    .def(py::self+=py::self)
    .def(py::self-=py::self)
    .def(py::self+py::self)
    .def(py::self-py::self)
    .def(py::self/py::self)
    .def(py::self/=py::self)
    .def(py::self*=py::self)
    .def(py::self < py::self)
    .def("to_integral", &FF::to_integral)
    .def("__str__", stream_to_string<FF>())
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self == py::self)
    .def(py::self != py::self);
    py::implicitly_convertible<int,FF>();
}
