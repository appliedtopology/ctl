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

#include "wrap_simplex.hpp"
//#include "wrap_boundary.hpp"
//#include "wrap_complex.hpp"
#include "wrap_persistence.hpp"
//## Define the module
//This is where we actually define the `ctl` module. We'll also have a `phat` module that's written
PYBIND11_PLUGIN(ctl) {
  //Create the module object. First arg is the name, second is the module docstring.
  py::module m("ctl", "C++ wrapper for CTL.");
  //Wrap the `persistence_pairs` class
  wrap_simplex(m);
  wrap_persistence(m);
  //We're done!
  return m.ptr();

}
