#include <iostream>
#include <vector>

struct Point {
  int32_t x_ = 0;
  int32_t y_ = 0;
};

std::istream& operator>>(std::istream& input, Point& point) {
  input >> point.x_ >> point.y_;
  return input;
}

struct Vector {
  int32_t x_ = 0;
  int32_t y_ = 0;
  Vector(const Point& start, const Point& end) : x_(end.x_ - start.x_), y_(end.y_ - start.y_) {}

  int64_t CrossProduct(const Vector& other) const;
};

int64_t Vector::CrossProduct(const Vector & other) const {
  return x_ * other.y_ - y_ * other.x_;
}

struct Segment {
  Point first_;
  Point second_;

  Segment(const Point& first, const Point& second) : first_(first),
                                                     second_(second) {}

  bool HasIntersection(const Segment& other) const;


};

bool BoundCheck(Segment s1, Segment s2) {
  if (s1.first_.x_ > s1.second_.x_)
    std::swap(s1.first_.x_, s1.second_.x_);
  if (s2.first_.x_ > s2.second_.x_)
    std::swap(s2.first_.x_, s2.second_.x_);
  if (s1.first_.y_ > s1.second_.y_)
    std::swap(s1.first_.y_, s1.second_.y_);
  if (s2.first_.y_ > s2.second_.y_)
    std::swap(s2.first_.y_, s2.second_.y_);

  return std::max(s1.first_.x_, s2.first_.x_) <= std::min(s1.second_.x_, s2.second_.x_) && \
      std::max(s1.first_.y_, s2.first_.y_) <= std::min(s1.second_.y_, s2.second_.y_);


}

bool Segment::HasIntersection(const Segment & other) const {
  Vector this_vector(first_, second_);
  Vector other_vector(other.first_, other.second_);
  return this_vector.CrossProduct(Vector(first_, other.first_)) * \
         this_vector.CrossProduct(Vector(first_, other.second_)) <= 0 && \
         other_vector.CrossProduct(Vector(other.first_, first_)) * \
         other_vector.CrossProduct(Vector(other.first_, second_)) <= 0 && \
         BoundCheck(*this, other);
}




uint32_t CountSegmentIntersections(const Segment& main_segment,
                                   const std::vector<Segment>& segments) {
  uint32_t intersection_count = 0;
  for (const Segment& other : segments) {
    if (main_segment.HasIntersection(other)) {
      ++intersection_count;
    }
  }
  return intersection_count;
}


int main() {
  Point A;
  Point B;
  std::cin >> A >> B;
  uint32_t N = 0;
  std::cin >> N;

  std::vector<Segment> segments;
  Point p1, p2;
  for (uint32_t i = 0; i < N; ++i) {
    std::cin >> p1 >> p2;
    segments.emplace_back(p1, p2);
  }

  std::cout << CountSegmentIntersections(Segment(A, B), segments);

  return 0;
}
