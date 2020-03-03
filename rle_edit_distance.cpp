#include "avl_tree.hpp"

// this function expects two trees that describe intervals [X_l, X_m] and [X_m, X_r]
// it will return a new balanced tree that is the joining of the two trees
BST* join(BST *t1, BST *t2){
  TreeNode *largest_t1 = BST::max(t1->root);
  TreeNode *smallest_t2 = BST::min(t2->root);
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
    t1->delete_node(largest_t1->segm);
    t2->delete_node(smallest_t2->segm);
  }
  else{
    aux = smallest_t2->segm;
    t2->delete_node(smallest_t2->segm);
  }
  // return new BST();
  return BST::join(t1,t2,aux);
}

int main(){
  BST bst1, bst2, *joined;
  bst1.insert(Segment(Point(0,0),Point(1,0)));
  bst1.insert(Segment(Point(1,0),Point(2,0)));
  bst1.insert(Segment(Point(2,0),Point(3,0)));

  bst2.insert(Segment(Point(3,0),Point(4,0)));
  bst2.insert(Segment(Point(4,0),Point(5,0)));
  bst2.insert(Segment(Point(5,0),Point(6,0)));
  bst2.insert(Segment(Point(6,0),Point(7,0)));
  bst2.insert(Segment(Point(7,0),Point(8,0)));
  bst2.insert(Segment(Point(8,0),Point(9,0)));
  bst2.insert(Segment(Point(9,0),Point(10,0)));
  bst2.insert(Segment(Point(10,0),Point(11,0)));
  bst2.insert(Segment(Point(11,0),Point(12,0)));
  // print_2D(bst2.root);
  joined = join(&bst1, &bst2);
  print_2D(joined->root);
  return 0;
}

