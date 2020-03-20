#include "avl_tree.hpp"
#include <cstddef>
#include <iostream>
#include <assert.h>
#include <limits.h>
#include <cstdlib>
#include <sstream>

#define COUNT 10

TreeNode::TreeNode(){
  this->active = true;
  this->dx = 0;
  this->dy = 0;
  this->dg = 0;
  this->dt = 0;
  this->type_l = NotInitialised;
  this->type_r = NotInitialised;
  this->height = 1;
  this->t_min = this->get_t_min();
  this->left = NULL;
  this->right = NULL;
}

TreeNode::TreeNode(Segment segm) : TreeNode(){
  this->segm = segm;
}

TreeNode::TreeNode(Segment segm, TreeNode* left, TreeNode* right) : TreeNode(segm){
  this->left = left;
  this->right = right;
  this->recompute_height();
  this->recompute_tmin();
}

bool has_collapse_time(TreeNode* node){
  if(node == NULL)
    return false;
  return  (node->type_l == ID && node->type_r == DF) ||
          (node->type_l == IF && node->type_r == FD) ||
          (node->type_l == FI && node->type_r == ID);
}

int TreeNode::get_t_min(){
  if(has_collapse_time(this)){
    return Point::get_manhattan(this->segm.left, this->segm.right);
  }
  return INT_MAX;
}

void TreeNode::recompute_tmin(){
  int t_left = (this->left != NULL) ? this->left->t_min : INT_MAX;
  int t_right = (this->right != NULL) ? this->right->t_min : INT_MAX;
  int this_tmin = this->get_t_min();

  this->t_min = std::min(this_tmin, std::min(t_left, t_right));
}

void TreeNode::update_value(Segment segm)
{
  this->segm = segm;
}

int height(TreeNode* node){
  if(node == NULL)
    return 0;
  return node->height;
}

void set_endpoints(TreeNode* node){
  if(node == NULL)
    return;

  if(node->segm.left.x == node->segm.right.x){
    node->type_l = _F;
    node->type_r = F_;
    return;
  }

  Segment s = node->segm;
  int slope = (s.right.y - s.left.y)/(s.right.x - s.left.x);
  assert(slope == 1 || slope == 0 || slope == -1);

  if(slope == 1){
    node->type_l = _I;
    node->type_r = I_;
  }
  if(slope == -1){
    node->type_l = _D;
    node->type_r = D_;
  }
  if(slope == 0){
    node->type_l = _F;
    node->type_r = F_;
  }
}

void move_point(Point &p, Point_t type, int dt){
  if(type == FI || type == IF || type == I_ || type == F_){
    p.x += 1 * dt;
  }
  if(type == ID){
    p.x += 0.5 * (double)dt;
    p.y -= 0.5 * (double)dt;
  }
}

Point_t update_point_type(Point_t type, int dg){
  assert(type != ID && type != DI);
  assert(!((type == _D || type == D_ || type == FD || type == DF) && dg == -1));
  assert(!((type == _I || type == I_ || type == FI || type == IF) && dg == 1));

  if(type == _I && dg == -1){
    return _F;
  }

  if(type == I_ && dg == -1){
    return F_;
  }

  if(type == _F && dg == -1){
    return _D;
  }

  if(type == F_ && dg == -1){
    return D_;
  }

  if(type == IF && dg == -1){
    return FD;
  }

  if(type == FI && dg == -1){
    return DF;
  }
  
  if(type == _D && dg == 1){
    return _F;
  }

  if(type == D_ && dg == 1){
    return F_;
  }

  if(type == _F && dg == 1){
    return _I;
  }

  if(type == F_ && dg == 1){
    return I_;
  }

  if(type == DF && dg == 1){
    return FI;
  }

  if(type == FD && dg == 1){
    return IF;
  }
  return type;
}

void TreeNode::update_endpoints(){
  if(this->dt == 0){
    return;
  }
  move_point(this->segm.left, this->type_l, this->dt);
  move_point(this->segm.right, this->type_r, this->dt);
  if(this->dg == 0){
    return;
  }
  this->type_l = update_point_type(this->type_l, this->dg);
  this->type_r = update_point_type(this->type_r, this->dg);
}

