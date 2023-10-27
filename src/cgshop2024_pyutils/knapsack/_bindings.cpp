/**
 * @file _bindings.cpp
 * @author Dominik Krupke (krupke@ibr.cs.tu-bs.de)
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

#include "./knapsack.h"
#include "./quadtree.h"

// Getting this name right is important! It has to equal the name in the
// CMakeLists.txt.
PYBIND11_MODULE(_bindings, m) {
  namespace py = pybind11;
  m.doc() = "Bindings to the C++ code.";
  using Num = cgshop2024::Num;
  using Rectangle = cgshop2024::Rectangle;
  using Coordinate = cgshop2024::Coordinate;
  using Val = long;
  using Quadtree = cgshop2024::QuadTree<Val>;
  using Element = cgshop2024::Element<Val>;
  using Knapsack = cgshop2024::PolygonalKnapsack;

  // Rectangle
  py::class_<Coordinate>(m, "Coordinate")
      .def(py::init<Num, Num>(), py::arg("x"), py::arg("y"))
      .def_readonly("x", &Coordinate::x)
      .def_readonly("y", &Coordinate::y);
  py::class_<Rectangle>(m, "Rectangle")
      .def(py::init<>())
      .def(py::init<long, long, long, long>(), py::arg("x"), py::arg("y"),
           py::arg("width"), py::arg("height"))
      .def("do_overlap", &Rectangle::do_overlap)
      .def("volume", &Rectangle::volume)
      .def("is_splitable", &Rectangle::is_splitable);
  // Quadtree
  py::class_<Quadtree>(m, "QuadTree")
      .def(py::init<Rectangle>(), py::arg("bounding_box"))
      .def(
          "add",
          [](Quadtree &self, Rectangle area, Val val) {
            self.add(Element(area, val));
          },
          py::arg("area"), py::arg("value"))
      .def("query", &Quadtree::query, py::arg("area"))
      .def("__len__", &Quadtree::size)
      .def("depth", &Quadtree::depth);
  // Knapsack
  py::class_<Knapsack>(m, "Knapsack")
      .def(py::init<std::vector<Coordinate>>(), py::arg("boundary"))
      .def("add_item", &Knapsack::add_item, py::arg("item"));
  // free functions
  m.def("translate", &cgshop2024::translate, py::arg("points"),
        py::arg("translation"));

  // Exceptions
  py::register_exception<cgshop2024::PackingError>(m, "PackingError");
}
