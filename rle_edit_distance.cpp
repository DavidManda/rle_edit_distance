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
  }
  else{
    aux = smallest_t2;
    t2.delete_node(smallest_t2);
    // point type of the incident segments will be updated in here when aux is inserted
    t_join = BST::join(t1.root,t2.root,aux);
  }

  return t_join;
}

// split a tree describing interval [x_l, x_r] into two trees describing [x_l, x_m],[x_m, x_r]
std::pair<BST, BST> split(BST T, float x_m){
  std::pair<BST, BST> sol;
  TreeNode* node = T.root->find_node_containing(x_m);
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
  // TODO!! this is not OK because it has complexity O(log(n))^2. Fix this
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

void find_leftmost_smaller(TreeNode *node1, TreeNode* node2, Segment &s){
  if(node1 == NULL)
    return;
  find_leftmost_smaller(node1->left, node2, s);

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

  Segment S1;
  find_leftmost_smaller(t1.root, t2.root, S1);

  return sol;
}

TreeNode* find_and_remove_collapsed_segm(TreeNode *root){
  assert(root != NULL);

  if(root->left && root->left->t_min == 0){
    root->left =  find_and_remove_collapsed_segm(root->left);
  }

  if(root->right && root->right->t_min == 0){
    root->right = find_and_remove_collapsed_segm(root->right);
  }
  // if node is collapsed
  if(root->get_t_min() == 0){
    // wrap node in a BST in order to use the BST API which is updates point types
    BST t = BST(root);
    t.delete_node(root->segm);
    root = t.root;
  }
  return root;
}

BST SWM(BST tree, int h){
  TreeNode *left_most = TreeNode::min(tree.root);
  TreeNode *right_most = TreeNode::max(tree.root);
  
  if(left_most->type_l == _I){
    Segment empty_segm = Segment(left_most->segm.left, left_most->segm.left);
    tree.insert(empty_segm);
  }
  if(right_most->type_r == D_){
    Segment empty_segm = Segment(right_most->segm.right, right_most->segm.right);
    tree.insert(empty_segm);
  }

  while(h > 0){
    int t_min = tree.root->t_min;
    if(h >= t_min){
      tree.apply_swm(t_min);
      h -= t_min;
      while(tree.root->t_min == 0){
        tree.root = find_and_remove_collapsed_segm(tree.root);
      }
    }
    else
    {
      tree.apply_swm(h);
      h = 0;
    }
  }
  return tree;
}

BST initialise(int n){
  return BST(new TreeNode(Segment(Point(1,0), Point(n,0))));
}

BST get_OUT_LEFT(BST LEFT, int h, int w){
  if(h <= w){
    BST S = SWM(LEFT, h);
    std::pair<BST, BST> p = split(S,h);
    BST S_l = p.first;
    BST S_r = p.second;

    S_l.change_grad(1);
    S_l.shift(0,-1);
    BST S1 = S_l;
    
    Segment segm = S.root->find_node_containing(h)->segm;
    float s_h = get_val_at_coord(h, segm.left, segm.right);
    BST S2 = initialise(w-h); S2.change_grad(1); S2.shift(0,s_h + h - 1);
    S_r.shift(0,w-1);
    BST S3 = S_r;
    return join(join(S1,S2),S3);
  }
  else{
    BST S = SWM(LEFT, w);
    std::pair<BST, BST> p = split(S,w);
    BST S_l = p.first;
    BST S_r = p.second;

    S_l.change_grad(1);
    S_l.shift(0,-1);
    BST S1 = S_l;

    S_r.shift(0,w-1);
    BST S2 = S_r;
    return join(S1,S2);
  }
}

BST get_OUT_TOP(BST TOP, int h, int w){
  if(h <= w){
    BST S = SWM(TOP,h);
    std::pair<BST, BST> p = split(S,h);
    BST S_l = p.first;
    BST S_r = p.second;

    S_l.shift(0,h-1);
    BST S1 = S_l;

    S_r.change_grad(-1); S_r.shift(0,w+h-1);
    BST S2 = S_r;
    return join(S1,S2);
  }
  else{
    BST S = SWM(TOP,w);
    std::pair<BST, BST> p = split(S,h);
    BST S_l = p.first;
    BST S_r = p.second;

    S_l.shift(0,h-1);
    BST S1 = S_l;

    Segment segm = S.root->find_node_containing(w)->segm;
    float s_w = get_val_at_coord(w, segm.left, segm.right);
    BST S2 = initialise(h-w); S2.change_grad(-1); S2.shift(w,s_w + h - 1);

    S_r.change_grad(-1); S_r.shift(h-w,w+h-1);
    BST S3 = S_r;
    return join(join(S1,S2),S3);
  }
}
