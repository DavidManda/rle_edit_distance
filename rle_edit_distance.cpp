#include "avl_tree.hpp"

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
  
  TreeNode *largest_t1 = BST::max(t1.root);
  TreeNode *smallest_t2 = BST::min(t2.root);
  if(largest_t1->segm.right.x != smallest_t2->segm.left.x){
    std::cout<<"Rightmost point of tree 1 should be equal to leftmost point of tree 2\n";
    throw;
  }

  int slope1 = (largest_t1->segm.right.y - largest_t1->segm.left.y) / 
              (largest_t1->segm.right.x - largest_t1->segm.left.x);
  int slope2 = (smallest_t2->segm.right.y - smallest_t2->segm.left.y) / 
              (smallest_t2->segm.right.x - smallest_t2->segm.left.x);
  Segment aux;
  // combine segments if they have the same gradient
  if(slope1 == slope2){
    aux.left = largest_t1->segm.left;
    aux.right = smallest_t2->segm.right;
    t1.delete_node(largest_t1->segm);
    t2.delete_node(smallest_t2->segm);
  }
  else{
    aux = smallest_t2->segm;
    t2.delete_node(smallest_t2->segm);
  }

  return BST::join(t1.root,t2.root,aux);
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
  return sol;
}
// int main(){
//   BST bst1, bst2, *joined;
//   bst1.insert(Segment(Point(0,0),Point(1,0)));
//   bst1.insert(Segment(Point(1,0),Point(2,0)));
//   bst1.insert(Segment(Point(2,0),Point(3,0)));

//   bst2.insert(Segment(Point(3,0),Point(4,0)));
//   bst2.insert(Segment(Point(4,0),Point(5,0)));
//   bst2.insert(Segment(Point(5,0),Point(6,0)));
//   bst2.insert(Segment(Point(6,0),Point(7,0)));
//   bst2.insert(Segment(Point(7,0),Point(8,0)));
//   bst2.insert(Segment(Point(8,0),Point(9,0)));
//   bst2.insert(Segment(Point(9,0),Point(10,0)));
//   bst2.insert(Segment(Point(10,0),Point(11,0)));
//   bst2.insert(Segment(Point(11,0),Point(12,0)));
//   // print_2D(bst2.root);
//   joined = join(&bst1, &bst2);
//   // print_2D(joined->root);
//   std::pair<BST*, BST*> p = split(joined, 5);
//   print_2D(p.first->root);
//   print_2D(p.second->root);
//   return 0;
// }

