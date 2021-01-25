#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>


struct Point {
  int64_t x_ = 0;
  int64_t y_ = 0;
  Point(int64_t x, int64_t y) : x_(x), y_(y) {}
  Point() = default;

  bool operator==(const Point& other) const;
};

bool Point::operator==(const struct Point & other) const {
  return x_ == other.x_ && y_ == other.y_;
}

std::istream& operator>>(std::istream& input, Point& point) {
  input >> point.x_ >> point.y_;
  return input;
}

struct Vector {
  int64_t x_ = 0;
  int64_t y_ = 0;
  Vector(const Point& start, const Point& end) : x_(end.x_ - start.x_),
                                                 y_(end.y_ - start.y_) {}

  inline int64_t CrossProduct(const Vector& other) const;
  inline int64_t GetSquaredLength() const;
};

inline int64_t Vector::CrossProduct(const Vector & other) const {
  return x_ * other.y_ - y_ * other.x_;
}

inline int64_t Vector::GetSquaredLength() const {
  return x_ * x_ + y_ * y_;
}


Point FindStartingPoint(const std::vector<Point>& points) {
  Point start_point = points.front();
  for (const Point& new_point : points) {
    // looking for point with lowest y coordinate and lowest x coordinate
    if (new_point.y_ < start_point.y_ ||
            (new_point.y_ == start_point.y_ && new_point.x_ < start_point.x_)) {
      start_point = new_point;
    }
  }
  return start_point;
}

struct CompareByAngle {
  Point start_point_;
  explicit CompareByAngle(const Point& point) : start_point_(point) {}

  bool operator()(const Point& p1, const Point& p2) {
    Vector v1(start_point_, p1);
    Vector v2(start_point_, p2);
    int64_t cross_product = v1.CrossProduct(v2);
    if (cross_product > 0) {
      return true;
    } else if (cross_product < 0) {
      return false;
    } else {
      return v1.GetSquaredLength() < v2.GetSquaredLength();
    }
  }
};


std::vector<Point> Get2DConvexHull(std::vector<Point>& points) {
  Point start_point = FindStartingPoint(points);
  std::sort(points.begin(), points.end(), CompareByAngle(start_point));

  // removing equivalent elements
  auto last = std::unique(points.begin(), points.end());
  points.erase(last, points.end());

  std::vector<Point> result;
  result.push_back(start_point);
  result.push_back(points[1]);
  for (size_t i = 2; i < points.size(); ++i) {
    Point p2 = result[result.size() - 1];
    Point p1 = result[result.size() - 2];
    while(result.size() >= 2 &&
          Vector(p1, p2).CrossProduct(Vector(p2, points[i])) < 0) {
      result.pop_back();
      p2 = p1;
      p1 = result[result.size() - 2];
    }
    result.push_back(points[i]);
  }
  return result;
}

double GetConvexHullPerimeter(std::vector<Point> points) {
  std::vector<Point> convex_hull = Get2DConvexHull(points);
  convex_hull.push_back(convex_hull.front());
  double result = 0;
  for(size_t i = 0; i < convex_hull.size() - 1; ++i) {
    Vector another_convex_edge (convex_hull[i], convex_hull[i + 1]);
    result += std::sqrt(another_convex_edge.GetSquaredLength());
  }
  return result;
}

int main() {
  int32_t n = 0;
  std::cin >> n;

  std::vector<Point> points(n);
  for (int32_t i = 0; i < n; ++i) {
    std::cin >> points[i];
  }
  std::cout << std::setprecision(10) << GetConvexHullPerimeter(points) ;
  return 0;
}
