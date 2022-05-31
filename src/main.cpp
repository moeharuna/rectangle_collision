#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <cassert>
#include <cstring>
#include <limits>

struct Point {
public:
  Point() = delete;
  Point(int64_t x, int64_t y):x{x}, y{y} {}
  int64_t x, y;
};
std::string to_string(const Point &p) {
  return "("+std::to_string(p.x)+", "+std::to_string(p.y)+")";
}


enum SegementOrentaion {
  CLOCKWISE,
  COUNTERCLOCKWISE,
  COLLINEAR
};

SegementOrentaion orientaion(Point a, Point b, Point c) {
  int64_t check =
    (b.y - a.y) * (c.x - b.x) -
    (b.x - a.x) * (c.y - b.y);
  if(check < 0) return COUNTERCLOCKWISE;
  if(check > 0) return CLOCKWISE;
  return COLLINEAR;
}

struct Line {
public:
  bool is_on_line(Point a) const {
    return a.x <= std::max(p1.x, p2.x) &&
      a.x >= std::min(p1.x, p2.x) &&
      a.y <= std::max(p1.y, p2.y) &&
      a.y >= std::min(p1.y, p2.y);
  }
  Line()  = delete;
  Line(Point a, Point b):p1(a), p2(b) {}
  Point p1, p2;
};



std::string to_string(const Line &l) {
  return to_string(l.p1)+";"+to_string(l.p2);
}



bool do_intersect(const Line &l1, const Line &l2)  {
  SegementOrentaion s1 = orientaion(l1.p1, l1.p2, l2.p1);
  SegementOrentaion s2 = orientaion(l1.p1, l1.p2, l2.p2);

  SegementOrentaion s3 = orientaion(l2.p1, l2.p2, l1.p1);
  SegementOrentaion s4 = orientaion(l2.p1, l2.p2, l1.p2);

  //if collinear but one of the points lies on line
  if(s1==COLLINEAR && l1.is_on_line(l2.p1))
    return true;
  if(s2==COLLINEAR && l1.is_on_line(l2.p2))
    return true;
  if(s3==COLLINEAR && l2.is_on_line(l1.p1))
    return true;
  if(s4==COLLINEAR && l2.is_on_line(l1.p2))
    return true;

  return s1!=s2 && s3!=s4;
}

class Polygon {
  public:
  Polygon() = delete;
  Polygon(std::vector<Point> vertices):vertices_{vertices} {}
  std::vector<Line> edges() const {
    std::vector<Line> result;
    for(size_t i=0; i<vertices_.size()-1; ++i) {
      result.push_back(Line(vertices_[i], vertices_[i+1]));
    }

    //add  edge between first and last vertex
    auto last =vertices_.size()-1;
    result.push_back(Line(vertices_[last], vertices_[0]));

    return result;
  }

  const std::vector<Point>& vertices() const {
    return vertices_;
  }

  uint32_t intersect_count(const Line &l) const {
    uint32_t count = 0;
    for(const auto &line:edges()) {
      if(do_intersect(line, l)) count++;
    }
    return count;
  }

  bool is_point_inside(const Point &p) const {
    //I use int32_t max limit for ray because int64_t limit may oveflow.
    Point check_p = Point(std::numeric_limits<int32_t>::max(), p.y);
    Line ray = Line(p, check_p);
    //if ray casted from point has odd number of collisions it's means that point is inside
    return intersect_count(ray)%2==1;
  }
  uint32_t points_inside_count(const Line &l) const {
    return  is_point_inside(l.p1)+is_point_inside(l.p2);
  }

  private:
  std::vector<Point> vertices_;
};



std::string to_string(const Polygon &p) {
  std::string result ="{\n";
  for(const auto& edge:p.edges()) {
    result+=to_string(edge)+"\n";
  }
  result+="}";
  return result;
}


enum PolygonLineCollisionResult {
  CONTAINS =0,
  CROSS,
  PARTIALLY_CONTAINS,
  OUTSIDE
};
typedef PolygonLineCollisionResult polyResult;
polyResult poly_collision(const Polygon &p, const Line &l) {
  std::vector<Line> poly_edges = p.edges();

  uint32_t collision_count =p.intersect_count(l);
  uint32_t points_inside_count = p.points_inside_count(l);

  if(points_inside_count==2) return CONTAINS;
  if(points_inside_count==1) return PARTIALLY_CONTAINS;
  if(points_inside_count==0 && collision_count>=2) return CROSS;
  return OUTSIDE;
}

std::vector<std::string> str_split(const std::string &str,
                                   const std::string &delim = " ") {
  std::vector<std::string> result{};
  int32_t begin = 0;
  int32_t end = -1;

  while ((end = str.find(delim, begin)) != -1) {
    result.push_back(str.substr(begin, end - begin));
    begin = end+delim.size();
  }

  auto last_str = str.substr(begin, str.size());
  if(last_str!="") {
    result.push_back(last_str);
  }
  return result;
}

Point point_from_str(const std::string& str) {
  auto vec = str_split(str, ",");
  assert(vec.size()==2);
  int64_t x = std::stoi(vec[0]);
  int64_t y = std::stoi(vec[1]);

  return Point(x, y);
}

Line read_line(std::ifstream &file) {
  std::string line_str;
  std::getline(file, line_str);
  std::vector<std::string> points = str_split(line_str);
  assert(points.size()==2);
  return Line(point_from_str(points.at(0)),
              point_from_str(points.at(1)));
}

std::vector<Line> read_lines() {
  std::ifstream lines_file("lines.txt");
  std::vector<Line> result;
  while(lines_file.good()) {
    std::string line_str;
    std::getline(lines_file, line_str);

    if(line_str=="") continue;

    std::vector<std::string> points = str_split(line_str);
    assert(points.size()==2);

    result.push_back(Line(point_from_str(points.at(0)),
                          point_from_str(points.at(1))));
  }
  return result;
}

std::vector<Polygon> read_polys() {
  std::ifstream polys_file("polys.txt");
  std::vector<Polygon> result{};

  while(polys_file.good()) {
    std::string poly_string;
    std::getline(polys_file, poly_string);

    if(poly_string=="") {
      continue;
    }

    std::vector<std::string> vertices = str_split(poly_string);
    std::vector<Point> vertices_vec;

    for(const auto &v:vertices) {
      vertices_vec.push_back(point_from_str(v));
    }

    result.push_back(Polygon(vertices_vec));
  }
  return result;
}

std::string result_to_string(int result) {
  std::string result_string;
  switch(result) {
  case CONTAINS:
    result_string = "inside";
    break;
  case CROSS:
    result_string = "crossing";
    break;
  case PARTIALLY_CONTAINS:
    result_string = "partilly crossing";
    break;
  case OUTSIDE:
    result_string = "outside";
    break;
  }
  return result_string;
}



int main(int argc, char *argv[]) {
  std::vector<Polygon> polys=  read_polys();
  std::vector<Line> lines = read_lines();
  std::ofstream out_file("results.txt");
  if(lines.size() != polys.size()) {
    std::cout<<"Wrong input. number of lines in lines.txt should match number of polygons in polys.txt";
    return 1;
  }
  for(size_t i=0; i<lines.size(); ++i) {

    int result = poly_collision(polys[i], lines[i]);
    #ifdef DEBUG_INFO
    std::cout<<"Case "<<i<<"\n";
    std::cout<<to_string(polys[i])<<"\n";
    std::cout<<to_string(lines[i])<<"\n";
    std::cout<<"Result: "<<result_to_string(result)<<"\n";
    std::cout<<"\n\n\n";
    #endif
    out_file<<result<<"\n";
  }
  return 0;
}
