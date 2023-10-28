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
using Num = int64_t;

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
      throw std::runtime_error(
          "Cannot create node with empty area. This is an overcautious "
          "exception of a state I did not expect to happen.");
    }
    if (depth() > max_depth) {
      throw std::runtime_error(
          "Node depth exceeds maximum depth. This is an overcautious exception "
          "of a state I did not expect to happen.");
    }
    children.reserve(4);
  }

  Rectangle area;
  std::vector<Element<T>> elements;
  std::vector<QuadTreeNode<T>> children;
  unsigned int max_elements = 25;
  unsigned int max_depth = 30;

  void split_node() {
    // Split the node.
    auto sub_areas = area.split();
    for (unsigned int i = 0; i < 4; i++) {
      children.push_back(QuadTreeNode<T>(sub_areas[i]));
    }
    // Move elements to children.
    std::vector<Element<T>> elements_to_keep_on_this_level;
    for (auto &element : elements) {
      // Keep large elements on this level.
      if (element.area.volume() > area.volume()) {
        elements_to_keep_on_this_level.push_back(element);
        continue;
      }
      // Move small elements to children.
      for (auto &child : children) {
        if (child.area.do_overlap(element.area)) {
          child.add(element);
        }
      }
    }
    elements = elements_to_keep_on_this_level;
  }

  void add(const Element<T> &element) {
    if (!area.do_overlap(element.area)) {
      throw std::runtime_error("Element does not overlap with node. This is a "
                               "bug in the quadtree implementation.");
    }
    if (!children.empty()) {
      if (element.area.volume() > area.volume()) {
        // Add large element to this node.
        // Otherwise, they will get duplicated too much.
        elements.push_back(element);
        if (elements.size() > 100) {
          std::cout << "Warning: Overfilling node in quadtree due to volume of "
                       "element (size: "
                    << elements.size() << ")" << std::endl;
        }
      } else {
        assert(children.size() == 4);
        // Add small elements to children.
        for (auto &child : children) {
          if (child.area.do_overlap(element.area)) {
            child.add(element);
          }
        }
      }
    } else {
      // Add to this node.
      elements.push_back(element);
      // Check if we need to split this node.
      if (elements.size() > max_elements) {
        if (!area.is_splitable()) {
          std::cout << "Warning: Overfull node (size: " << elements.size()
                    << ") cannot be split." << std::endl;
          return;
        }
        // Split this node.
        split_node();
      }
    }
  }

  std::vector<T> query(const Rectangle &query_area) const {
    std::vector<T> result;
    if (!area.do_overlap(query_area)) {
      return result;
    }
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
    if (!children.empty()) {
      // Query children.
      for (auto &child : children) {
        if (child.area.do_overlap(query_area)) {
          auto child_result = child.query(query_area);
          result.insert(result.end(), child_result.begin(), child_result.end());
        }
      }
    }
    return result;
  }

  unsigned int size() const {
    unsigned int result = 0;
    if (!children.empty()) {
      // Size of children.
      for (auto &child : children) {
        result += child.size();
      }
    }
    // Size of this node.
    result += elements.size();
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

  void add(const Element<T> &element) {
    if (!element.area.do_overlap(root.area)) {
      throw std::runtime_error("Trying to insert an element into the quadtree "
                               "that does not overlap with the root node.");
    }
    root.add(element);
  }

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
