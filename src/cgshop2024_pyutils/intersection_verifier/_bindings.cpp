
// pybind11
#include <pybind11/operators.h> // to define operator overloading
#include <pybind11/pybind11.h>  // basic pybind11 functionality
#include <pybind11/stl.h>       // automatic conversion of vectors

#include "simple_verifier.h"

namespace py = pybind11;

void bind_intersection_verifier(py::module &m) {
  using Coordinate = cgshop2024::Coordinate;
  using IntersectionChecker = cgshop2024::SimpleIntersectionChecker;
  using AbstractChecker = cgshop2024::AbstractChecker;

  py::class_<IntersectionChecker, AbstractChecker>(m, "IntersectionChecker")
      .def(py::init<std::vector<Coordinate>>(), py::arg("boundary"))
      .def("add_item", &IntersectionChecker::add_item, py::arg("item"));
}