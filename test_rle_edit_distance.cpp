#include "rle_edit_distance.hpp"
#include <assert.h>

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
  assert(t.root->segm.left.type == _I);
  assert(t.root->segm.right.type == I_);
  std::pair<BST, BST> pair = split(t, 5);
  BST t_l = pair.first;
  BST t_r = pair.second;
  assert(t_l.root->segm.left.type = _I);
  assert(t_l.root->segm.right.type = I_);
  assert(t_r.root->segm.left.type = _I);
  assert(t_r.root->segm.right.type = I_);
}

void test_point_types_join_same_grad(){
  BST t1 = BST(new TreeNode(Segment(Point(0,0),Point(10,10))));
  BST t2 = BST(new TreeNode(Segment(Point(10,10),Point(20,20))));
  BST t = join(t1, t2);
  assert(t.root->segm == Segment(Point(0,0),Point(20,20)));
  assert(t.root->segm.left.type == _I);
  assert(t.root->segm.right.type == I_);
}

void test_point_types_join_diff_grad(){
  BST t1 = BST(new TreeNode(Segment(Point(0,0),Point(10,10))));
  BST t2 = BST(new TreeNode(Segment(Point(10,10),Point(20,10))));
  BST t = join(t1, t2);
  TreeNode *left = TreeNode::min(t.root);
  TreeNode *right = TreeNode::max(t.root);
  assert(left->segm.left.type == _I);
  assert(left->segm.right.type == IF);
  assert(right->segm.left.type == IF);
  assert(right->segm.right.type == F_);
  BST t3 = BST(new TreeNode(Segment(Point(20,10),Point(30,0))));
  BST tt = join(t, t3);
  assert(tt.root->left->segm.left.type == _I);
  assert(tt.root->left->segm.right.type == IF);
  assert(tt.root->segm.left.type == IF);
  assert(tt.root->segm.right.type == FD);
  assert(tt.root->right->segm.left.type == FD);
  assert(tt.root->right->segm.right.type == D_);
}

void test_join_with_DI_type(){
  BST t1 = BST(new TreeNode(Segment(Point(0,10),Point(10,0))));
  BST t2 = BST(new TreeNode(Segment(Point(10,0),Point(20,10))));
  BST t = join(t1, t2);
  TreeNode *left = t.find(Segment(Point(0,10),Point(10,0)));
  TreeNode *right = t.find(Segment(Point(10,0),Point(20,10)));
  TreeNode *empty_node = t.find(Segment(Point(10,10),Point(10,10)));
  assert(left->segm.left.type == _D);
  assert(left->segm.right.type == DF);
  assert(empty_node->segm.left.type == DF);
  assert(empty_node->segm.right.type == FI);
  assert(right->segm.left.type == FI);
  assert(right->segm.right.type == I_);
}

void test_point_types_join(){
  test_point_types_join_same_grad();
  test_point_types_join_diff_grad();
  test_join_with_DI_type();
}

void test_point_types(){
  test_point_types_split();
  test_point_types_join();
}

void test_swm_one_segm(){
  Segment flat(Point(0,1), Point(10,1));
  int h = 3;
  BST t = BST(new TreeNode(flat));
  t = SWM(t, 3);

  // propagate request
  TreeNode* min = TreeNode::min(t.root);
  TreeNode* max = TreeNode::max(t.root);

  assert(t.root->segm == Segment(Point(0,1), Point(13, 1)));
  assert(t.root->segm.left.type == _F);
  assert(t.root->segm.right.type == F_);

  Segment incr(Point(0,0), Point(10,10));
  t = BST(new TreeNode(incr));
  t = SWM(t, 3);
  min = TreeNode::min(t.root);
  max = TreeNode::max(t.root);
  assert(min->segm == Segment(Point(0,0), Point(3,0)));
  assert(max->segm == Segment(Point(3,0), Point(13, 10)));
  assert(min->segm.left.type == _F);
  assert(min->segm.right.type == FI);
  assert(max->segm.left.type == FI);
  assert(max->segm.right.type == I_);

  Segment decr(Point(0,0), Point(10,-10));
  t = BST(new TreeNode(decr));
  t = SWM(t, 3);
  min = TreeNode::min(t.root);
  max = TreeNode::max(t.root);
  assert(min->segm == Segment(Point(0,0), Point(10,-10)));
  assert(max->segm == Segment(Point(10,-10), Point(13, -10)));
  assert(min->segm.left.type == _D);
  assert(min->segm.right.type == DF);
  assert(max->segm.left.type == DF);
  assert(max->segm.right.type == F_);
}

