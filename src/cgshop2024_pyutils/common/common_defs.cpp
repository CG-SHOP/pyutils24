#include "common_defs.h"

namespace cgshop2024 {

class AbstractChecker {
public:
  const Polygon2 container;
  std::vector<Polygon2> items;

  AbstractChecker(CV boundary) : container{to_poly(boundary)}, items() {
    if (container.area() <= 0) {
      throw std::runtime_error("Container must have positive area.");
    }
  }

  // should throw PackingError for invalid packings, otherwise return
  virtual void check_item(CV &item, Polygon2 &item_poly) = 0;

  void add_item(CV &item) {
    for (auto &p : item) {
      // Check if point is inside container
      if (!inside_container(p.x, p.y)) {
        throw PackingError("Item must be inside container.");
      }
    }
    Polygon2 item_poly = to_poly(item);
    check_item(item, item_poly);
    // if returned and didn't throw:
    items.push_back(item_poly);
  }

private:
  bool inside_container(Num x, Num y) {
    return container.bounded_side(Point(x, y)) != CGAL::ON_UNBOUNDED_SIDE;
  }

  Polygon2 to_poly(const std::vector<Coordinate> &points) {
    std::vector<Point> cgal_points;
    for (auto &p : points) {
      cgal_points.emplace_back(p.x, p.y);
    }
    return Polygon2(cgal_points.begin(), cgal_points.end());
  }
};

} // namespace cgshop2024

#endif // __COMMON_DEFS_H__