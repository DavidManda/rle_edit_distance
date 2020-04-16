#pragma once
#include <string>

class TreeNode
{
public:
  int key;
  int val;
  int x, y;
  TreeNode *left;
  TreeNode *right;

  TreeNode(int _key, int _val, int _x, int _y);
  void operator=(const TreeNode &node);
  void insert(int _key, int _val, int _x, int _y);
  void update_value(int _val, int _x, int _y);
  std::string to_string();
  TreeNode *find(int _key);
};

class binarySearchTree
{
public:
  TreeNode *root;

  binarySearchTree();
  void insert(int _key, int _val, int _x, int _y);
  std::string to_string();
  TreeNode *find(int key);
  TreeNode *find_predec(int key);
  TreeNode *find_succ(int key);
  void delete_node(int key);
};

void print_2D(TreeNode *root);