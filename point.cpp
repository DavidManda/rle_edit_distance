#include "point.hpp"
#include <sstream>
#include <cmath>

Point::Point(double _x, double _y){
  x = _x;
  y = _y;
  type = NotInitialised;
}

Point::Point(double _x, double _y, Point_t _type) : Point(_x, _y) {
  type = _type;
}

Point::Point(){}

int Point::get_manhattan(Point p1, Point p2){
  return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

std::ostream& operator<<(std::ostream& out, const Point_t value){
  const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
  switch(value){
    PROCESS_VAL(NotInitialised);     
    PROCESS_VAL(DI);     
    PROCESS_VAL(ID);
    PROCESS_VAL(IF);     
    PROCESS_VAL(FD);     
    PROCESS_VAL(FI);
    PROCESS_VAL(DF);     
    PROCESS_VAL(_I);     
    PROCESS_VAL(_F);
    PROCESS_VAL(_D);
    PROCESS_VAL(I_);     
    PROCESS_VAL(F_);     
    PROCESS_VAL(D_);
  }
#undef PROCESS_VAL

    return out << s;
}

std::string Point::to_string()
{
  std::stringstream ss;
  ss << "(" << x << ", " << y << ") " << type <<' ';
  return ss.str();
}

bool Point::operator ==(const Point& s){
  return x == s.x && y == s.y;
}


Point& Point::operator +=(const Point& p){
  this->x += p.x;
  this->y += p.y;
  return *this;
}