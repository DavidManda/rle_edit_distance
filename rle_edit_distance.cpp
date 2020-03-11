#include "avl_tree.hpp"
#include <assert.h>

float get_val_at_coord(float coord, Point p1, Point p2)
{
  if(coord < p1.x || coord > p2.x)
  {
    std::cout<<"Coord should be between the two given points!\n";
    return 0;
  }
  float slope = (p2.y - p1.y)/(p2.x - p1.x);
  
  return p1.y + slope * (coord - p1.x);
}

// this function expects two trees that describe intervals [X_l, X_m] and [X_m, X_r]
// it will return a new balanced tree that is the joining of the two trees
// t1 and t2 will be compromised and shouldn't be used after calling this function
BST join(BST t1, BST t2){
  if(t1.root == NULL){
    return BST(t2.root);
  }
  if(t2.root == NULL){
    return BST(t1.root);
  }
  
  Segment largest_t1 = TreeNode::max(t1.root)->segm;
  Segment smallest_t2 = TreeNode::min(t2.root)->segm;
  if(largest_t1.right.x != smallest_t2.left.x){
    std::cout<<"Rightmost point of tree 1 should be equal to leftmost point of tree 2\n";
    throw;
  }

  int slope1 = (largest_t1.right.y - largest_t1.left.y) / 
              (largest_t1.right.x - largest_t1.left.x);
  int slope2 = (smallest_t2.right.y - smallest_t2.left.y) / 
              (smallest_t2.right.x - smallest_t2.left.x);
  Segment aux;
  BST t_join;
  // combine segments if they have the same gradient
  if(slope1 == slope2){
    aux.left = largest_t1.left;
    aux.right = smallest_t2.right;
    t1.delete_node(largest_t1);
    t2.delete_node(smallest_t2);
    t_join = BST::join(t1.root,t2.root,aux);
    t_join.update_point_type(aux);
  }
  else{
    aux = smallest_t2;
    t2.delete_node(smallest_t2);
    t_join = BST::join(t1.root,t2.root,aux);
    t_join.update_point_type(largest_t1);
    TreeNode *n = t_join.find(largest_t1);
    t_join.update_point_type(smallest_t2);
  }

  return t_join;
}

TreeNode* find_segm_containing(TreeNode* root, float x){
  if(root == NULL || root->segm.contains(x)){
    return root;
  }

  if(x < root->segm.left.x){
    return find_segm_containing(root->left, x);
  }
  if(x > root->segm.right.x){
    return find_segm_containing(root->right, x);
  }
  else{
    std::cout<<"Element " << x <<" is not contained by the interval described by the tree!\n";
    throw;
  }
}

// split a tree describing interval [x_l, x_r] into two trees describing [x_l, x_m],[x_m, x_r]
std::pair<BST, BST> split(BST T, float x_m){
  std::pair<BST, BST> sol;
  TreeNode* node = find_segm_containing(T.root, x_m);
  Segment segm = node->segm;

  T.delete_node(segm);

  if(x_m > segm.left.x && x_m < segm.right.x){
    // If x_m is strictly within the segment, we split it and then split the tree on the 
    // right part of the segment
    float val = get_val_at_coord(x_m, segm.left, segm.right);
    Segment s1 = Segment(segm.left, Point(x_m, val));
    Segment s2 = Segment(Point(x_m, val), segm.right);
    T.insert(s1);
    T.insert(s2);
    sol = BST::split(T.root, s2);
  }
  else if(x_m == segm.left.x){
    // we keep the segment in the right part of the tree by using it for splitting
    T.insert(segm);
    sol = BST::split(T.root,segm);
  }
  else{
    // we keep the segment in the left part of the tree by using its successor to split the tree
    T.insert(segm);
    TreeNode* succ = T.find_succ(segm);
    sol = BST::split(T.root, succ->segm);
  }
  // update type of points of the new boundry elements
  TreeNode* aux = sol.first.root;
  while(aux){
    sol.first.update_point_type(aux->segm);
    aux = aux->right;
  }
  aux = sol.second.root;
  while(aux){
    sol.second.update_point_type(aux->segm);
    aux = aux->left;
  }
  return sol;
}

// Takes the minumum of the functions described by t1 and t2
// The functions should be over the same interval [x_l, x_r]
// Assumes there exists x_m in [x_l, x_r] such that:
// F1(x) > F2(x) if x < x_m and F1(x) <= F2(x) if x >= x_m
// where F1 and F2 are the functions described by t1 and t2
BST combine(BST t1, BST t2){
  Point min1 = TreeNode::min(t1.root)->segm.left;
  Point min2 = TreeNode::min(t2.root)->segm.left;
  Point max1 = TreeNode::max(t1.root)->segm.right;
  Point max2 = TreeNode::max(t2.root)->segm.right;
  assert(min1 == min2);
  assert(max1 == max2);

  BST sol;
  // check if x_m == x_l
  if(min1.y <= min2.y){
    return t1;
  }
  // check if x_m == x_r
  if(max1.y > max2.y){
    return t2;
  }
  


  return sol;
}

