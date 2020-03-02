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
  void operator=(const TreeNode &node);
  static TreeNode* insert(TreeNode* root, Segment segm);
  void update_value(Segment segm);
  void recompute_height();
  int get_balance();
  static TreeNode* rotate_right(TreeNode* root);
  static TreeNode* rotate_left(TreeNode* root);
  std::string to_string();
  TreeNode *find(Segment segm);
};

class BST
{
public:
  TreeNode *root;

  BST();
  void insert(Segment segm);
  std::string to_string();
  TreeNode *find(Segment segm);
  TreeNode *find_predec(Segment segm);
  TreeNode *find_succ(Segment segm);
  void delete_node(Segment segm);
};

void print_2D(TreeNode *root);