//Required header for using pybind11
#include <pybind11/pybind11.h>

//Automatic conversions of stl containers to Python ones
#include <pybind11/stl.h>

//Additional support for operators and numpy
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

//All the things we're going to wrap
#include <ctl/ctl.hpp>

namespace py = pybind11;

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
//## Define the module
//This is where we actually define the `_phat` module. We'll also have a `phat` module that's written
//in Python, which will use `_phat` as an implementation detail.
PYBIND11_PLUGIN(_ctl) {
  //Create the module object. First arg is the name, second is the module docstring.
  py::module m("ctl", "C++ wrapper for CTL. Please use the CTL module, not the _ctl module");

  //Wrap the `persistence_pairs` class
  wrap_simplex(m);

  //We're done!
  return m.ptr();

}
