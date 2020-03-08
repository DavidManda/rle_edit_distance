#include "point.hpp"
#include <sstream>

Point::Point(float _x, float _y)
{
  x = _x;
  y = _y;
}

Point::Point(){}

std::string Point::to_string()
{
  std::stringstream ss;
  ss << "(" << x << ", " << y << ")";
  return ss.str();
}

bool Point::operator ==(const Point& s){
  return x == s.x && y == s.y;
}