#include "avl_tree.hpp"
#include <cstddef>
#include <iostream>
#include <assert.h>
#include <limits.h>
#include <cstdlib>
#include <sstream>

#define COUNT 10

TreeNode::TreeNode()
{
  this->active = true;
  this->shift = Point(0, 0);
  this->dg = 0;
  this->dt = 0;
  this->height = 1;
  this->t_min = this->get_own_tmin();
  this->left = NULL;
  this->right = NULL;
}

TreeNode::TreeNode(Segment segm) : TreeNode()
{
  this->segm = segm;
}

TreeNode::TreeNode(Segment segm, TreeNode *left, TreeNode *right) : TreeNode(segm)
{
  this->left = left;
  this->right = right;
  this->recompute_height();
  this->recompute_subtree_tmin();
}

bool has_collapse_time(TreeNode *node)
{
  if (node == NULL)
    return false;
  Point left = node->segm.left, right = node->segm.right;
  return (left.type == ID && right.type == DF) ||
         (left.type == IF && right.type == FD) ||
         (left.type == FI && right.type == ID);
}

int TreeNode::get_own_tmin()
{
  if (has_collapse_time(this))
  {
    return Point::get_manhattan(this->segm.left, this->segm.right);
  }
  return INT_MAX;
}

void TreeNode::recompute_subtree_tmin()
{
  int t_left = (this->left != NULL) ? this->left->t_min : INT_MAX;
  int t_right = (this->right != NULL) ? this->right->t_min : INT_MAX;
  int this_tmin = this->get_own_tmin();

  this->t_min = std::min(this_tmin, std::min(t_left, t_right));
}

void TreeNode::update_value(TreeNode *node)
{
  this->segm = node->segm;
  this->active = node->active;
  this->shift = node->shift;
  this->dg = node->dg;
  this->dt = node->dt;
  this->t_min = node->t_min;
}

int height(TreeNode *node)
{
  if (node == NULL)
    return 0;
  return node->height;
}

void move_point(Point &p, int dt)
{
  Point_t type = p.type;
  if (type == FI || type == IF || type == I_ || type == F_)
  {
    p.x += 1 * dt;
  }
  if (type == ID)
  {
    p.x += 0.5 * (double)dt;
    p.y -= 0.5 * (double)dt;
  }
}

void TreeNode::request_shift(int dx, int dy)
{
  if (dx == 0 && dy == 0)
    return;
  this->active = false;
  this->shift += Point(dx, dy);
}

void TreeNode::request_grad_change(int dg)
{
  if (dg == 0)
    return;
  this->active = false;
  this->dg += dg;
  this->shift += Point(0, dg * this->shift.x);
}

void TreeNode::request_swm(int dt)
{
  if (dt == 0)
    return;
  this->active = false;
  if (this->t_min != INT_MAX)
    this->t_min -= dt;
  if (this->dg == 0)
  {
    this->dt += dt;
  }
  else if (this->dg == 1)
  {
    this->request_shift(dt, 0);
  }
}

void TreeNode::apply_shift()
{
  this->segm.left += this->shift;
  this->segm.right += this->shift;
}

void TreeNode::apply_change_grad()
{
  if (this->dg == 0)
  {
    return;
  }
  this->segm.change_gradient(this->dg);
}

void TreeNode::apply_swm()
{
  if (this->dt == 0)
    return;
  move_point(this->segm.left, this->dt);
  move_point(this->segm.right, this->dt);
}

void TreeNode::lazy_update(TreeNode *node)
{
  if (node == NULL || node->active)
  {
    return;
  }
  node->apply_swm();
  node->apply_change_grad();
  node->apply_shift();

  if (node->left)
  {
    node->left->request_swm(node->dt);
    node->left->request_grad_change(node->dg);
    node->left->request_shift(node->shift.x, node->shift.y);
  }
  if (node->right)
  {
    node->right->request_swm(node->dt);
    node->right->request_grad_change(node->dg);
    node->right->request_shift(node->shift.x, node->shift.y);
  }

  node->active = true;
  node->shift = Point(0, 0);
  node->dg = 0;
  node->dt = 0;
}

