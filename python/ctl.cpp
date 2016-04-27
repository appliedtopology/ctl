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

template< typename T>
std::function<std::string(const T&)> 
stream_to_string(){
    return [](const T& t){
        std::stringstream ss;
        ss << t;
        return ss.str();
    };
}

#include "wrap_ff.hpp"
#include "wrap_term.hpp"
#include "wrap_simplex.hpp"
//#include "wrap_cube.hpp"
#include "wrap_complex.hpp"
#include "wrap_persistence.hpp"
#include "wrap_vr.hpp"

//## Define the module
//This is where we actually define the `ctl` module. We'll also have a `phat` module that's written
PYBIND11_PLUGIN(ctl) {
  //Create the module object. First arg is the name, second is the module docstring.
  py::module m("ctl", "C++ wrapper for CTL.");
  //Wrap the `persistence_pairs` class
  wrap_ff<2>(m);
  wrap_ff<3>(m);
  wrap_ff<5>(m);
  wrap_ff<7>(m);
  wrap_ff<11>(m);
  wrap_simplex(m);
  //wrap_cube(m);
  wrap_complex(m);
  wrap_persistence(m);
  wrap_vr(m);
  //We're done!
  return m.ptr();

}
