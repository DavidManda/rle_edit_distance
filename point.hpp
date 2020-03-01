#ifndef point
#define point

#include <iostream>

class Point{
public:
  float x, y;
  Point(float _x, float _y);
  Point();
  std::string to_string();
};

#endif