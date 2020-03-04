#include "avl_tree.hpp"
#include <cstddef>
#include <iostream>

#define COUNT 10

TreeNode::TreeNode(Segment segm)
{
  this->segm = segm;
  this->height = 1;
  left = NULL;
  right = NULL;
}

TreeNode::TreeNode(){}

TreeNode::TreeNode(Segment segm, TreeNode* left, TreeNode* right){
  this->segm = segm;
  this->left = left;
  this->right = right;
  this->recompute_height();
}

void TreeNode::operator=(const TreeNode &node)
{
  this->segm = node.segm;
  this->height = node.height;
}

void TreeNode::update_value(Segment segm)
{
  this->segm = segm;
}

static TreeNode* rotate_right(TreeNode* root){
  TreeNode *left = root->left;
  TreeNode *left_right = left->right;

  // rotation
  left->right = root;
  root->left = left_right;

  root->recompute_height();
  left->recompute_height();

  return left;
}

static TreeNode* rotate_left(TreeNode* root){
  TreeNode* right = root->right;
  TreeNode* right_left = right->left;

  right->left = root;
  root->right = right_left;

  root->recompute_height();
  right->recompute_height();

  return right;
}

static TreeNode* insert(TreeNode* root, Segment segm)
{
  if (root == NULL)  
    return(new TreeNode(segm));  

  if (segm < root->segm)  
    root->left = insert(root->left, segm);  
  else if (segm > root->segm)  
    root->right = insert(root->right, segm);  
  else{
    root->update_value(segm);  
    return root;
  }

  root->recompute_height();

  int balance = root->get_balance();

  // left left
  if(balance > 1 && (segm < root->left->segm)){
    return rotate_right(root);
  }
  // right right
  if(balance < -1 && segm > root->right->segm){
    return rotate_left(root);
  }
  // left right
  if(balance > 1 && segm > root->left->segm){
    root->left = rotate_left(root->left);
    return rotate_right(root);
  }
  // right left
  if(balance < -1 && segm < root->right->segm){
    root->right = rotate_right(root->right);
    return rotate_left(root);
  }

  // if here no rotation happened
  return root;
}

int TreeNode::get_balance(){
  int h_right = (this->right != NULL) ? this->right->height : 0;
  int h_left = (this->left != NULL) ? this->left->height : 0;
  return h_left - h_right; 
}

void TreeNode::recompute_height()
{
  int h_right = (this->right != NULL) ? this->right->height : 0;
  int h_left = (this->left != NULL) ? this->left->height : 0;
  this->height = 1 + std::max(h_left, h_right);
}

std::string TreeNode::to_string()
{
  return this->segm.to_string();
}

TreeNode *TreeNode::find(Segment segm)
{
  if (this->segm == segm)
  {
    return this;
  }
  TreeNode *next_node;
  if (this->segm > segm)
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
  return next_node->find(segm);
}

void _find_predec_succ(TreeNode *root, Segment segm, TreeNode *&predec, TreeNode *&succ)
{
  // Base case
  if (root == NULL)
    return;

  // If key is present at root
  if (root->segm == segm)
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
  if (root->segm > segm)
  {
    succ = root;
    _find_predec_succ(root->left, segm, predec, succ);
  }
  else // go to right subtree
  {
    predec = root;
    _find_predec_succ(root->right, segm, predec, succ);
  }
}

BST::BST()
{
  this->root = NULL;
}

BST::BST(TreeNode* root){
  this->root = root;
}

void BST::insert(Segment segm)
{
  if (this->root == NULL)
  {
    this->root = new TreeNode(segm);
    return;
  }
  // these dots are weird here but seems like I need them
  this->root = ::insert(this->root, segm);
}

TreeNode *BST::find(Segment segm)
{
  if (this->root == NULL)
  {
    return NULL;
  }
  return this->root->find(segm);
}

// This returns the node with the biggest key that is smaller than _key
TreeNode *BST::find_predec(Segment segm)
{
  TreeNode *predec = NULL;
  TreeNode *succ = NULL;
  _find_predec_succ(this->root, segm, predec, succ);

  return predec;
}

TreeNode *BST::find_succ(Segment segm)
{
  TreeNode *succ = NULL;
  TreeNode *predec = NULL;
  _find_predec_succ(this->root, segm, predec, succ);

  return succ;
}

// Given a non-empty binary search tree, return the node with minimum
// key value found in that tree. Note that the entire tree does not
// need to be searched.
TreeNode* BST::min(TreeNode *node)
{
  TreeNode *current = node;

  /* loop down to find the leftmost leaf */
  while (current && current->left != NULL)
    current = current->left;

  return current;
}

TreeNode* BST::max(TreeNode *node){
  TreeNode *current = node;

  while(current && current->right != NULL)
    current = current->right;
  
  return current;
}

