#pragma once
#include <string>

class treeNode
{
public:
  int key;
  int val;
  int x, y;
  treeNode *left;
  treeNode *right;

  treeNode(int _key, int _val, int _x, int _y);
  void operator=(const treeNode &node);
  void insert(int _key, int _val, int _x, int _y);
  void update_value(int _val, int _x, int _y);
  std::string to_string();
  treeNode *find(int _key);
};

class binarySearchTree
{
public:
  treeNode *root;

  binarySearchTree();
  void insert(int _key, int _val, int _x, int _y);
  std::string to_string();
  treeNode *find(int key);
  treeNode *find_predec(int key);
  treeNode *find_succ(int key);
  void delete_node(int key);
};

void print_2D(treeNode *root);