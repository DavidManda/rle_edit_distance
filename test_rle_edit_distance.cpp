#include "rle_edit_distance.hpp"
#include <assert.h>

void test_get_val_at_coord(){
  Point p1(0,0), p2(10,0);
  float coord = 5;
  float val = get_val_at_coord(coord, p1, p2);
  assert(val == 0);
  
  p1 = Point(0,0);
  p2 = Point(10,10);
  coord = 5;
  val = get_val_at_coord(coord, p1, p2);
  assert(val == 5);

  p1 = Point(0,0);
  p2 = Point(10,10);
  coord = 5.5;
  val = get_val_at_coord(coord, p1, p2);
  assert(val == 5.5);

  p1 = Point(0,0);
  p2 = Point(1,0);
  coord = 0;
  val = get_val_at_coord(coord, p1, p2);
  assert(val == 0);
}

void test_join_empty_trees(){
  BST t_l,t_r, t;
  t = join(t_l, t_r);

  assert(t.root == NULL);
}

void test_join_left_empty(){
  BST t_l,t_r,t;
  Segment s_0011 = Segment(Point(0,0), Point(1,1));
  t_r.insert(s_0011);
  t = join(t_l, t_r);

  assert(t.root->segm == s_0011);
}

void test_join_right_empty(){
  BST t_l,t_r,t;
  Segment s_0011(Point(0,0), Point(1,1));
  t_l.insert(s_0011);
  t = join(t_l, t_r);

  assert(t.root->segm == s_0011);
}

void test_join_same_gradient(){
  BST t_l,t_r,t;
  Segment s_0011(Point(0,0), Point(1,1));
  Segment s_1122(Point(1,1), Point(2,2));
  t_l.insert(s_0011);
  t_r.insert(s_1122);
  t = join(t_l, t_r);

  // check that segments were joined as they have the same gradient
  assert(t.root->segm == Segment(Point(0,0), Point(2,2)));

}

void test_join_different_gradient(){
  BST t_l,t_r,t;
  Segment s_0011(Point(0,0), Point(1,1));
  Segment s_1121(Point(1,1), Point(2,1));
  Segment s_2133(Point(2,1), Point(3,1));
  t_l.insert(s_0011);
  t_r.insert(s_1121);
  t_r.insert(s_2133);
  t = join(t_l, t_r);

  assert(t.root->segm == s_1121);
  assert(t.root->left->segm == s_0011);
  assert(t.root->right->segm == s_2133);
}

void test_join(){
  test_join_empty_trees();
  test_join_left_empty();
  test_join_right_empty();
  test_join_same_gradient();
  test_join_different_gradient();
}

void test_point_types_split(){
  BST t;
  Segment s(Point(0,0), Point(10,10));
  t = BST(new TreeNode(s));
  assert(t.root->type_l == _I);
  assert(t.root->type_r == I_);
  std::pair<BST, BST> pair = split(t, 5);
  BST t_l = pair.first;
  BST t_r = pair.second;
  assert(t_l.root->type_l = _I);
  assert(t_l.root->type_r = I_);
  assert(t_r.root->type_l = _I);
  assert(t_r.root->type_r = I_);
}

void test_point_types_join_same_grad(){
  BST t1 = BST(new TreeNode(Segment(Point(0,0),Point(10,10))));
  BST t2 = BST(new TreeNode(Segment(Point(10,10),Point(20,20))));
  BST t = join(t1, t2);
  assert(t.root->segm == Segment(Point(0,0),Point(20,20)));
  assert(t.root->type_l == _I);
  assert(t.root->type_r == I_);
}

void test_point_types_join_diff_grad(){
  BST t1 = BST(new TreeNode(Segment(Point(0,0),Point(10,10))));
  BST t2 = BST(new TreeNode(Segment(Point(10,10),Point(20,10))));
  BST t = join(t1, t2);
  TreeNode *left = TreeNode::min(t.root);
  TreeNode *right = TreeNode::max(t.root);
  assert(left->type_l == _I);
  assert(left->type_r == IF);
  assert(right->type_l == IF);
  assert(right->type_r == F_);
  BST t3 = BST(new TreeNode(Segment(Point(20,10),Point(30,0))));
  BST tt = join(t, t3);
  assert(tt.root->left->type_l == _I);
  assert(tt.root->left->type_r == IF);
  assert(tt.root->type_l == IF);
  assert(tt.root->type_r == FD);
  assert(tt.root->right->type_l == FD);
  assert(tt.root->right->type_r == D_);
}

void test_point_types_join(){
  test_point_types_join_same_grad();
  test_point_types_join_diff_grad();
}

void test_point_types(){
  test_point_types_split();
  test_point_types_join();
}

void test_rle_edit_distance(){
  test_get_val_at_coord();
  test_join();
  test_point_types();
}
