// Creates a Python class for an `Abstract_simplex`. 
void wrap_product(py::module &mod){
  using s = ctl::Product_cell< ctl::Abstract_simplex, ctl::Abstract_simplex >;
   py::class_<s>(mod, std::string("Simplex_product").c_str())
    //Default no-args constructor
    .def(py::init())
    .def(py::init<typename s::Cell1, typename s::Cell2> ())

    .def("dimension", &s::dimension, "dimension")
    .def("__str__", stream_to_string<s>())
    .def("first_cell", [](const s& product){
			return product.first_cell();
 	 },
	"first cell", py::keep_alive<0,1>())
    
    .def("second_cell", [](const s& product){
			return product.second_cell();
 	 },
	"second cell", py::keep_alive<0,1>())
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self < py::self);

    wrap_term<s, ctl::Finite_field<2>>(mod, "Term_Product_Z2");
    wrap_term<s, ctl::Finite_field<3>>(mod, "Term_Product_Z3");
    wrap_term<s, ctl::Finite_field<5>>(mod, "Term_Product_Z5");
    
    using b = ctl::Product_boundary< ctl::Simplex_boundary, ctl::Simplex_boundary> ;
    py::class_<b>(mod, std::string("Simplex_product_boundary").c_str())
    //Default no-args constructor
    .def(py::init())
    .def("__call__", [](const b &bd, const s& smplx) {
		    return py::make_iterator(bd.begin(smplx), bd.end(smplx), py::return_value_policy::move);
    }, py::keep_alive<0, 1>())
    .def("length", [](const b& bd, const s& smplx){
        return bd.length( smplx);
    }, "length of the boundary");

}
