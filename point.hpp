#ifndef point
#define point

#include <iostream>

class Point{
public:
  float x, y;
  Point(float _x, float _y);
  Point();
  bool operator ==(const Point& s);
  std::string to_string();
};

#endif