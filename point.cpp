#include "point.hpp"
#include <sstream>

Point::Point(int _x, int _y)
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