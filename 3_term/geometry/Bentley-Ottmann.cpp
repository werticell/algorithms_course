#include <algorithm>
#include <iostream>
#include <optional>
#include <set>
#include <vector>


struct Point {
  int32_t x_ = 0;
  int32_t y_ = 0;
  Point(int32_t x, int32_t y) : x_(x), y_(y) {}
  Point() = default;
};

std::istream& operator>>(std::istream& input, Point& point) {
  input >> point.x_ >> point.y_;
  return input;
}

struct Vector {
  int32_t x_ = 0;
  int32_t y_ = 0;
  Vector(const Point& start, const Point& end) : x_(end.x_ - start.x_),
                                                 y_(end.y_ - start.y_) {}

  int64_t CrossProduct(const Vector& other) const;
};

int64_t Vector::CrossProduct(const Vector & other) const {
  return x_ * other.y_ - y_ * other.x_;
}

struct Segment {
  Point first_;
  Point second_;
  int32_t index_in_array = 0;
  Segment() = default;
  Segment(const Point& first, const Point& second) : first_(first),
                                                     second_(second) {}

  bool HasIntersection(const Segment& other) const;
  double GetFunctionValue(int32_t x) const;

  bool operator<(const Segment& other) const;
};


bool Segment::operator<(const Segment & other) const {
  int32_t max_segment_start = std::max(std::min(first_.x_, second_.x_),
                                       std::min(other.first_.x_, other.second_.x_));
  // ordering segments by the y value of their start point
  return other.GetFunctionValue(max_segment_start) > GetFunctionValue(max_segment_start);
}

double Segment::GetFunctionValue(int32_t x) const {
  if (first_.x_ == second_.x_) {
    return first_.y_;
  } else {
    return first_.y_ +
           static_cast<double>(((x - first_.x_) * (first_.y_ - second_.y_)))/ (first_.x_ - second_.x_);
  }
}

std::istream& operator>>(std::istream& input, Segment& segment) {
  input >> segment.first_.x_>> segment.first_.y_ >>
           segment.second_.x_>> segment.second_.y_;
  return input;
}

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


struct Event{
  enum EventType : int8_t {
    SEGMENT_START        = 1,
    SEGMENT_END          = -1,
    UNDEFINED            = 0
  };
  int32_t x_ = 0;
  int32_t segment_index_in_array_ = 0;
  EventType type_ = UNDEFINED;

  Event() = default;
  Event(int32_t x, EventType type, int32_t index) : x_(x), segment_index_in_array_(index),
  type_(type) {}

  bool operator<(const Event& other) const;
};

bool Event::operator<(const Event & other) const {
  if (x_ == other.x_) {
    return type_ > other.type_;
  } else {
    return x_ < other.x_;
  }
}


// mostly to get the copy for iterator
std::set<Segment>::iterator
GetPreviousSegmentIterator(const std::set<Segment>& set,
                           std::set<Segment>::iterator it) {

  return it == set.begin() ? set.end() : --it;
}

std::set<Segment>::iterator
GetNextSegmentIterator(const std::set<Segment>& set,
                       std::set<Segment>::iterator it) {

  return ++it;
}



std::optional<std::pair<int32_t, int32_t>>
HasIntersection(const std::vector<Segment>& segments) {
  std::set<Segment> processing_segments;
  std::vector<Event> events;

  for (size_t i = 0; i < segments.size(); ++i) {
    events.emplace_back(std::min(segments[i].first_.x_, segments[i].second_.x_),
                  Event::SEGMENT_START, i);

    events.emplace_back(std::max(segments[i].first_.x_, segments[i].second_.x_),
                  Event::SEGMENT_END, i);
  }
  std::sort(events.begin(), events.end());


  for (const Event& new_event: events) {
    // index of segment event in segment vector
    int32_t index = new_event.segment_index_in_array_;

    if (new_event.type_ == Event::SEGMENT_START) {
      auto next_segment_it = processing_segments.lower_bound(segments[index]);
      auto prev_segment_it = GetPreviousSegmentIterator(processing_segments, next_segment_it);

      if (next_segment_it != processing_segments.end() &&
          next_segment_it->HasIntersection(segments[index])) {
        return std::make_pair(next_segment_it->index_in_array, index);
      }
      if (prev_segment_it != processing_segments.end() &&
          prev_segment_it->HasIntersection(segments[index])) {
        return std::make_pair(prev_segment_it->index_in_array, index);
      }
      processing_segments.insert(segments[index]);
    } else {

      auto segment_start_it = processing_segments.find(segments[index]);
      auto next_segment_it = GetNextSegmentIterator(processing_segments, segment_start_it);
      auto prev_segment_it = GetPreviousSegmentIterator(processing_segments, segment_start_it);

      if (next_segment_it != processing_segments.end() &&
          prev_segment_it != processing_segments.end() &&
          next_segment_it->HasIntersection(*prev_segment_it)) {
        return std::make_pair(next_segment_it->index_in_array,
                              prev_segment_it->index_in_array);
      }
      processing_segments.erase(segment_start_it);
    }
  }
  return std::nullopt;
}



int main() {
  int32_t n = 0;
  std::cin >> n;

  std::vector<Segment> segments(n);
  for (int32_t i = 0; i < n; ++i) {
    std::cin >> segments[i];
    segments[i].index_in_array = i;
  }
  std::optional<std::pair<int, int>> result = HasIntersection(segments);
  if (result) {
    std::cout << "YES" << '\n' ;
    std::cout << std::min(result->first + 1, result->second + 1) << " ";
    std::cout << std::max(result->first + 1, result->second + 1);
  } else {
    std::cout << "NO";
  }

}