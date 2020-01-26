#ifndef point
#define point

#include <iostream>

class Point{
public:
  int x, y;
  Point(int _x, int _y);
  std::string to_string();
};

#endif