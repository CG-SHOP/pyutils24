"""
This package provides a polygonal knapsack that
allows to pack polygons and will throw an error
if an item does not fit.

The implementation is exact and in C++. The code
will be built automatically when installing the
package. This may take some time. Your IDE may
show errors because the module _bindings will
only be available after building. If you want
to fix that you can call `python setup.py develop`
to build the module and make it available in
the source folder.
"""

from .._bindings.knapsack_verifier import (
    Knapsack,
    QuadTree,
    Rectangle,
)

__all__ = [
    "Rectangle",
    "QuadTree",
    "Knapsack",
]
