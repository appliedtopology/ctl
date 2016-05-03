// Creates a Python class for an `Simplex_boundary`. 

template< typename s>
void do_wrap_complex( py::module& mod, std::string name){
py::class_<s>(mod, name.c_str())
    //Default no-args constructor
    .def("__init__", [](s &instance, typename s::Complex& X, typename s::Complex& A) { new (&instance) s(X,A); })
    .def("size", &s::size, "number of cells")
    .def("__len__", &s::size)
    .def("__contains__", [](const s &cmplx, const typename s::Cell& c){ return (cmplx.find_cell( c) != cmplx.end()); })
    .def("__iter__", [](const s &c) { return py::make_iterator(c.begin(), c.end()); }, py::keep_alive<0, 1>())
    .def("__str__", stream_to_string<s>())
    .def(py::self == py::self)
    .def(py::self != py::self);

    using b = ctl::Relative_complex_boundary<s>;
    py::class_<b>(mod, (name+"_boundary").c_str())
    //Default no-args constructor
    .def("__init__", [](b &instance, s& X) { new (&instance) b(X); })
    .def("__call__", [](b &bd,  typename std::pair< typename s::Cell, typename s::Data> t) {
	auto i = bd.complex().find_cell( t.first);
	return py::make_iterator(bd.begin(i), bd.end(i)); 
    }, py::keep_alive<0, 1>())
    .def("length", [](const b& bd, const typename s::iterator smplx){
        return bd.length( smplx);
    }, "length of the boundary");
}

void wrap_rel_complex(py::module &mod){
  typedef ctl::Relative_cell_complex< ctl::Simplicial_complex<> > Relative_sc;
  typedef ctl::Relative_cell_complex< ctl::Prod_simplicial_complex > Relative_pd;
  do_wrap_complex< Relative_sc>( mod, "Relative_simplicial_complex");
  do_wrap_complex< Relative_pd>( mod, "Relative_product_complex"); 
}
