#ifndef __SIMPLE_VERIFIER_H__
#define __SIMPLE_VERIFIER_H__

#include "../common/cgal_defs.h"
#include "../common/common_defs.h"
namespace cgshop2024 {

class SimpleIntersectionChecker : public AbstractChecker {

public:
  SimpleIntersectionChecker(std::vector<Coordinate> boundary)
      : AbstractChecker{boundary} {}

  void check_item(CV &item, Polygon2 &item_poly) override {
    for (const auto &packed : items) {
      std::vector<Polygon_with_holes2> intersections;
      CGAL::intersection(packed, item_poly, std::back_inserter(intersections));
      for (const auto &intersection : intersections) {
        if (intersection.outer_boundary().area() != 0) {
          throw PackingError("Items must not intersect.");
        }
      }
    }
  }
};
} // namespace cgshop2024
#endif // __SIMPLE_VERIFIER_H__
