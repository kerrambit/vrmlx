#include "vrmlxpy.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/attr.h>
#include <pybind11/cast.h>

namespace py = pybind11;

PYBIND11_MODULE(vrmlxpy, m) {
  m.doc() = "Python bindings for vrmlxpy.";

  m.def("print_version", &vrmlxpy::PrintVersion, "A function that prints a current vrmlxpy version.");

  m.def("convert_vrml",
      py::overload_cast<const std::string&, const std::string&, const std::string&>(&vrmlxpy::ConvertVrmlToGeom),
      "Converts a VRML file to a geometry format based on a configuration file", py::arg("input_filename"),
      py::arg("output_filename"), py::arg("config_filename"));
}