#include <algorithm>
#include <iostream>
#include <vector>


struct Point {
  double x_ = 0;
  double y_ = 0;
  Point(double x, double y) : x_(x), y_(y) {}
  Point() = default;
  Point operator+(const Point& other) const;
  Point&operator*=(int32_t k);
};


Point Point::operator+(const Point& other) const {
  return {x_ + other.x_, y_ + other.y_};
}

Point& Point::operator*=(int32_t k) {
  x_ *= k;
  y_ *= k;
  return *this;
}
std::istream& operator>>(std::istream& input, Point& point) {
  input >> point.x_ >> point.y_;
  return input;
}

struct Vector {
  double x_ = 0;
  double y_ = 0;
  Vector(const Point& start, const Point& end) : x_(end.x_ - start.x_), y_(end.y_ - start.y_) {}

  double CrossProduct(const Vector& other) const;
};

double Vector::CrossProduct(const Vector & other) const {
  return x_ * other.y_ - y_ * other.x_;
}

class Polygon {
private:
  std::vector<Point> vertices_;

  friend Polygon MinkowskiSum(const Polygon& first, const Polygon& second);
public:
  Polygon() = default;
  Polygon(const std::vector<Point>& other);

  void AddVertex(const Point& another_point);

  bool HasIntersection(const Polygon& other) const;
  bool ConvexPolygonContains(const Point& point) const;

  Polygon GetNormalisedForMinkowski() const;
  Polygon GetScaled(int32_t k) const;
};

static const double EPS = 1e-9;

Polygon::Polygon(const std::vector<Point> &points) : vertices_(points) {}

Polygon Polygon::GetNormalisedForMinkowski() const {
  int32_t bottom_left_point_ind = 0;

  // looking for the most left most bottom point
  for (int32_t i = 1; i < vertices_.size(); ++i) {
    const Point& current_point = vertices_[i];
    const Point& bottom_left_point = vertices_[bottom_left_point_ind];
    if (current_point.y_ < bottom_left_point.y_ ||
        (abs(current_point.y_ - bottom_left_point.y_) < EPS &&
              current_point.x_ < bottom_left_point.x_)) {

      bottom_left_point_ind = i;
    }
  }


  std::vector<Point> result_vertices(vertices_.size());
  for (int32_t i = 0; i < vertices_.size(); ++i) {
    result_vertices[i] = vertices_[(i + bottom_left_point_ind + 1) %
                              vertices_.size()];
  }

  // reverse points in anti-clockwise order
  std::reverse(result_vertices.begin(), result_vertices.end());
  result_vertices.push_back(result_vertices.front());
  return {result_vertices};
}


void Polygon::AddVertex(const Point &another_point) {
  vertices_.push_back(another_point);
}


// scales all points with the k coefficient
Polygon Polygon::GetScaled(int32_t k) const {
  std::vector<Point> result_points;
  for (Point new_point : vertices_) {
    result_points.push_back(new_point *= k);
  }
  return {result_points};
}



Polygon MinkowskiSum(const Polygon& first, const Polygon& second) {
  Polygon result;
  result.AddVertex(first.vertices_[0] + second.vertices_[0]);

  for (int32_t i = 0, j = 0;
       i < first.vertices_.size() && j < second.vertices_.size();) {
    if (i == first.vertices_.size() - 1) {

      result.AddVertex(first.vertices_[i] + \
                       second.vertices_[j + 1]);
      ++j;
    } else if (j == second.vertices_.size() - 1) {

      result.AddVertex(first.vertices_[i + 1] + \
                       second.vertices_[j]);
      ++i;
    } else {

      Point a_1 = first.vertices_[i] + second.vertices_[j + 1];
      Point a_2 = second.vertices_[j] + first.vertices_[i + 1];
      Point prev = result.vertices_.back();
      if (Vector(prev, a_1).CrossProduct(Vector(prev, a_2)) >= 0) {
        result.AddVertex(first.vertices_[i] + second.vertices_[j + 1]);
        ++j;
      } else {
        result.AddVertex(second.vertices_[j] + first.vertices_[i + 1]);
        ++i;
      }
    }
  }
  return result;

}

// takes Convex polygon and some Point;
// returns true if this polygon contains this Point otherwise returns false
bool Polygon::ConvexPolygonContains(const Point& point) const {

  for (int32_t i = 0; i < vertices_.size() - 1; ++i){
    Vector side(vertices_[i], vertices_[i + 1]);
    Vector v_to_point(vertices_[i], point);
    // if it's left rotation than it doesn't contains out point
    if (side.CrossProduct(v_to_point) < 0) {
      return false;
    }
  }
  return true;
}


bool Polygon::HasIntersection(const Polygon& other) const {
  Polygon this_normalised_copy = GetNormalisedForMinkowski();
  Polygon other_normalised_copy = other.GetScaled(-1).GetNormalisedForMinkowski();
  Polygon minkowski_result = MinkowskiSum(this_normalised_copy, other_normalised_copy);
  return minkowski_result.ConvexPolygonContains({0, 0});
}


Polygon GetPolygon(int32_t size) {
  std::vector<Point> vertices(size);
  for (int32_t i = 0; i < size; ++i) {
    std::cin >> vertices[i];
  }
  return {vertices};
}


int main () {
  int32_t n = 0, m = 0;
  std::cin >> n;
  Polygon first = GetPolygon(n);

  std::cin >> m;
  Polygon second = GetPolygon(m);

  std::cout << (first.HasIntersection(second) ? "YES" : "NO" );
  return 0;
}