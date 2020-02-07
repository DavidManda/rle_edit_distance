#include "point.hpp"
#include <sstream>

Point::Point(float _x, float _y)
{
  x = _x;
  y = _y;
}

std::string Point::to_string()
{
  std::stringstream ss;
  ss << "(" << x << ", " << y << ")";
  return ss.str();
}