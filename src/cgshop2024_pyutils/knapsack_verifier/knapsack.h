#ifndef __KNAPSACK_H__
#define __KNAPSACK_H__

#include "../common/cgal_defs.h"
#include "../common/common_defs.h"
#include "./quadtree.h"

namespace cgshop2024 {

Rectangle get_bbox(const std::vector<Coordinate> &points) {
  Num min_x = std::numeric_limits<Num>::max();
  Num min_y = std::numeric_limits<Num>::max();
  Num max_x = std::numeric_limits<Num>::min();
  Num max_y = std::numeric_limits<Num>::min();
  for (auto &p : points) {
    // Update bounding box
    min_x = std::min(min_x, p.x);
    min_y = std::min(min_y, p.y);
    max_x = std::max(max_x, p.x);
    max_y = std::max(max_y, p.y);
  }
  return Rectangle(min_x, min_y, max_x - min_x, max_y - min_y);
}

class PolygonalKnapsack : public AbstractChecker {

public:
  PolygonalKnapsack(std::vector<Coordinate> boundary)
      : AbstractChecker{boundary}, qtree(get_bbox(boundary)) {}

  void check_item(CV &item, Polygon2 &item_poly) override {
    // Check intersections with other items
    Rectangle bbox = get_bbox(item);
    auto possible_intersections = qtree.query(bbox);
    for (auto &i : possible_intersections) {
      if (CGAL::do_intersect(item_poly, items[i])) {
        throw PackingError("Items must not intersect.");
      }
    }
    // Add item to quadtree
    qtree.add(Element<int>(bbox, items.size()));
  }

  QuadTree<int> qtree;
};
} // namespace cgshop2024
#endif // __KNAPSACK_H__
