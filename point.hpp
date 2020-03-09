#ifndef point
#define point

#include <iostream>

class Point{
public:
  double x, y;
  Point(double _x, double _y);
  Point();
  bool operator ==(const Point& s);
  std::string to_string();
};

#endif