TreeNode *TreeNode::rotate_right(TreeNode *root)
{
  TreeNode *left = root->left;
  TreeNode *left_right = left->right;
  TreeNode::lazy_update(root);
  TreeNode::lazy_update(left);
  TreeNode::lazy_update(left_right);
  // rotation
  left->right = root;
  root->left = left_right;

  root->recompute_subtree_tmin();
  root->recompute_height();
  left->recompute_subtree_tmin();
  left->recompute_height();

  return left;
}

TreeNode *TreeNode::rotate_left(TreeNode *root)
{
  TreeNode *right = root->right;
  TreeNode *right_left = right->left;
  TreeNode::lazy_update(root);
  TreeNode::lazy_update(right);
  TreeNode::lazy_update(right_left);
  right->left = root;
  root->right = right_left;

  root->recompute_subtree_tmin();
  root->recompute_height();
  right->recompute_subtree_tmin();
  right->recompute_height();

  return right;
}

TreeNode *TreeNode::insert(TreeNode *root, Segment segm)
{
  if (root == NULL)
    return (new TreeNode(segm));

  TreeNode::lazy_update(root);
  if (segm <= root->segm)
    root->left = insert(root->left, segm);
  else if (segm >= root->segm)
    root->right = insert(root->right, segm);
  else
  {
    // node already exists
    return NULL;
  }
  root->recompute_subtree_tmin();
  root->recompute_height();

  int balance = root->get_balance();

  // left left
  if (balance > 1 && (segm < root->left->segm))
  {
    return TreeNode::rotate_right(root);
  }
  // right right
  if (balance < -1 && segm > root->right->segm)
  {
    return TreeNode::rotate_left(root);
  }
  // left right
  if (balance > 1 && segm > root->left->segm)
  {
    root->left = TreeNode::rotate_left(root->left);
    return TreeNode::rotate_right(root);
  }
  // right left
  if (balance < -1 && segm < root->right->segm)
  {
    root->right = TreeNode::rotate_right(root->right);
    return TreeNode::rotate_left(root);
  }

  // if here no rotation happened
  return root;
}

