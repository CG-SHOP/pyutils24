#ifndef __KNAPSACK_H__
#define __KNAPSACK_H__

#include "./cgal_defs.h"
#include "./quadtree.h"

namespace cgshop2024 {

class PackingError : public std::exception {
public:
  PackingError(std::string msg) : msg(msg) {}
  virtual const char *what() const throw() { return msg.c_str(); }

private:
  std::string msg;
};

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

std::vector<Coordinate> translate(const std::vector<Coordinate> &points,
                                  Coordinate translation) {
  std::vector<Coordinate> translated;
  translated.reserve(points.size());
  for (auto &p : points) {
    translated.emplace_back(p.x + translation.x, p.y + translation.y);
  }
  return translated;
}

class PolygonalKnapsack {
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

public:
  PolygonalKnapsack(std::vector<Coordinate> boundary)
      : container(), items(), qtree(get_bbox(boundary)) {
    container = to_poly(boundary);
    if (container.area() <= 0) {
      throw std::runtime_error("Container must have positive area.");
    }
  }

  void add_item(std::vector<Coordinate> &item) {
    for (auto &p : item) {
      // Check if point is inside container
      if (!inside_container(p.x, p.y)) {
        throw PackingError("Item must be inside container.");
      }
    }
    Polygon2 item_poly = to_poly(item);
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
    items.push_back(item_poly);
  }

  Polygon2 container;
  std::vector<Polygon2> items;
  QuadTree<int> qtree;
};
} // namespace cgshop2024
#endif // __KNAPSACK_H__
