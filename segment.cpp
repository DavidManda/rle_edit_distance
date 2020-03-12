#include "segment.hpp"
#include <sstream>
#include <assert.h>

Segment::Segment(Point _left, Point _right)
{
  left = _left;
  right = _right;
}

Segment::Segment(){}

bool Segment::contains(int x){
  return left.x <= x && right.x >= x;
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