#ifndef segment
#define segment

#include "point.hpp"
#include <iostream>

class Segment{
public:
  Point left, right;
  Segment(Point _left, Point _right);
  Segment();
  float get_val_at_coord(float x);
  int get_slope();
  bool contains(float x);
  static Point get_intersection(Segment s1, Segment s2);
  bool operator <(const Segment& s);
  bool operator >(const Segment& s);
  bool operator <=(const Segment& s);
  bool operator >=(const Segment& s);
  bool operator ==(const Segment& s);
  std::string to_string();
};

#endif