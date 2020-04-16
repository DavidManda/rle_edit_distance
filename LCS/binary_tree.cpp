#include "binary_tree.hpp"
#include <cstddef>
#include <iostream>

#define COUNT 10

TreeNode::TreeNode(int _key, int _val, int _x, int _y)
{
  key = _key;
  val = _val;
  x = _x;
  y = _y;
  left = NULL;
  right = NULL;
}

void TreeNode::operator=(const TreeNode &node)
{
  key = node.key;
  val = node.val;
  x = node.x;
  y = node.y;
}

void TreeNode::update_value(int _val, int _x, int _y)
{
  this->val = _val;
  this->x = _x;
  this->y = _y;
}

void TreeNode::insert(int _key, int _val, int _x, int _y)
{
  if (this->key == _key)
  {
    this->update_value(_val, _x, _y);
    return;
  }
  if (this->key > _key)
  {
    if (this->left == NULL)
    {
      this->left = new TreeNode(_key, _val, _x, _y);
      return;
    }
    this->left->insert(_key, _val, _x, _y);
  }
  else
  {
    if (this->right == NULL)
    {
      this->right = new TreeNode(_key, _val, _x, _y);
      return;
    }
    this->right->insert(_key, _val, _x, _y);
  }
}

std::string TreeNode::to_string()
{
  return "Key: " + std::to_string(this->key) + "\nVal: " + std::to_string(this->val) + "\nX: " + std::to_string(this->x) + "\nY: " + std::to_string(this->y) + "\n";
}

TreeNode *TreeNode::find(int _key)
{
  if (this->key == _key)
  {
    return this;
  }
  TreeNode *next_node;
  if (this->key > _key)
  {
    next_node = this->left;
  }
  else
  {
    next_node = this->right;
  }
  if (next_node == NULL)
  {
    return NULL;
  }
  return next_node->find(_key);
}

void _find_predec_succ(TreeNode *root, int key, TreeNode *&predec, TreeNode *&succ)
{
  // Base case
  if (root == NULL)
    return;

  // If key is present at root
  if (root->key == key)
  {
    // the maximum value in left subtree is predecessor
    if (root->left != NULL)
    {
      TreeNode *tmp = root->left;
      while (tmp->right)
        tmp = tmp->right;
      predec = tmp;
    }

    // the minimum value in right subtree is successor
    if (root->right != NULL)
    {
      TreeNode *tmp = root->right;
      while (tmp->left)
        tmp = tmp->left;
      succ = tmp;
    }
    return;
  }

  // If key is smaller than root's key, go to left subtree
  if (root->key > key)
  {
    succ = root;
    _find_predec_succ(root->left, key, predec, succ);
  }
  else // go to right subtree
  {
    predec = root;
    _find_predec_succ(root->right, key, predec, succ);
  }
}

binarySearchTree::binarySearchTree()
{
  this->root = NULL;
}

void binarySearchTree::insert(int _key, int _val, int _x, int _y)
{
  if (this->root == NULL)
  {
    this->root = new TreeNode(_key, _val, _x, _y);
    return;
  }
  this->root->insert(_key, _val, _x, _y);
}

TreeNode *binarySearchTree::find(int _key)
{
  if (this->root == NULL)
  {
    return NULL;
  }
  return this->root->find(_key);
}

// This returns the node with the biggest key that is smaller than _key
TreeNode *binarySearchTree::find_predec(int _key)
{
  TreeNode *predec = NULL;
  TreeNode *succ = NULL;
  _find_predec_succ(this->root, _key, predec, succ);

  return predec;
}

TreeNode *binarySearchTree::find_succ(int _key)
{
  TreeNode *succ = NULL;
  TreeNode *predec = NULL;
  _find_predec_succ(this->root, _key, predec, succ);

  return succ;
}

// Given a non-empty binary search tree, return the node with minimum
// key value found in that tree. Note that the entire tree does not
// need to be searched.
TreeNode *min_value_node(TreeNode *node)
{
  TreeNode *current = node;

  /* loop down to find the leftmost leaf */
  while (current && current->left != NULL)
    current = current->left;

  return current;
}

// Given a binary search tree and a key, this function deletes the key
// and returns the new root
TreeNode *_delete_node(TreeNode *root, int key)
{

  // base case
  if (root == NULL)
    return root;

  // If the key to be deleted is smaller than the root's key,
  // then it lies in left subtree
  if (key < root->key)
    root->left = _delete_node(root->left, key);

  // If the key to be deleted is greater than the root's key,
  // then it lies in right subtree
  else if (key > root->key)
    root->right = _delete_node(root->right, key);

  // if key is same as root's key, then This is the node
  // to be deleted
  else
  {
    // node with only one child or no child
    if (root->left == NULL)
    {
      TreeNode *temp = root->right;
      free(root);
      return temp;
    }
    else if (root->right == NULL)
    {
      TreeNode *temp = root->left;
      free(root);
      return temp;
    }

    // node with two children: Get the inorder successor (smallest
    // in the right subtree)
    TreeNode *temp = min_value_node(root->right);

    // Copy the inorder successor's content to this node
    *root = *temp;

    // Delete the inorder successor
    root->right = _delete_node(root->right, temp->key);
  }
  return root;
}

void binarySearchTree::delete_node(int key)
{
  if (this->root == NULL)
  {
    return;
  }
  this->root = _delete_node(this->root, key);
}
// Function to print binary tree in 2D
// It does reverse inorder traversal
void print_2D_util(TreeNode *root, int space)
{
  // Base case
  if (root == NULL)
    return;

  // Increase distance between levels
  space += COUNT;

  // Process right child first
  print_2D_util(root->right, space);

  // Print current node after space
  // count
  std::cout << "\n";
  for (int i = COUNT; i < space; i++)
    std::cout << " ";
  std::cout << root->key << "\n";

  // Process left child
  print_2D_util(root->left, space);
}

void print_2D(TreeNode *root)
{
  print_2D_util(root, 0);
}