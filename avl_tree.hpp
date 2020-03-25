#pragma once
#include <string>
#include <stdlib.h>
#include "segment.hpp"
class TreeNode
{
public:
  Segment segm;
  bool active;
  int dx, dy, dg, dt, t_min;
  Point_t type_l, type_r;
  int height;
  TreeNode *left;
  TreeNode *right;

  TreeNode(Segment segm);
  TreeNode();
  TreeNode(Segment segm, TreeNode* left, TreeNode* right);
  int get_t_min();
  void shift(int dx, int dy);
  void change_grad(int dg);
  void apply_swm(int dt);
  void update_endpoints();
  static void lazy_update(TreeNode* node);
  void set_right(TreeNode* node);
  void set_left(TreeNode* node);
  static TreeNode* insert(TreeNode* root, Segment segm);
  void update_value(Segment segm);
  void recompute_height();
  void recompute_tmin();
  int get_balance();
  TreeNode* find_node_containing(float x);
  static TreeNode* delete_node(TreeNode *root, Segment segm);
  static TreeNode *min(TreeNode *node);
  static TreeNode *max(TreeNode *node);
  static TreeNode* rotate_right(TreeNode* root);
  static TreeNode* rotate_left(TreeNode* root);
  static TreeNode* get_new_copy(TreeNode* root);
  std::string to_string();
  TreeNode *find(Segment segm);
  static void free(TreeNode *node);
};

class BST
{
public:
  TreeNode *root;

  BST();
  BST(TreeNode* root);
  void insert(Segment segm);
  std::string to_string();
  TreeNode *find(Segment segm);
  TreeNode *find_predec(Segment segm);
  TreeNode *find_succ(Segment segm);
  bool is_balanced();
  void shift(int dx, int dy);
  void change_grad(int dg);
  void apply_swm(int dt);
  void update_point_type(Segment segm);
  void update_tmin_on_path_to(Segment segm);
  float get_value_at_coord(float x);
  void find_leftmost_smaller(TreeNode *node, Segment &s, bool &found_segm);
  void find_rightmost_larger(TreeNode *node, Segment &s, bool &found_segm);
  static BST join(TreeNode *t_l, TreeNode *t_r, Segment segm);
  static BST join(TreeNode *t_l, TreeNode *t_r);
  static BST get_new_copy(BST t);
  // splits the tree, keeping the segment in the right partition of the tree
  // This function has side effect for the subtree rooted at root, which is compromised
  static std::pair<BST, BST> split(TreeNode* root, Segment segm);
  void delete_node(Segment segm);
};

void print_2D(BST T);