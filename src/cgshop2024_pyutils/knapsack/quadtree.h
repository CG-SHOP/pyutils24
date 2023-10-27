/**
 * @file quadtree.h
 * @author Dominik Krupke (krupke@ibr.cs.tu-bs.de)
 * @brief This file contains the interface for a simple quadtree.
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <array>
#include <optional>
#include <stdexcept> // runtime_error
#include <vector>

namespace cgshop2024 {
using Num = long;

class Coordinate {
public:
  Coordinate() : x(0), y(0) {}
  Coordinate(Num x, Num y) : x(x), y(y) {}
  Num x, y;
};

class Rectangle {
public:
  Rectangle() : x(0), y(0), width(0), height(0) {
    // Empty rectangle as placeholder.
  }

  Rectangle(Num x, Num y, Num width, Num height)
      : x(x), y(y), width(width), height(height) {
    if (width <= 0 || height <= 0) {
      // Just to make sure that we didn't do something stupid.
      throw std::runtime_error(
          "Rectangle must have positive width and height.");
    }
  }

  bool do_overlap(const Rectangle &other) const {
    // Two rectangles to not overlap iff there is either a
    // horizontal or vertical line separating them.
    // Note: Overlapping at the border is not considered overlapping.
    if (other.volume() == 0 || volume() == 0) {
      throw std::runtime_error("Comparison on empty rectangle.");
    }
    if (x + width <= other.x || other.x + other.width <= x) {
      return false;
    }
    if (y + height <= other.y || other.y + other.height <= y) {
      return false;
    }
    return true;
  }

  Num volume() const { return width * height; }

  bool is_splitable() const { return width > 1 && height > 1; }

  std::array<Rectangle, 4> split() const {
    if (width == 0 || height == 0) {
      throw std::runtime_error("Cannot split empty rectangle.");
    }
    // Split the rectangle into four (roughly) equally sized rectangles.
    std::array<Rectangle, 4> result;
    result[0] = Rectangle(x, y, width / 2, height / 2);
    assert(result[0].do_overlap(*this));
    result[1] = Rectangle(x + width / 2, y, width / 2, height / 2);
    assert(result[1].do_overlap(*this));
    result[2] = Rectangle(x, y + height / 2, width / 2, height / 2);
    assert(result[2].do_overlap(*this));
    result[3] = Rectangle(x + width / 2, y + height / 2, width / 2, height / 2);
    assert(result[3].do_overlap(*this));
    assert(result[0].volume() + result[1].volume() + result[2].volume() +
               result[3].volume() ==
           volume());
    return result;
  }

  Num x, y, width, height;
};

template <typename T> class Element {
public:
  Element(const Rectangle &area, const T &data) : area(area), data(data) {}
  Rectangle area;
  T data;
};

template <typename T> class QuadTreeNode {
public:
  QuadTreeNode() : area(Rectangle()) {}
  QuadTreeNode(const Rectangle &area) : area(area) {
    if (area.volume() == 0) {
      throw std::runtime_error("Cannot create node with empty area.");
    }
    if (depth() > max_depth) {
      throw std::runtime_error("Node depth exceeds maximum depth.");
    }
    children.reserve(4);
  }

  Rectangle area;
  std::vector<Element<T>> elements;
  std::vector<QuadTreeNode<T>> children;
  unsigned int max_elements = 25;
  unsigned int max_depth = 30;

  void add(const Element<T> &element) {
    if (!area.do_overlap(element.area)) {
      throw std::runtime_error("Element does not overlap with node.");
    }
    if (!children.empty()) {
      assert(elements.empty());
      assert(children.size() == 4);
      // Add to children.
      for (auto &child : children) {
        if (child.area.do_overlap(element.area)) {
          child.add(element);
        }
      }
    } else {
      // Add to this node.
      elements.push_back(element);
      if (elements.size() > max_elements) {
        if (!area.is_splitable()) {
          throw std::runtime_error("Node is not splitable.");
        }
        // Split the node.
        auto sub_areas = area.split();
        for (unsigned int i = 0; i < 4; i++) {
          children.push_back(QuadTreeNode<T>(sub_areas[i]));
        }
        // Move elements to children.
        for (auto &child : children) {
          for (auto &element : elements) {
            if (child.area.do_overlap(element.area)) {
              child.add(element);
            }
          }
        }
        elements.clear();
      }
    }
  }

  std::vector<T> query(const Rectangle &query_area) const {
    std::vector<T> result;
    if (!area.do_overlap(query_area)) {
      return result;
    }
    if (!children.empty()) {
      // Query children.
      for (auto &child : children) {
        if (child.area.do_overlap(query_area)) {
          auto child_result = child.query(query_area);
          result.insert(result.end(), child_result.begin(), child_result.end());
        }
      }
    } else {
      // Query this node.
      for (auto &element : elements) {
        if (element.area.do_overlap(query_area)) {
          // only add if the element is not already in the result.
          if (std::find(result.begin(), result.end(), element.data) ==
              result.end()) {
            result.push_back(element.data);
          }
        }
      }
    }
    return result;
  }

  unsigned int size() const {
    unsigned int result = 0;
    if (!children.empty()) {
      // Query children.
      for (auto &child : children) {
        result += child.size();
      }
    } else {
      // Query this node.
      result += elements.size();
    }
    return result;
  }

  unsigned int depth() const {
    unsigned int result = 0;
    // Query children.
    for (auto &child : children) {
      result = std::max(result, child.depth());
    }
    return result + 1;
  }
};

template <typename T> class QuadTree {
public:
  QuadTree(const Rectangle &area) : root(area) {}

  void add(const Element<T> &element) { root.add(element); }

  std::vector<T> query(const Rectangle &query_area) const {
    return root.query(query_area);
  }

  unsigned int size() const { return root.size(); }

  unsigned int depth() const { return root.depth(); }

protected:
  QuadTreeNode<T> root;
};
}; // namespace cgshop2024

#endif /* __QUADTREE_H__ */
