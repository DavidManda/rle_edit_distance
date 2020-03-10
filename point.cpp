#include "point.hpp"
#include <sstream>
#include <cmath>

Point::Point(double _x, double _y)
{
  x = _x;
  y = _y;
}

Point::Point(){}

int Point::get_manhattan(Point p1, Point p2){
  return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

std::string Point::to_string()
{
  std::stringstream ss;
  ss << "(" << x << ", " << y << ")";
  return ss.str();
}

bool Point::operator ==(const Point& s){
  return x == s.x && y == s.y;
}