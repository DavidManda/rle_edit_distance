#include "segment.hpp"
#include <sstream>
#include <assert.h>

Segment::Segment(Point _left, Point _right)
{
  left = _left;
  right = _right;
  update_endpoints();
}

Segment::Segment() : Segment(Point(0,0), Point(0,0)){}

void Segment::update_endpoints(){
  if(left.x == right.x){
    left.type = _F;
    right.type = F_;
  }
  else{
    int slope = (right.y - left.y)/(right.x - left.x);
    assert(slope == 1 || slope == 0 || slope == -1);

    if(slope == 1){
      left.type = _I;
      right.type = I_;
    }
    if(slope == -1){
      left.type = _D;
      right.type = D_;
    }
    if(slope == 0){
      left.type = _F;
      right.type = F_;
    }
  }
}

Point_t Segment::get_new_point_type(int dg, Point_t type){
  assert(type != ID && type != DI);
  assert(!((type == _D || type == D_ || type == FD || type == DF) && dg == -1));
  assert(!((type == _I || type == I_ || type == FI || type == IF) && dg == 1));

  if(type == _I && dg == -1){
    return _F;
  }

  if(type == I_ && dg == -1){
    return F_;
  }

  if(type == _F && dg == -1){
    return _D;
  }

  if(type == F_ && dg == -1){
    return D_;
  }

  if(type == IF && dg == -1){
    return FD;
  }

  if(type == FI && dg == -1){
    return DF;
  }
  
  if(type == _D && dg == 1){
    return _F;
  }

  if(type == D_ && dg == 1){
    return F_;
  }

  if(type == _F && dg == 1){
    return _I;
  }

  if(type == F_ && dg == 1){
    return I_;
  }

  if(type == DF && dg == 1){
    return FI;
  }

  if(type == FD && dg == 1){
    return IF;
  }
  return type;
}

void Segment::change_gradient(int dg){
  left.y += left.x * dg;
  right.y += right.x * dg;

  left.type = get_new_point_type(dg, left.type);
  right.type = get_new_point_type(dg, right.type);
}

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
  ss << '[' <<this->left.type<<this->left.to_string()<<'-'<<this->right.to_string()<<this->right.type<<"]";
  // ss<<this->left.x;
  return ss.str();
}