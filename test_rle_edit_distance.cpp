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

// void test_join(){
  // BST t_l,t_r, t;
  // t = *join(&t_l, &t_r);

  // assert(t->root == NULL);
  // Segment s_0011 = Segment(Point(0,0), Point(1,1));
  // t->insert(s_0011);
  // std::cout<<t<<' '<<t_l<<' '<<t_r<<'\n';
  // std::cout<<t->root<<' '<<t_l->root<<' '<<t_r->root<<'\n';
  // assert(t->root->segm == s_0011);
  // assert(t_l->root == NULL && t_r->root == NULL);

  // BST::free(t);t = NULL;
  // t_l->insert(s_0011);
  // t = join(t_l, t_r);
  // assert(t->root->segm == s_0011);

  // BST::free(t); t = NULL;
  // BST::free(t_l); t_l = NULL;
  // BST::free(t_r); t_r = NULL;
  // t_l = new BST(); t_r = new BST();
  // Segment s_1121(Point(1,1), Point(2,1));
  // t_r->insert(s_1121);
  // t = join(t_l, t_r);
  // TreeNode *min = BST::min(t->root);
  // TreeNode *max = BST::max(t->root);
  // assert(min->segm == s_0011);
  // assert(max->segm == s_1121);

  // BST::free(t);BST::free(t_l);BST::free(t_r); 
  // t_l = new BST(); t_r = new BST(); 
  // Segment s_1122(Point(1,1), Point(2,2));
  // t_l->insert(s_0011);
  // t_r->insert(s_1122);
  // t = join(t_l, t_r);
  // assert(t->root->segm == Segment(Point(0,0), Point(2,2)));
// }

void test_rle_edit_distance(){
  test_get_val_at_coord();
}
