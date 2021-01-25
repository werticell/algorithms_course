#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>


struct Point3 {
  int32_t x_;
  int32_t y_;
  int32_t z_;
  int32_t id_;
  Point3() = default;
  Point3(int32_t x, int32_t y, int32_t z): x_(x), y_(y), z_(z), id_(-1) {}
  Point3(int32_t x, int32_t y, int32_t z, int32_t id): x_(x), y_(y), z_(z), id_(id) {}


  Point3 operator+(const Point3 &other) const;
  Point3 operator-(const Point3 &other) const;

  bool operator==(const Point3 & other) const;
  bool operator!=(const Point3 & other) const;
};

std::istream& operator>>(std::istream& input, Point3& point) {
  input >> point.x_ >> point.y_ >>point.z_;
  return input;
}

bool Point3::operator==(const Point3 & other) const {
  return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
}

bool Point3::operator!=(const Point3 & other) const {
  return !(*this == other);
}

Point3 Point3::operator+(const Point3 &other) const {
  return Point3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
}

Point3 Point3::operator-(const Point3 &other) const {
  return Point3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}


struct HashPoint {
  std::size_t operator() (const Point3 & p) const
  {
    return std::hash<std::string>{}(std::to_string(p.x_) + \
                                    std::to_string(p.y_) + \
                                    std::to_string(p.z_));
  }
};

int8_t VolumeSign(const Point3& p1, const Point3& p2, const Point3& p3, const Point3 & p) {
  int64_t vol;
  Point3 a = p1 - p;
  Point3 b = p2 - p;
  Point3 c = p3 - p;
  vol = a.x_ * (b.y_ * c.z_ - b.z_ * c.y_) +
      a.y_ * (b.z_ * c.x_ - b.x_ * c.z_) +
      a.z_ * (b.x_ * c.y_ - b.y_ * c.x_);
  return (vol == 0) ? 0 : (vol > 0 ? 1 : -1);
}

struct Face {
  bool is_visible;
  Point3 vertices[3];
  Face(const Point3 & p1, const Point3 & p2,
       const Point3 & p3): vertices{p1, p2, p3}, is_visible(false) {}

  void ChangeNormalOrientation();

  friend std::ostream& operator<<(std::ostream& os, const Face& f);

  void Normalise();

  bool operator<(const Face& other) const;
};

std::ostream& operator<<(std::ostream& os, const Face& f) {
  os << "3 " << f.vertices[0].id_ << " " << f.vertices[1].id_ << \
        " "  << f.vertices[2].id_;
  return os;
}

void Face::ChangeNormalOrientation() {
  std::swap(vertices[0], vertices[2]);
}

void Face::Normalise() {
  auto min_point_it =
      std::min_element(std::begin(vertices), std::end(vertices),
                       [] (const Point3& p1, const Point3& min)
                       {
                         return p1.id_ < min.id_;
                       });

  int32_t min_id = min_point_it->id_;
  while (vertices[0].id_ != min_id) {
    std::rotate(std::begin(vertices),
                std::begin(vertices) + 1, std::end(vertices));
  }
}

bool Face::operator<(const Face& other) const {
  if (vertices[0].id_ == other.vertices[0].id_) {
    if (vertices[1].id_ == other.vertices[1].id_) {
      if (vertices[2].id_ == other.vertices[2].id_) {
        return false;
      } else {
        return vertices[2].id_ < other.vertices[2].id_;
      }
    } else {
      return vertices[1].id_ < other.vertices[1].id_;
    }
  } else {
    return vertices[0].id_ < other.vertices[0].id_;
  }
}

struct Edge {
  bool to_be_deleted;
  Face *adjacent_face1, *adjacent_face2;
  Point3 points[2];

  Edge() = default;
  Edge(const Point3 & p1, const Point3 & p2): points{p1, p2},
                                             adjacent_face1(nullptr),
                                             adjacent_face2(nullptr),
                                             to_be_deleted(false) {}

  void AddAdjacentFace(Face* face);
  void DeleteAdjacentFace(Face* face);
  Face* GetAdjacentFace() const;


};

size_t HashEdge(const Edge& edge) {
  return HashPoint{}(edge.points[0]) ^ HashPoint{}(edge.points[1]);
}
Face* Edge::GetAdjacentFace() const {
  return (adjacent_face1 == nullptr) ? adjacent_face2 : adjacent_face1;
}

void Edge::AddAdjacentFace(Face* face) {
  (adjacent_face1 == nullptr ? adjacent_face1 : adjacent_face2) = face;
}

void Edge::DeleteAdjacentFace(Face* face)
{
  if(adjacent_face1 != face && adjacent_face2 != face) return;
  (adjacent_face1 == face ? adjacent_face1 : adjacent_face2) = nullptr;
}


class ConvexHullBuilder {
private:
  std::list<Face> faces_in_cvh;
  std::vector<Point3> points_;
  std::map<size_t, Edge> edges_in_cvh;

public:
  explicit ConvexHullBuilder(const std::vector<Point3>& points);
  void PrintConvexHull();

private:
  void CreateEdge(const Point3& p1, const Point3& p2, Face* adjacent_face);


