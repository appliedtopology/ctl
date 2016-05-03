// Creates a Python class for an `Simplex_boundary`. 
template< typename Cell, typename Coefficient>
void wrap_term(py::module &mod, std::string term){
   typedef ctl::Term<Cell, Coefficient> Term;
   py::class_<Term>(mod, term.c_str())
    //Default no-args constructor
    .def(py::init())
    .def(py::init<Cell, Coefficient>())
    .def("coefficient", [](const Term& t){
        return t.coefficient();
     }, "coefficient")
    .def("cell", [](const Term& t){
        return t.cell();
     }, "cell")
    .def(py::self < py::self)
    .def(py::self == py::self)
    .def("__str__", stream_to_string<Term>())
    .def(py::self != py::self);
     py::implicitly_convertible<std::pair<Cell, Coefficient>, Term >();
}