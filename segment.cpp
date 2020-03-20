#include "segment.hpp"
#include <sstream>
#include <assert.h>

Segment::Segment(Point _left, Point _right)
{
  left = _left;
  right = _right;
}

Segment::Segment(){}

bool Segment::contains(float x){
  return left.x <= x && right.x >= x;
}

int Segment::get_slope(){
  assert(this->left.x != this->right.x);
  return (this->right.y - this->left.y)/(this->right.x - this->left.x);
}

float Segment::get_val_at_coord(float x){
  Point p1 = this->left;
  Point p2 = this->right;
  assert(p1.x <= x && x <= p2.x);
  if(p1.x == p2.x){
    assert(p1.y == p2.y);
    return p1.y;
  }
  float slope = (p2.y - p1.y)/(p2.x - p1.x);
  
  return p1.y + slope * (x - p1.x);
}

Point Segment::get_intersection(Segment s1, Segment s2){
  Point a1 = s1.left, a2 = s1.right, b1 = s2.left, b2 = s2.right;
  float slope_1 = (a2.y - a1.y)/(a2.x - a1.x);
  float slope_2 = (b2.y - b1.y)/(b2.x - b1.x);
  assert(slope_1 != slope_2);

  float x = (b1.y - a1.y + slope_1*a1.x - slope_2*b1.x)/(slope_1-slope_2);
  float y = a1.y + (x - a1.x) * slope_1;
  return Point(x, y);
}

bool Segment::operator <(const Segment& s){
  return left.x < s.left.x && right.x <= s.left.x;
}

bool Segment::operator >(const Segment& s){
  return right.x > s.right.x && left.x >= s.right.x;
}

bool Segment::operator <=(const Segment& s){
  return left.x <= s.left.x && right.x <= s.left.x;
}

bool Segment::operator >=(const Segment& s){
  return right.x >= s.right.x && left.x >= s.right.x;
}

bool Segment::operator ==(const Segment& s){
  return left.x == s.left.x && right.x == s.right.x;
}

std::string Segment::to_string(){
  std::stringstream ss;
  ss << '[' << this->left.to_string()<<'-'<<this->right.to_string()<<']';
  // ss<<this->left.x;
  return ss.str();
}