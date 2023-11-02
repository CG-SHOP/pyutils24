// pybind11
#include <pybind11/operators.h> // to define operator overloading
#include <pybind11/pybind11.h>  // basic pybind11 functionality
#include <pybind11/stl.h>       // automatic conversion of vectors

namespace py = pybind11;

// forward definition of the submodules
void bind_common(py::module &);
void bind_knapsack(py::module &);
void bind_intersection_verifier(py::module &);

// Getting this name right is important! It has to equal the name in the
// CMakeLists.txt.
PYBIND11_MODULE(_bindings, m) {
  m.doc() = "Bindings to the C++ code.";
  bind_common(m);
  auto knapsack_verifier =
      m.def_submodule("knapsack_verifier", "Verifier based on a Knapsack");
  bind_knapsack(knapsack_verifier);
  auto intersection_verifier =
      m.def_submodule("intersection_verifier",
                      "Simple, but slow verifier based on intersections");
  bind_intersection_verifier(intersection_verifier);
}