void test_swm_collapsing_segm(){
  Segment incr(Point(0,0), Point(10,10));
  Segment decr(Point(10,10), Point(15, 5));
  Segment flat(Point(15,5), Point(20,5));

  BST t1 = BST(new TreeNode(incr));
  BST t2 = BST(new TreeNode(decr));
  BST t3 = BST(new TreeNode(flat));
  BST t = join(join(t1, t2), t3);
  TreeNode *mid = t.root;
  TreeNode *left = t.root->left;
  TreeNode *right = t.root->right;

  assert(left->segm.left.type == _I);
  assert(left->segm.right.type == ID);
  assert(mid->segm.left.type == ID);
  assert(mid->segm.right.type == DF);
  assert(right->segm.left.type == DF);
  assert(right->segm.right.type == F_);
  assert(t.root->t_min == 10);

  t = SWM(t, 5);
  mid = t.root;
  left = t.root->left;
  right = t.root->right;
  TreeNode *left_left = left->left;

  assert(left_left->segm.left.type == _F);
  assert(left_left->segm.right.type == FI);
  assert(left->segm.left.type == FI);
  assert(left->segm.right.type == ID);
  assert(mid->segm.left.type == ID);
  assert(mid->segm.right.type == DF);
  assert(right->segm.left.type == DF);
  assert(right->segm.right.type == F_);
  assert(t.root->t_min == 5);

  t = SWM(t, 6);
  mid = t.root;
  left = t.root->left;
  right = t.root->right;
  assert(left->segm.left.type == _F);
  assert(left->segm.right.type == FI);
  assert(mid->segm.left.type == FI);
  assert(mid->segm.right.type == IF);
  assert(right->segm.left.type == IF);
  assert(right->segm.right.type == F_);
}

void test_swm_empty_segm(){
  Segment s1(Point(1,2),Point(2,1)), s2(Point(2,1), Point(3,2)), s3(Point(3,2), Point(4,2));
  BST t;t.insert(s2);t.insert(s1);t.insert(s3);
  BST t_swm = SWM(t, 3);
  TreeNode* node = t.root->find_node_containing(4);
  assert(node != NULL);
}

void test_swm_case_1(){
  Segment s1(Point(1,4), Point(2,3)), s2(Point(2,3), Point(2.5, 3.5)), s3(Point(2.5, 3.5), Point(3,3));
  BST t; t.insert(s2);t.insert(s1);t.insert(s3);
  BST t_swm = SWM(t,1);
  assert(t_swm.root->segm == Segment(Point(1,4), Point(2,3)));
  assert(t_swm.root->right->segm == Segment(Point(2,3), Point(4,3)));
}

void test_swm_free_node(){
  Segment s1(Point(1,2), Point(1.5, 2.5)), s2(Point(1.5, 2.5), Point(2,2));
  BST t; t.insert(s2); t.insert(s1);
  BST t_swm = SWM(t, 1);
  // if this doesn't fail then we're fine
}

void test_swm(){
  test_swm_one_segm();
  test_swm_collapsing_segm();
  test_swm_empty_segm();
  test_swm_case_1();
  test_swm_free_node();
}

void test_combine_simple(){
  BST T1 = BST(new TreeNode(Segment(Point(0,4), Point(10,4))));
  T1.insert(Segment(Point(10,4), Point(15,4)));

  BST T2 = join(BST(new TreeNode(Segment(Point(0,0), Point(4,4)))), BST(new TreeNode(Segment(Point(4,4), Point(15,4)))));

  BST T_comb = combine(T1, T2);
  TreeNode *mid = T_comb.root;
  TreeNode *left = mid->left;
  TreeNode *right = mid->right;

  assert(left->segm == Segment(Point(0,0), Point(4,4)));
  assert(mid->segm == Segment(Point(4,4), Point(10,4)));
  assert(right->segm == Segment(Point(10,4), Point(15,4)));
}

void test_combine_1(){
  BST t1, t2;
  Segment s1(Point(1,4), Point(3,4)), s2(Point(3,4), Point(5,2)), s3(Point(5,2), Point(6,2)), s4(Point(1,3), Point(6,3));
  t1.insert(s2);t1.insert(s1);t1.insert(s3);
  t2.insert(s4);
  BST t = combine(t1, t2);
}

void test_combine(){
  test_combine_simple();
  test_combine_1();
}

void test_get_OUT_LEFT(){
  Segment s1(Point(1, 3), Point(2,2)), s2(Point(2,2), Point(3, 2)), s3(Point(3,2), Point(4,1)), s4(Point(4,1), Point(5, 2));
  BST t; t.insert(s3);t.insert(s2);t.insert(s4);t.insert(s1);
  BST out = get_OUT_LEFT(t, 5, 3);
  TreeNode *min = TreeNode::min(out.root);
  assert(min->segm.left.type == _F);
}

void test_get_OUT_TOP(){
  
}

void test_rle_split(){
  Segment s1(Point(1,2), Point(2,1)), s2(Point(2,1), Point(6,1)), s3(Point(6,1), Point(8,3));
  BST t; t.insert(s2); t.insert(s1); t.insert(s3);
  std::pair<BST, BST> p = split(t,5);
  assert(p.first.root->segm == Segment(Point(1,2), Point(2,1)));
  assert(p.first.root->right->segm == Segment(Point(2,1), Point(5,1)));
  assert(p.second.root->segm == Segment(Point(5,1), Point(6,1)));
  assert(p.second.root->right->segm == Segment(Point(6,1), Point(8,3)));
}
void test_rle_edit_distance(){
  test_join();
  test_point_types();
  test_swm();
  test_combine();
  test_get_OUT_LEFT();
  test_get_OUT_TOP();
  test_rle_split();
}
