#pragma once
#include <string>
#include "segment.hpp"
class TreeNode
{
public:
  Segment segm;
  bool active;
  int dx, dy, dg, dt;
  Point_t type_l, type_r;
  int height;
  TreeNode *left;
  TreeNode *right;

  TreeNode(Segment segm);
  TreeNode();
  TreeNode(Segment segm, TreeNode* left, TreeNode* right);
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
  int get_balance();
  static TreeNode *min(TreeNode *node);
  static TreeNode *max(TreeNode *node);
  static TreeNode* rotate_right(TreeNode* root);
  static TreeNode* rotate_left(TreeNode* root);
  std::string to_string();
  TreeNode *find(Segment segm);
  static void free(TreeNode *node);

private:
  int t_min;
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
  static BST join(TreeNode *t_l, TreeNode *t_r, Segment segm);
  static BST join(TreeNode *t_l, TreeNode *t_r);
  // splits the tree, keeping the segment in the right partition of the tree
  // This function has side effect for the subtree rooted at root, which is compromised
  static std::pair<BST, BST> split(TreeNode* root, Segment segm);
  void delete_node(Segment segm);
};

void print_2D(TreeNode *root);