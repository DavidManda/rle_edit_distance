#ifndef point
#define point

#include <iostream>

class Point{
public:
  float x, y;
  Point(float _x, float _y);
  std::string to_string();
};

#endif