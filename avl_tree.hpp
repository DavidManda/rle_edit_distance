#pragma once
#include <string>
#include "segment.hpp"
class TreeNode
{
public:
  Segment segm;
  int height;
  TreeNode *left;
  TreeNode *right;

  TreeNode(Segment segm);
  TreeNode();
  TreeNode(Segment segm, TreeNode* left, TreeNode* right);
  void operator=(const TreeNode &node);
  static TreeNode* insert(TreeNode* root, Segment segm);
  void update_value(Segment segm);
  void recompute_height();
  int get_balance();
  static TreeNode* rotate_right(TreeNode* root);
  static TreeNode* rotate_left(TreeNode* root);
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
  static TreeNode *min(TreeNode *node);
  static TreeNode *max(TreeNode *node);
  static BST *join(TreeNode *t_l, TreeNode *t_r, Segment segm);
  static std::pair<BST*, BST*> split(TreeNode* root, Segment segm);
  void delete_node(Segment segm);
  static void free(BST *tree);
};

void print_2D(TreeNode *root);