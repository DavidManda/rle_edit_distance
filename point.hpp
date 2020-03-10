#ifndef point
#define point

#include <iostream>

enum Point_t { NotInitialised, DI, ID, IF, FD, FI, DF, _I, _F, _D, I_, F_, D_ } ;

class Point{
public:
  double x, y;
  Point(double _x, double _y);
  Point();
  static int get_manhattan(Point p1, Point p2);
  bool operator ==(const Point& s);
  std::string to_string();
};

#endif