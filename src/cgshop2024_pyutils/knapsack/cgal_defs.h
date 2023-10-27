#ifndef CGAL_DEFS_H
#define CGAL_DEFS_H

// CGAL
#include <CGAL/Boolean_set_operations_2.h> // do_intersect
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Polygon_2.h>

namespace cgshop2024 {
// Define CGAL types
using Kernel = CGAL::Epeck; // Exact Predicates Exact Constructions Kernel
using Point = CGAL::Point_2<Kernel>;
using Polygon2 = CGAL::Polygon_2<Kernel>;
} // namespace cgshop2024
#endif // CGAL_DEFS_H
