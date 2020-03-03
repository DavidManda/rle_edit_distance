#include "segment.hpp"
#include <sstream>

Segment::Segment(Point _left, Point _right)
{
  left = _left;
  right = _right;
}

Segment::Segment(){}

bool Segment::contains(int x){
  return left.x <= x && right.x >= x;
}

bool Segment::operator <(const Segment& s){
  return left.x < s.left.x && right.x <= s.left.x;
}

bool Segment::operator >(const Segment& s){
  return right.x > s.right.x && left.x >= s.right.x;
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