int TreeNode::get_balance()
{
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
  ss << this->segm.to_string();
  ss << ' ' << this->active << ' ' << this->shift.to_string() << ' ' << this->dt << ' ' << this->dg << ' ' << this->t_min;
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

// log(n)
TreeNode *TreeNode::find_node_containing(float x)
{
  lazy_update(this);
  if (this->segm.contains(x))
  {
    return this;
  }

  if (x < this->segm.left.x && this->left != NULL)
  {
    return this->left->find_node_containing(x);
  }
  if (x > this->segm.right.x && this->right != NULL)
  {
    return this->right->find_node_containing(x);
  }
  else
  {
    std::cout << "Element " << x << " is not contained by the interval described by the tree!\n";
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
      while (tmp->right)
      {
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
      while (tmp->left)
      {
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

BST::BST(TreeNode *root)
{
  this->root = root;
}

bool is_balanced_(TreeNode *root)
{
  if (root == NULL)
    return true;
  int h = root->height;
  root->recompute_height();
  if (h != root->height)
  {
    std::cout << "Heights should already be the correct and updated value, caught in is_balanced\n";
    throw;
  }
  int balance = root->get_balance();
  return balance >= -1 && balance <= 1 && is_balanced_(root->left) && is_balanced_(root->right);
}

bool is_continuous_(TreeNode *root)
{
  if (root == NULL)
    return true;

  TreeNode *pred = NULL, *succ = NULL;
  bool cont_left = true, cont_right = true;
  _find_predec_succ(root, root->segm, pred, succ);
  if (pred != NULL)
  {
    cont_left = (pred->segm.right.x == root->segm.left.x) && (pred->segm.right.y == root->segm.left.y);
  }
  if (succ != NULL)
  {
    cont_right = (root->segm.right.x == succ->segm.left.x) && (root->segm.right.y == succ->segm.left.y);
  }
  return cont_left && cont_right && is_continuous_(root->left) && is_continuous_(root->right);
}
// O(n)
bool BST::is_balanced()
{
  return is_balanced_(root);
}

// O(nlog(n))
bool BST::is_continuous()
{
  return is_continuous_(this->root);
}

void TreeNode::set_right(TreeNode *node)
{
  this->right = node;
  this->recompute_height();
  this->recompute_subtree_tmin();
}

void TreeNode::set_left(TreeNode *node)
{
  this->left = node;
  this->recompute_height();
  this->recompute_subtree_tmin();
}

// inserts node into tree, sets point type and updates t_min
void BST::insert(Segment segm)
{
  if (this->root == NULL)
  {
    this->root = new TreeNode(segm);
    return;
  }

  this->root = TreeNode::insert(this->root, segm);

  this->update_point_type(segm);
}

TreeNode *BST::find(Segment segm)
{
  if (this->root == NULL)
  {
    return NULL;
  }
  return this->root->find(segm);
}

float BST::get_value_at_coord(float x)
{
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
// log(n)
TreeNode *TreeNode::min(TreeNode *node)
{
  if (node == NULL)
  {
    return NULL;
  }

  TreeNode *current = node;
  lazy_update(current);
  /* loop down to find the leftmost leaf */
  while (current && current->left != NULL)
  {
    current = current->left;
    lazy_update(current);
  }

  return current;
}

// log(n)
TreeNode *TreeNode::max(TreeNode *node)
{
  TreeNode *current = node;
  lazy_update(current);
  while (current && current->right != NULL)
  {
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

  if (segm == root->segm)
  {
    // node with only one child or no child
    if (root->left == NULL)
    {
      TreeNode *temp = root->right;
      delete (root);
      return temp;
    }
    else if (root->right == NULL)
    {
      TreeNode *temp = root->left;
      delete (root);
      return temp;
    }

    // node with two children: Get the inorder successor (smallest
    // in the right subtree) - this will be a leaf
    TreeNode *temp = TreeNode::min(root->right);
    TreeNode::lazy_update(temp);
    // Copy the inorder successor's content to this node
    root->update_value(temp);

    // Delete the inorder successor
    root->right = _delete_node(root->right, temp->segm);
  }
  else if (segm <= root->segm)
    root->left = _delete_node(root->left, segm);
  else
    root->right = _delete_node(root->right, segm);

  // If the tree had only one node
  if (root == NULL)
    return root;
  root->recompute_subtree_tmin();
  root->recompute_height();
  int balance = root->get_balance();
  // left left
  if (balance > 1 && root->left->get_balance() >= 0)
  {
    return TreeNode::rotate_right(root);
  }
  // left right
  if (balance > 1 && root->left->get_balance() < 0)
  {
    root->left = TreeNode::rotate_left(root->left);
    return TreeNode::rotate_right(root);
  }
  // right right
  if (balance < -1 && root->right->get_balance() <= 0)
  {
    return TreeNode::rotate_left(root);
  }
  // right left
  if (balance < -1 && root->right->get_balance() > 0)
  {
    root->right = TreeNode::rotate_right(root->right);
    return TreeNode::rotate_left(root);
  }

  return root;
}

TreeNode *TreeNode::delete_node(TreeNode *root, Segment segm)
{
  if (root == NULL)
    return NULL;

  return _delete_node(root, segm);
}

bool should_concat(Segment s1, Segment s2)
{
  if (s1.left.x == s1.right.x || s2.left.x == s2.right.x)
    return false;
  return s1.get_slope() == s2.get_slope() && s1.right == s2.left;
}

void BST::delete_node(Segment segm)
{
  if (this->root == NULL)
  {
    return;
  }
  TreeNode *pred = this->find_predec(segm);
  TreeNode *succ = this->find_succ(segm);
  // Store the values here because the pointers might change or be freed,
  // but we still need the segment values to perform updates
  Segment pred_segm, succ_segm;
  if (pred != NULL)
    pred_segm = pred->segm;
  if (succ != NULL)
    succ_segm = succ->segm;
  this->root = _delete_node(this->root, segm);

  if (pred != NULL && succ != NULL && should_concat(pred_segm, succ_segm))
  {
    Segment concat = Segment(pred_segm.left, succ_segm.right);
    this->root = _delete_node(this->root, pred_segm);
    this->root = _delete_node(this->root, succ_segm);
    this->insert(concat);
  }
  else
  {
    if (pred != NULL)
      this->update_point_type(pred_segm);
    if (succ != NULL)
      this->update_point_type(succ_segm);
  }
}
// returns incident point of the two segments
Point_t get_midpoint_type(Segment s_l, Segment s_r)
{

  if (s_l.right.x == s_l.left.x)
  {
    if (s_r.right.x == s_r.left.x)
    {
      // it can be the case that multiple neighbouring segments have collapsed and will be deleted
      // as they are deleted sequentially, there is a point in time where there are two neighbouring
      // empty segments, but they will be deleted.
      return NotInitialised;
    }
    int slope_r = (s_r.right.y - s_r.left.y) / (s_r.right.x - s_r.left.x);
    if (slope_r == -1)
      return FD;
    return FI;
  }

  if (s_r.right.x == s_r.left.x)
  {
    if (s_l.right.x == s_l.left.x)
    {
      // it can be the case that multiple neighbouring segments have collapsed and will be deleted
      // as they are deleted sequentially, there is a point in time where there are two neighbouring
      // empty segments, but they will be deleted.
      return NotInitialised;
    }
    int slope_l = (s_l.right.y - s_l.left.y) / (s_l.right.x - s_l.left.x);
    if (slope_l == -1)
      return DF;
    return IF;
  }
  int slope_r = (s_r.right.y - s_r.left.y) / (s_r.right.x - s_r.left.x);
  int slope_l = (s_l.right.y - s_l.left.y) / (s_l.right.x - s_l.left.x);
  if (slope_l == -1)
  {
    if (slope_r == 0)
      return DF;
    if (slope_r == 1)
      return DI;
  }
  if (slope_l == 0)
  {
    if (slope_r == -1)
      return FD;
    if (slope_r == 1)
      return FI;
  }
  if (slope_l == 1)
  {
    if (slope_r == -1)
      return ID;
    if (slope_r == 0)
      return IF;
  }
  return NotInitialised;
}

// O(log(n))
void BST::update_point_type(Segment segm)
{
  TreeNode *node = this->find(segm);
  assert(node != NULL);
  TreeNode *predec = this->find_predec(segm);
  TreeNode *succ = this->find_succ(segm);
  node->segm.update_endpoints();
  Segment s = node->segm;
  if (succ != NULL)
  {
    Segment s_r = succ->segm;
    Point_t type = get_midpoint_type(s, s_r);
    // Here we insert empty segment ONLY if the two segments are actually continuous
    // it can be the case that they are not when we perform a delete in order to split
    // that segment in two. After the delete and before the inserts there is a gap in the tree
    // and we should not insert an empty segment there
    if (type != DI || s.right.x != s_r.left.x)
    {
      node->segm.right.type = type;
      succ->segm.left.type = type;
      this->update_tmin_on_path_to(succ->segm);
    }
    else
    {
      Segment empty_segm = Segment(s.right, s.right);
      this->insert(empty_segm);
    }
  }

  if (predec != NULL)
  {
    Segment s_l = predec->segm;
    Point_t type = get_midpoint_type(s_l, s);
    // Here we insert empty segment ONLY if the two segments are actually continuous
    // it can be the case that they are not when we perform a delete in order to split
    // that segment in two. After the delete and before the inserts there is a gap in the tree
    // and we should not insert an empty segment there
    if (type != DI || s_l.right.x != s.left.x)
    {
      node->segm.left.type = type;
      predec->segm.right.type = type;
      this->update_tmin_on_path_to(predec->segm);
    }
    else
    {
      Segment empty_segm = Segment(s.left, s.left);
      this->insert(empty_segm);
    }
  }
  this->update_tmin_on_path_to(node->segm);
}

// O(log(n))
void update_tmin_on_path_to_(TreeNode *root, Segment segm)
{
  assert(root != NULL);
  if (root->segm == segm)
  {
    root->recompute_subtree_tmin();
  }
  else if (root->segm >= segm)
  {
    update_tmin_on_path_to_(root->left, segm);
  }
  else
  {
    update_tmin_on_path_to_(root->right, segm);
  }
  root->recompute_subtree_tmin();
}

void BST::update_tmin_on_path_to(Segment segm)
{
  update_tmin_on_path_to_(this->root, segm);
}

void BST::request_shift(int dx, int dy)
{
  if (this->root == NULL)
    return;
  this->root->request_shift(dx, dy);
  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BST
  // TreeNode *min = TreeNode::min(this->root);
  // TreeNode *max = TreeNode::max(this->root);
}

void BST::request_grad_change(int dg)
{
  if (this->root == NULL)
    return;
  this->root->request_grad_change(dg);

  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BST
  // TreeNode *min = TreeNode::min(this->root);
  // TreeNode *max = TreeNode::max(this->root);
}

void BST::request_swm(int dt)
{
  this->root->request_swm(dt);

  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BST
  // TreeNode *min = TreeNode::min(this->root);
  // TreeNode *max = TreeNode::max(this->root);
}

TreeNode *join_right(TreeNode *t_l, TreeNode *t_r, Segment segm)
{
  TreeNode::lazy_update(t_r);
  TreeNode::lazy_update(t_l);
  TreeNode *left_left = t_l->left;
  TreeNode *left_right = t_l->right;
  TreeNode::lazy_update(left_left);
  TreeNode::lazy_update(left_right);
  Segment left_segm = t_l->segm;
  if (height(left_right) < height(t_r) + 1)
  {
    TreeNode *aux = new TreeNode(segm, left_right, t_r);
    if (height(aux) <= height(left_left) + 1)
    {
      t_l->set_right(aux);
      return t_l;
    }
    else
    {
      t_l->set_right(TreeNode::rotate_right(aux));
      return TreeNode::rotate_left(t_l);
    }
  }
  else
  {
    TreeNode *aux = join_right(left_right, t_r, segm);
    t_l->set_right(aux);
    if (height(t_l->right) > height(t_l->left) + 1)
      return TreeNode::rotate_left(t_l);
    else
      return t_l;
  }
}

TreeNode *join_left(TreeNode *t_l, TreeNode *t_r, Segment segm)
{
  TreeNode::lazy_update(t_r);
  TreeNode::lazy_update(t_l);
  TreeNode *right_right = t_r->right;
  TreeNode *right_left = t_r->left;
  Segment right_segm = t_r->segm;
  TreeNode::lazy_update(right_left);
  TreeNode::lazy_update(right_right);

  if (height(right_left) < height(t_l) + 1)
  {
    TreeNode *aux = new TreeNode(segm, t_l, right_left);
    if (height(aux) <= height(right_right) + 1)
    {
      t_r->set_left(aux);
      return t_r;
    }
    else
    {
      t_r->set_left(TreeNode::rotate_left(aux));
      return TreeNode::rotate_right(t_r);
    }
  }
  else
  {
    TreeNode *aux = join_left(t_l, right_left, segm);
    t_r->set_left(aux);
    if (height(t_r->left) > height(t_r->right) + 1)
      return TreeNode::rotate_right(t_r);
    else
      return t_r;
  }
}

// joins the two trees and inserts a node with segm in it
BST BST::join(TreeNode *t_l, TreeNode *t_r, Segment segm)
{
  BST joined_tree = BST();
  TreeNode *root;

  if (t_l == NULL && t_r == NULL)
  {
    joined_tree.insert(segm);
    return joined_tree;
  }
  if (t_l == NULL)
  {
    joined_tree.root = t_r;
    joined_tree.insert(segm);
    return joined_tree;
  }
  if (t_r == NULL)
  {
    joined_tree.root = t_l;
    joined_tree.insert(segm);
    return joined_tree;
  }

  if (t_l->height > t_r->height + 1)
  {
    root = join_right(t_l, t_r, segm);
  }
  else if (t_r->height > t_l->height + 1)
  {
    root = join_left(t_l, t_r, segm);
  }
  else
  {
    root = new TreeNode(segm, t_l, t_r);
  }
  joined_tree.root = root;
  joined_tree.update_point_type(segm);
  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BST
  // TreeNode *min = TreeNode::min(joined_tree.root);
  // TreeNode *max = TreeNode::max(joined_tree.root);
  return joined_tree;
}

BST BST::join(TreeNode *t_l, TreeNode *t_r)
{
  BST joined_tree;
  if (t_l == NULL && t_r == NULL)
  {
    return joined_tree;
  }
  if (t_l == NULL)
  {
    joined_tree.root = t_r;
    return joined_tree;
  }
  if (t_r == NULL)
  {
    joined_tree.root = t_l;
    return joined_tree;
  }
  Segment min_right = TreeNode::min(t_r)->segm;
  t_r = _delete_node(t_r, min_right);
  return BST::join(t_l, t_r, min_right);
}

// splits the tree, keeping the segment in the right partition of the tree
// This function has side effect for the subtree rooted at root, which is compromised
std::pair<BST, BST> split_(TreeNode *root, Segment segm)
{
  std::pair<BST, BST> pair;
  if (root == NULL)
  {
    return pair;
  }
  TreeNode::lazy_update(root);
  if (segm == root->segm)
  {
    BST left = (root->left != NULL) ? BST(root->left) : BST();
    BST right = (root->right != NULL) ? BST(root->right) : BST();
    right.insert(segm);
    // free root as we will lose pointer to it, it's value was inserted in right
    delete (root);
    return std::pair<BST, BST>(left, right);
  }
  if (segm <= root->segm)
  {
    std::pair<BST, BST> aux = split_(root->left, segm);
    BST right = BST::join(aux.second.root, root->right, root->segm);
    delete (root);
    return std::pair<BST, BST>(aux.first, right);
  }
  std::pair<BST, BST> aux = split_(root->right, segm);
  BST left = BST::join(root->left, aux.first.root, root->segm);
  delete (root);
  return std::pair<BST, BST>(left, aux.second);
}

std::pair<BST, BST> BST::split(TreeNode *root, Segment segm)
{
  std::pair<BST, BST> pair = split_(root, segm);
  // This ensures the invariant that no deferred changes are stored
  //  on the leftmost and on the rightmost path of the BSTs
  // TreeNode *aux;
  // aux = TreeNode::min(pair.first.root);
  // aux = TreeNode::max(pair.first.root);
  // aux = TreeNode::min(pair.second.root);
  // aux = TreeNode::max(pair.second.root);
  return pair;
}

void TreeNode::free(TreeNode *node)
{
  if (node == NULL)
  {
    return;
  }
  TreeNode::free(node->left);
  TreeNode::free(node->right);
  delete (node);
  node = NULL;
}

TreeNode *get_new_copy_(TreeNode *root)
{
  if (root == NULL)
    return NULL;
  TreeNode *sol = new TreeNode();
  *sol = *root;
  sol->left = get_new_copy_(root->left);
  sol->right = get_new_copy_(root->right);
  return sol;
}

// Finds leftmost node in the subtree rooted at "root" whose value at the right end
// of the segment is <= than the value in the tree on which this method is called
// O(log^2(n))
void BST::find_leftmost_smaller(TreeNode *root, Segment &s)
{
  if (root == NULL)
    return;

  TreeNode::lazy_update(root);
  float val = this->get_value_at_coord(root->segm.right.x);
  if (root->segm.right.y > val)
  {
    this->find_leftmost_smaller(root->right, s);
  }
  else if (root->segm.right.y <= val)
  {
    s = root->segm;
    this->find_leftmost_smaller(root->left, s);
  }
}

// Finds rightmost node in the subtree rooted at "root" whose value at the left end
// of the segment is < than the value in the tree on which this method is called
// O(log^2(n))
void BST::find_rightmost_smaller(TreeNode *root, Segment &s)
{
  if (root == NULL)
    return;

  TreeNode::lazy_update(root);
  float val = this->get_value_at_coord(root->segm.left.x);
  if (root->segm.left.y >= val)
  {
    this->find_rightmost_smaller(root->left, s);
  }
  else
  {
    s = root->segm;
    this->find_rightmost_smaller(root->right, s);
  }
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
  std::cout << "----------------------------\n";
}
