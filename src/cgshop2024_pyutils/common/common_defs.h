#ifndef __COMMON_DEFS_H__
#define __COMMON_DEFS_H__

#include "cgal_defs.h"

namespace cgshop2024 {

class PackingError : public std::exception {
public:
  PackingError(std::string msg) : msg(msg) {}
  virtual const char *what() const throw() { return msg.c_str(); }

private:
  std::string msg;
};

using Num = int64_t; // specifically int64_t, as long (and others) are platform-dependant

class Coordinate {
public:
  Coordinate() : x(0), y(0) {}
  Coordinate(Num x, Num y) : x(x), y(y) {}
  Num x, y;
};

using CV = std::vector<Coordinate>;

inline CV translate(const CV &points, Coordinate translation) {
  CV translated;
  translated.reserve(points.size());
  for (auto &p : points) {
    translated.emplace_back(p.x + translation.x, p.y + translation.y);
  }
  return translated;
}


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