  void Initialise();
  void ExtendConvexHull(const Point3 &new_point);
  void AddNewFace(const Point3 & a, const Point3 & b,
                  const Point3 & c, const Point3 &point_in_cvh);
  static Point3 FindPointInCurrentConvexHull(const Face* cur_face, const Edge&edge);
  void RemoveVisiblePart();
};



ConvexHullBuilder::ConvexHullBuilder(const std::vector<Point3>& points) : points_(points) {
  Initialise();
  for (int32_t i = 4; i < points.size(); ++i) {
    ExtendConvexHull(points[i]);
    RemoveVisiblePart();
  }
}

void ConvexHullBuilder::Initialise() {
  Point3& p1 = points_[0];
  Point3& p2 = points_[1];
  Point3& p3 = points_[2];
  Point3& p4 = points_[3];

  AddNewFace(p1, p2, p3, p4);
  AddNewFace(p1, p2, p4, p3);
  AddNewFace(p1, p3, p4, p2);
  AddNewFace(p2, p3, p4, p1);
}

void ConvexHullBuilder::CreateEdge(const Point3 &p1, const Point3 &p2, Face* adjacent_face_pt) {
  size_t key = HashEdge({p1, p2});
  if(edges_in_cvh.find(key) == edges_in_cvh.end()) {
    edges_in_cvh.emplace(key, Edge(p1, p2));
  }
  edges_in_cvh[key].AddAdjacentFace(adjacent_face_pt);
}

void ConvexHullBuilder::AddNewFace(const Point3 & a, const Point3 & b,
                            const Point3 & c, const Point3 &point_in_cvh) {
  // if normal of face in current order facing in the convex hull then we should swap
  Face new_face (a, b, c);
  if(VolumeSign(new_face.vertices[0], new_face.vertices[1],
                 new_face.vertices[2], point_in_cvh) < 0)  {
    new_face.ChangeNormalOrientation();
  }
  new_face.Normalise();
  faces_in_cvh.push_back(new_face);

  // creating edges with the adjacent faces
  CreateEdge(a, b, &faces_in_cvh.back());
  CreateEdge(a, c, &faces_in_cvh.back());
  CreateEdge(b, c, &faces_in_cvh.back());
}


Point3
ConvexHullBuilder::FindPointInCurrentConvexHull(const Face*cur_face, const Edge&edge) {
  for (const Point3& face_point : cur_face->vertices) {
    if(face_point != edge.points[0] && face_point != edge.points[1])  {
      return face_point;
    }
  }
  return {};
}

void ConvexHullBuilder::ExtendConvexHull(const Point3 &new_point) {
  // finding the horizon and the planes to remove after all marks
  bool is_any_visible = false;
  for(auto&new_face : faces_in_cvh) {
    if(VolumeSign(new_face.vertices[0], new_face.vertices[1],
                   new_face.vertices[2], new_point) < 0) {
      new_face.is_visible = true;
      is_any_visible = true;
    }
  }
  if(!is_any_visible) {
    return;
  }

  // looking throw all edges that we have in order to find horizon
  // edges and edges which both adjacent faces are visible (they will be removed)
  for (auto& [key, edge] : edges_in_cvh) {
    Face* face1 = edge.adjacent_face1;
    Face* face2 = edge.adjacent_face2;

    if(face1 == nullptr || face2 == nullptr) {
      continue;
    } else if(face1->is_visible && face2->is_visible) {
      edge.to_be_deleted = true;
    } else if(face1->is_visible || face2->is_visible) {

      if(face2->is_visible) {
        // to make sure that face1 is the visible one
        std::swap(face1, face2);
      }
      // we should delete the visible face and add a new one with out new_point
      edge.DeleteAdjacentFace(face1);
      AddNewFace(edge.points[0], edge.points[1], new_point,
                 FindPointInCurrentConvexHull(face1, edge));
    }
  }
}


void ConvexHullBuilder::RemoveVisiblePart() {
  for (auto it = edges_in_cvh.begin(); it != edges_in_cvh.end();) {
    it = (it->second.to_be_deleted) ? edges_in_cvh.erase(it) : ++it;
  }
  for (auto it = faces_in_cvh.begin(); it != faces_in_cvh.end();) {
    it = (it->is_visible) ? faces_in_cvh.erase(it) : ++it;
  }
}


void ConvexHullBuilder::PrintConvexHull() {
  std::cout << faces_in_cvh.size() << '\n';
  faces_in_cvh.sort();
  for(const auto & new_face : faces_in_cvh) {
      std::cout<< new_face << '\n';
  }
}

int main () {
  int32_t m = 0;
  std::cin >> m;

  for (int32_t j = 0; j < m; ++j) {
    int32_t n = 0;
    std::cin >> n;

    std::vector<Point3> points(n);
    for (int32_t i = 0; i < n; ++i) {
      std::cin >> points[i];
      points[i].id_ = i;
    }
    ConvexHullBuilder cvh_builder(points);
    cvh_builder.PrintConvexHull();
  }
  return 0;
}
