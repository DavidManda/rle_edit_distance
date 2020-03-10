#ifndef segment
#define segment

#include "point.hpp"
#include <iostream>

class Segment{
public:
  Point left, right;
  Segment(Point _left, Point _right);
  Segment();
  bool contains(int x);
  bool operator <(const Segment& s);
  bool operator >(const Segment& s);
  bool operator <=(const Segment& s);
  bool operator >=(const Segment& s);
  bool operator ==(const Segment& s);
  std::string to_string();
};

#endif