// Given a binary search tree and a key, this function deletes the key
// and returns the new root
TreeNode *_delete_node(TreeNode *root, Segment segm)
{

  // base case
  if (root == NULL)
    return root;

  // If the key to be deleted is smaller than the root's key,
  // then it lies in left subtree
  if (segm < root->segm)
    root->left = _delete_node(root->left, segm);

  // If the key to be deleted is greater than the root's key,
  // then it lies in right subtree
  else if (segm > root->segm)
    root->right = _delete_node(root->right, segm);

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
    TreeNode *temp = BST::min(root->right);

    // Copy the inorder successor's content to this node
    *root = *temp;

    // Delete the inorder successor
    root->right = _delete_node(root->right, temp->segm);
  }
  // If the tree had only one node
  if(root == NULL)
    return root;
  
  root->recompute_height();

  int balance = root->get_balance();
  // left left
  if(balance > 1 && root->left->get_balance() >= 0){
    return rotate_right(root);
  }
  // left right
  if(balance > 1  && root->left->get_balance() < 0){
    root->left = rotate_left(root->left);
    return rotate_right(root);
  }
  // right right
  if(balance < -1 && root->right->get_balance() <= 0){
    return rotate_left(root);
  }
  // right left
  if(balance < -1 && root->right->get_balance() > 0){
    root->right = rotate_left(root->right);
    return rotate_left(root);
  }

  return root;
}

void BST::delete_node(Segment segm)
{
  if (this->root == NULL)
  {
    return;
  }
  this->root = _delete_node(this->root, segm);
}

TreeNode *join_right(TreeNode *t_l, TreeNode *t_r, Segment segm){
  TreeNode *left_left = t_l->left;
  int left_left_height = (left_left != NULL) ? left_left->height : 0;
  TreeNode *left_right = t_l->right;
  Segment left_segm = t_l->segm;
  std::cout<<(left_right->height < t_r->height + 1)<<'\n';
  if(left_right->height < t_r->height + 1){
    TreeNode *aux = new TreeNode(segm, left_right, t_r);
    if(aux->height <= left_left_height + 1){
      return new TreeNode(left_segm, left_left, aux);
    }
    else{
      return rotate_left(new TreeNode(left_segm, left_left, rotate_right(aux)));
    }
  }
  else{
    TreeNode *aux = join_right(left_right, t_r, segm);
    TreeNode *join_sol = new TreeNode(left_segm, left_left, aux);
    if(left_left_height < aux->height + 1)
      return rotate_left(join_sol);
    else
      return join_sol;
  }
}

TreeNode *join_left(TreeNode *t_l, TreeNode *t_r, Segment segm){
  TreeNode *right_right = t_r->right;
  int right_right_height = (right_right != NULL) ? right_right->height : 0;
  TreeNode *right_left = t_r->left;
  Segment right_segm = t_r->segm;

  if(right_left->height < t_l->height + 1){
    TreeNode *aux = new TreeNode(segm, t_l, right_left);
    if(aux->height <= right_right_height + 1){
      return new TreeNode(right_segm, aux, right_right);
    }
    else{
      return rotate_right(new TreeNode(right_segm, rotate_left(aux), right_right));
    }
  }
  else{
    TreeNode* aux = join_left(t_l, right_left, segm);
    TreeNode* join_sol = new TreeNode(right_segm, aux, right_right);
    if(aux->height + 1 > right_right->height + 1)
      return rotate_right(join_sol);
    else
      return join_sol;
  }
}

BST* BST::join(TreeNode *t_l, TreeNode *t_r, Segment segm){
  BST *joined_tree = new BST();
  TreeNode *root;

  if(t_l == NULL && t_r == NULL){
    joined_tree->insert(segm);
    return NULL;
  }
  if(t_l == NULL){
    joined_tree->root = t_r;
    joined_tree->insert(segm);
    return joined_tree;
  }
  if(t_r == NULL){
    joined_tree->root = t_l;
    joined_tree->insert(segm);
    return joined_tree;
  }

  if(t_l->height > t_r->height + 1){
    root = join_right(t_l, t_r, segm);
  }
  else if(t_r->height > t_l->height + 1){
    root = join_left(t_l, t_r, segm);
  }
  else
  {
    root = new TreeNode(segm, t_l, t_r);
  }
  joined_tree->root = root;
  return joined_tree;
}

// splits the tree, keeping the segment in the right partition of the tree
std::pair<BST*, BST*> BST::split(TreeNode* root, Segment segm){
  if(root == NULL){
    return std::pair<BST*, BST*>(NULL, NULL);
  }
  if(segm == root->segm){
    BST* left = (root->left != NULL) ? new BST(root->left) : new BST();
    BST* right = (root->right != NULL) ? new BST(root->right) : new BST();
    right->insert(segm);
    return std::pair<BST*, BST*>(left, right);
  }
  if(segm < root->segm){
    std::pair<BST*, BST*> aux = BST::split(root->left, segm);
    return std::pair<BST*, BST*>(aux.first, BST::join(aux.second->root, root->right, root->segm));
  }
  std::pair<BST*, BST*> aux = BST::split(root->right, segm);
  return std::pair<BST*, BST*>(BST::join(root->left, aux.first->root, root->segm), new BST(root->right)); 
  
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
  std::cout << root->to_string() << "\n";

  // Process left child
  print_2D_util(root->left, space);
}

void print_2D(TreeNode *root)
{
  print_2D_util(root, 0);
}