void TreeNode::shift(int dx, int dy){
  this->active = false;
  this->dx += dx;
  this->dy += dy;
}

void TreeNode::change_grad(int dg){
  this->active = false;
  this->dg += dg;
}

void TreeNode::apply_swm(int dt){
  this->active = false;
  // TODO make sure you should update this here or just recompute the value. maybe do both.
  this->t_min -= dt;
  if(this->dg == 0){
    this->dt += dt;
  }
  else if(this->type_l == IF || this->type_l == FI){
    this->shift(dt, 0);
  }
}

void TreeNode::lazy_update(TreeNode* node){
  if(node == NULL || node->active){
    return;
  }
  node->update_endpoints();

  // update gradient before performing shift, this is important
  node->segm.left.y += node->segm.left.x * node->dg;
  node->segm.right.y += node->segm.right.x * node->dg;


  // perform shift
  node->segm.left.x += node->dx;
  node->segm.left.y += node->dy;

  node->segm.right.x += node->dx;
  node->segm.right.y += node->dy;
  
  if(node->left){
    node->left->apply_swm(node->dt);
    node->left->change_grad(node->dg);
    node->left->shift(node->dx, node->dy);
  }
  if(node->right){
    node->right->apply_swm(node->dt);
    node->right->change_grad(node->dg);
    node->right->shift(node->dx, node->dy);
  }

  node->active = true;
  node->dx = 0;
  node->dy = 0;
  node->dg = 0;
  node->dt = 0;
}

TreeNode* TreeNode::rotate_right(TreeNode* root){
  TreeNode *left = root->left;
  TreeNode *left_right = left->right;
  TreeNode::lazy_update(root);
  TreeNode::lazy_update(left);
  TreeNode::lazy_update(left_right);
  // rotation
  left->right = root;
  root->left = left_right;

  root->recompute_tmin();
  root->recompute_height();
  left->recompute_tmin();
  left->recompute_height();

  return left;
}

TreeNode* TreeNode::rotate_left(TreeNode* root){
  TreeNode* right = root->right;
  TreeNode* right_left = right->left;
  TreeNode::lazy_update(root);
  TreeNode::lazy_update(right);
  TreeNode::lazy_update(right_left);
  right->left = root;
  root->right = right_left;

  root->recompute_tmin();
  root->recompute_height();
  right->recompute_tmin();
  right->recompute_height();

  return right;
}

