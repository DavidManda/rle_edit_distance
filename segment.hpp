#ifndef segment
#define segment

#include "point.hpp"
#include <iostream>

class Segment{
public:
  Point left, right;
  Segment(Point _left, Point _right);
  Segment();
};

#endif