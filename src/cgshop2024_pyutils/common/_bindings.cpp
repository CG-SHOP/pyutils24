#ifndef __COMMON_BINDINGS_H__
#define __COMMON_BINDINGS_H__
/**
 * @file _bindings.cpp
 * @author Dominik Krupke (krupke@ibr.cs.tu-bs.de),
 * @brief Bindings to the C++ code.
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */

// pybind11
#include <pybind11/operators.h> // to define operator overloading
#include <pybind11/pybind11.h>  // basic pybind11 functionality
#include <pybind11/stl.h>       // automatic conversion of vectors

#include "cgal_defs.h"
#include "common_defs.h"

namespace py = pybind11;

void bind_common(py::module &m) {
  using Num = cgshop2024::Num;
  using Coordinate = cgshop2024::Coordinate;
  using AbstractChecker = cgshop2024::AbstractChecker;

  py::class_<Coordinate>(m, "Coordinate")
      .def(py::init<Num, Num>(), py::arg("x"), py::arg("y"))
      .def_readonly("x", &Coordinate::x)
      .def_readonly("y", &Coordinate::y);

  // this abstract checker itself is not used, but is subclassed for the actual verifiers
  py::class_<AbstractChecker>(m, "AbstractChecker")
      .def("add_item", &AbstractChecker::add_item, py::arg("item"));

  // free functions
  m.def("translate", &cgshop2024::translate, py::arg("points"),
        py::arg("translation"));

  // Exceptions
  py::register_exception<cgshop2024::PackingError>(m, "PackingError");
}

#endif //__COMMON_BINDINGS_H__