TreeNode* TreeNode::insert(TreeNode* root, Segment segm)
{
  if (root == NULL)
    return(new TreeNode(segm));  
  TreeNode::lazy_update(root);
  if (segm <= root->segm)
    root->left = insert(root->left, segm);
  else if (segm >= root->segm)  
    root->right = insert(root->right, segm);  
  else{
    // node already exists
    return NULL;
  }
  root->recompute_tmin();
  root->recompute_height();

  int balance = root->get_balance();

  // left left
  if(balance > 1 && (segm < root->left->segm)){
    return TreeNode::rotate_right(root);
  }
  // right right
  if(balance < -1 && segm > root->right->segm){
    return TreeNode::rotate_left(root);
  }
  // left right
  if(balance > 1 && segm > root->left->segm){
    root->left = TreeNode::rotate_left(root->left);
    return TreeNode::rotate_right(root);
  }
  // right left
  if(balance < -1 && segm < root->right->segm){
    root->right = TreeNode::rotate_right(root->right);
    return TreeNode::rotate_left(root);
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
  std::stringstream ss;
  ss<<this->segm.to_string()<<' '<<'('<<this->dx<<','<<this->dy<<") "<<this->dt<<' '<<this->dg<<' '<<this->t_min;
  return ss.str();
}

TreeNode *TreeNode::find(Segment segm)
{
  lazy_update(this);
  if (this->segm == segm)
  {
    return this;
  }
  TreeNode *next_node;
  if (this->segm >= segm)
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


TreeNode* TreeNode::find_node_containing(float x){
  if(this->segm.contains(x)){
    return this;
  }

  if(x < this->segm.left.x && this->left != NULL){
    return this->left->find_node_containing(x);
  }
  if(x > this->segm.right.x && this->right != NULL){
    return this->right->find_node_containing(x);
  }
  else{
    std::cout<<"Element " << x <<" is not contained by the interval described by the tree!\n";
    throw;
  }
}

void _find_predec_succ(TreeNode *root, Segment segm, TreeNode *&predec, TreeNode *&succ)
{
  // Base case
  if (root == NULL)
    return;
  TreeNode::lazy_update(root);
  // If key is present at root
  if (root->segm == segm)
  {
    // the maximum value in left subtree is predecessor
    if (root->left != NULL)
    {
      TreeNode *tmp = root->left;
      TreeNode::lazy_update(tmp);
      while (tmp->right){
        tmp = tmp->right;
        TreeNode::lazy_update(tmp);
      }
      predec = tmp;
    }

    // the minimum value in right subtree is successor
    if (root->right != NULL)
    {
      TreeNode *tmp = root->right;
      TreeNode::lazy_update(tmp);
      while (tmp->left){
        tmp = tmp->left;
        TreeNode::lazy_update(tmp);
      }
      succ = tmp;
    }
    return;
  }

  // If key is smaller than root's key, go to left subtree
  if (root->segm >= segm)
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
  set_endpoints(this->root);
}

bool is_balanced_(TreeNode* root){
  if(root == NULL)
    return true;
  int h = root->height;
  root->recompute_height();
  if(h != root->height){
    std::cout<<"Heights should already be the correct and updated value, caught in is_balanced\n";
    throw;
  }
  int balance = root->get_balance();
  return balance >= -1 && balance <= 1 && is_balanced_(root->left) && is_balanced_(root->right);
}

bool BST::is_balanced(){
  return is_balanced_(root);
}

void TreeNode::set_right(TreeNode* node){
  this->right = node;
  this->recompute_height();
  this->recompute_tmin();
}

void TreeNode::set_left(TreeNode* node){
  this->left = node;
  this->recompute_height();
  this->recompute_tmin();
}

// inserts node into tree, sets point type and updates t_min
void BST::insert(Segment segm)
{
  if (this->root == NULL)
  {
    this->root = new TreeNode(segm);
    set_endpoints(this->root);
    return;
  }

  this->root = TreeNode::insert(this->root, segm);
  // update point types
  TreeNode *pred = BST::find_predec(segm);
  TreeNode *succ = BST::find_succ(segm);
  this->update_point_type(segm);
  if(pred != NULL)
    this->update_point_type(pred->segm);
  if(succ != NULL)
    this->update_point_type(succ->segm);
}

TreeNode *BST::find(Segment segm)
{
  if (this->root == NULL)
  {
    return NULL;
  }
  return this->root->find(segm);
}

float BST::get_value_at_coord(float x){
  Segment s = this->root->find_node_containing(x)->segm;
  return s.get_val_at_coord(x);
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
TreeNode* TreeNode::min(TreeNode *node)
{
  if(node == NULL){
    return NULL;
  }

  TreeNode *current = node;
  lazy_update(current);
  /* loop down to find the leftmost leaf */
  while (current && current->left != NULL){
    current = current->left;
    lazy_update(current);
  }

  return current;
}

TreeNode* TreeNode::max(TreeNode *node){
  TreeNode *current = node;
  lazy_update(current);
  while(current && current->right != NULL){
    current = current->right;
    lazy_update(current);
  }
  
  return current;
}

// Given a binary search tree and a key, this function deletes the key
// and returns the new root
TreeNode *_delete_node(TreeNode *root, Segment segm)
{
  // base case
  if (root == NULL)
    return root;
  TreeNode::lazy_update(root);

  if(segm == root->segm)
  {
    // node with only one child or no child
    if (root->left == NULL)
    {
      TreeNode *temp = root->right;
      return temp;
    }
    else if (root->right == NULL)
    {
      TreeNode *temp = root->left;
      return temp;
    }

    // node with two children: Get the inorder successor (smallest
    // in the right subtree) - this will be a leaf
    TreeNode *temp = TreeNode::min(root->right);
    TreeNode::lazy_update(temp);
    // Copy the inorder successor's content to this node
    root->update_value(temp->segm);

    // Delete the inorder successor
    root->right = _delete_node(root->right, temp->segm);
  }
  else if (segm <= root->segm)
    root->left = _delete_node(root->left, segm);
  else
    root->right = _delete_node(root->right, segm);

  
  // If the tree had only one node
  if(root == NULL)
    return root;
  root->recompute_tmin();
  root->recompute_height();
  // print_2D(root);
  int balance = root->get_balance();
  // left left
  if(balance > 1 && root->left->get_balance() >= 0){
    return TreeNode::rotate_right(root);
  }
  // left right
  if(balance > 1  && root->left->get_balance() < 0){
    root->left = TreeNode::rotate_left(root->left);
    return TreeNode::rotate_right(root);
  }
  // right right
  if(balance < -1 && root->right->get_balance() <= 0){
    return TreeNode::rotate_left(root);
  }
  // right left
  if(balance < -1 && root->right->get_balance() > 0){
    root->right = TreeNode::rotate_right(root->right);
    return TreeNode::rotate_left(root);
  }

  return root;
}

TreeNode* TreeNode::delete_node(TreeNode* root, Segment segm){
  if(root == NULL)
    return NULL;

  return _delete_node(root, segm);

}

void BST::delete_node(Segment segm)
{
  if (this->root == NULL)
  {
    return;
  }
  TreeNode *pred = this->find_predec(segm);
  TreeNode *succ = this->find_succ(segm);
  this->root = _delete_node(this->root, segm);

  if(pred != NULL && succ != NULL && pred->segm.get_slope() == succ->segm.get_slope()){
    Segment concat = Segment(pred->segm.left, succ->segm.right);
    this->root = _delete_node(this->root, pred->segm);
    this->root = _delete_node(this->root, succ->segm);
    this->insert(concat);
  }
  else{
    if(pred != NULL)
      this->update_point_type(pred->segm);
    if(succ != NULL)
      this->update_point_type(succ->segm);
  }
}

Point_t get_midpoint_type(Segment s_l, Segment s_r){

  if(s_l.right.x == s_l.left.x){
    assert(s_r.right.x != s_r.left.x);
    int slope_r = (s_r.right.y - s_r.left.y)/(s_r.right.x - s_r.left.x);
    if(slope_r == -1)
      return FD;
    return FI;
  }

  if(s_r.right.x == s_r.left.x){
    assert(s_l.right.x != s_l.left.x);
    int slope_l = (s_l.right.y - s_l.left.y)/(s_l.right.x - s_l.left.x);
    if(slope_l == -1)
      return DF;
    return IF;
  }
  int slope_r = (s_r.right.y - s_r.left.y)/(s_r.right.x - s_r.left.x);
  int slope_l = (s_l.right.y - s_l.left.y)/(s_l.right.x - s_l.left.x);
  if(slope_l == -1){
    if(slope_r == 0)
      return DF;
    if(slope_r == 1)
      return DI;
  }
  if(slope_l == 0){
    if(slope_r == -1)
      return FD;
    if(slope_r == 1)
      return FI;
  }
  if(slope_l == 1){
    if(slope_r == -1)
      return ID;
    if(slope_r == 0)
      return IF;
  }
  return NotInitialised;
}

// O(log(n))
void BST::update_point_type(Segment segm){
  TreeNode *node = this->find(segm);
  assert(node != NULL);
  TreeNode *predec = this->find_predec(segm);
  TreeNode *succ = this->find_succ(segm);
  set_endpoints(node);
  Segment s = node->segm;
  if(succ != NULL){
    Segment s_r = succ->segm;
    Point_t type = get_midpoint_type(s, s_r);
    if(type != DI){
      node->type_r = type;
    }
    else
    {
      Segment empty_segm = Segment(s.right, s.right);
      this->insert(empty_segm);
    }
  }

  if(predec != NULL){
    Segment s_l = predec->segm;
    Point_t type = get_midpoint_type(s_l, s);
    if(type != DI){
      node->type_l = type;
    }
    else
    {
      Segment empty_segm = Segment(s.left, s.left);
      this->insert(empty_segm);
    }
  }
  this->update_tmin_on_path_to(node->segm);
}

void update_tmin_on_path_to_(TreeNode* root, Segment segm){
  assert(root != NULL);
  if(root->segm == segm){
    root->recompute_tmin();
  }
  else if(root->segm >= segm){
    update_tmin_on_path_to_(root->left, segm);
  }
  else
  {
    update_tmin_on_path_to_(root->right, segm);
  }
  root->recompute_tmin();
}

void BST::update_tmin_on_path_to(Segment segm){
  update_tmin_on_path_to_(this->root, segm);
}

void BST::shift(int dx, int dy){
  if(this->root == NULL)
    return;
  this->root->shift(dx, dy);
  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BST
  TreeNode* min = TreeNode::min(this->root);
  TreeNode* max = TreeNode::max(this->root);
}

void BST::change_grad(int dg){
  if(this->root == NULL)
    return;
  this->root->change_grad(dg);

  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BST
  TreeNode* min = TreeNode::min(this->root);
  TreeNode* max = TreeNode::max(this->root);
}

void BST::apply_swm(int dt){
  this->root->apply_swm(dt);

  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BST
  TreeNode* min = TreeNode::min(this->root);
  TreeNode* max = TreeNode::max(this->root);
}

TreeNode *join_right(TreeNode *t_l, TreeNode *t_r, Segment segm){
  TreeNode::lazy_update(t_r);
  TreeNode::lazy_update(t_l);
  TreeNode *left_left = t_l->left;
  TreeNode *left_right = t_l->right;
  TreeNode::lazy_update(left_left);
  TreeNode::lazy_update(left_right);
  Segment left_segm = t_l->segm;
  if(height(left_right) < height(t_r) + 1){
    TreeNode *aux = new TreeNode(segm, left_right, t_r);
    if(height(aux) <= height(left_left) + 1){
      t_l->set_right(aux);
      return t_l;
    }
    else{
      t_l->set_right(TreeNode::rotate_right(aux));
      return TreeNode::rotate_left(t_l);
    }
  }
  else{
    TreeNode *aux = join_right(left_right, t_r, segm);
    t_l->set_right(aux);
    if(height(t_l->right) > height(t_l->left) + 1)
      return TreeNode::rotate_left(t_l);
    else
      return t_l;
  }
}

TreeNode *join_left(TreeNode *t_l, TreeNode *t_r, Segment segm){
  TreeNode::lazy_update(t_r);
  TreeNode::lazy_update(t_l);
  TreeNode *right_right = t_r->right;
  TreeNode *right_left = t_r->left;
  Segment right_segm = t_r->segm;
  TreeNode::lazy_update(right_left);
  TreeNode::lazy_update(right_right);

  if(height(right_left) < height(t_l) + 1){
    TreeNode *aux = new TreeNode(segm, t_l, right_left);
    if(height(aux) <= height(right_right) + 1){
      t_r->set_left(aux);
      return t_r;
    }
    else{
      t_r->set_left(TreeNode::rotate_left(aux));
      return TreeNode::rotate_right(t_r);
    }
  }
  else{
    TreeNode* aux = join_left(t_l, right_left, segm);
    t_r->set_left(aux);
    if(height(t_r->left) > height(t_r->right) + 1)
      return TreeNode::rotate_right(t_r);
    else
      return t_r;
  }
}

// joins the two trees and inserts a node with segm in it
BST BST::join(TreeNode *t_l, TreeNode *t_r, Segment segm){
  BST joined_tree = BST();
  TreeNode *root;

  if(t_l == NULL && t_r == NULL){
    joined_tree.insert(segm);
    return joined_tree;
  }
  if(t_l == NULL){
    TreeNode::lazy_update(t_r);
    joined_tree.root = t_r;
    joined_tree.insert(segm);
    return joined_tree;
  }
  if(t_r == NULL){
    TreeNode::lazy_update(t_l);
    joined_tree.root = t_l;
    joined_tree.insert(segm);
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
  joined_tree.root = root;
  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BST
  TreeNode* min = TreeNode::min(joined_tree.root);
  TreeNode* max = TreeNode::max(joined_tree.root);
  return joined_tree;
}

BST BST::join(TreeNode *t_l, TreeNode *t_r){
  BST joined_tree;
  if(t_l == NULL && t_r == NULL){
    return joined_tree;
  }
  if(t_l == NULL){
    joined_tree.root = t_r;
    return joined_tree;
  }
  if(t_r == NULL){
    joined_tree.root = t_l;
    return joined_tree;
  }
  Segment min_right = TreeNode::min(t_r)->segm;
  t_r = _delete_node(t_r, min_right);
  return BST::join(t_l, t_r, min_right);
}

// splits the tree, keeping the segment in the right partition of the tree
// This function has side effect for the subtree rooted at root, which is compromised
std::pair<BST, BST> split_(TreeNode* root, Segment segm){
  std::pair<BST, BST> pair;
  if(root == NULL){
    return pair;
  }
  TreeNode::lazy_update(root);
  if(segm == root->segm){
    BST left = (root->left != NULL) ? BST(root->left) : BST();
    BST right = (root->right != NULL) ? BST(root->right) : BST();
    right.insert(segm);
    return std::pair<BST, BST>(left, right);
  }
  if(segm <= root->segm){
    std::pair<BST, BST> aux = split_(root->left, segm);
    if(!aux.first.is_balanced()){
      print_2D(aux.first.root);
      throw;
    }
    if(!aux.second.is_balanced()){
      print_2D(aux.second.root);
      throw;
    }
    BST right = BST::join(aux.second.root, root->right, root->segm);
    return std::pair<BST, BST>(aux.first, right);
  }
  std::pair<BST, BST> aux = split_(root->right, segm);
  if(!aux.first.is_balanced()){
    print_2D(aux.first.root);
    throw;
  }
  if(!aux.second.is_balanced()){
    print_2D(aux.second.root);
    throw;
  }
  return std::pair<BST, BST>(BST::join(root->left, aux.first.root, root->segm), aux.second); 
}

std::pair<BST, BST> BST::split(TreeNode* root, Segment segm){
  std::pair<BST, BST> pair = split_(root, segm);

  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BSTs
  TreeNode* aux;
  aux = TreeNode::min(pair.first.root);
  aux = TreeNode::max(pair.first.root);
  aux = TreeNode::min(pair.second.root);
  aux = TreeNode::max(pair.second.root);
  return pair;
}

void TreeNode::free(TreeNode *node){
  if(node == NULL){
    return;
  }
  TreeNode::free(node->left);
  TreeNode::free(node->right);
  std::free(node);
  node = NULL;
}

TreeNode* get_new_copy_(TreeNode* root){
  if(root == NULL)
    return NULL;
  TreeNode* sol = new TreeNode();
  *sol = *root;
  sol->left = get_new_copy_(root->left);
  sol->right = get_new_copy_(root->right);
  return sol;
}

BST BST::get_new_copy(BST t){
  BST sol;
  if(t.root == NULL)
    return sol;
  sol.root = new TreeNode();
  *sol.root = *t.root;
  sol.root->left = get_new_copy_(t.root->left);
  sol.root->right = get_new_copy_(t.root->right);
  return sol;
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

void print_2D(BST t)
{
  print_2D_util(t.root, 0);
  std::cout<<"----------------------------\n";
}
