#ifndef point
#define point

#include <iostream>

enum Point_t { ID, IF, FD, FI, DF, _I, _F, _D, I_, F_, D_ } ;

class Point{
public:
  double x, y;
  Point_t type;
  Point(double _x, double _y);
  Point();
  bool operator ==(const Point& s);
  std::string to_string();
};

#endif