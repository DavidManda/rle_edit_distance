#include "avl_tree.hpp"
#include "segment.hpp"
#include <iostream>

int main(){
  TreeNode node1 = TreeNode(Segment(Point(0,0),Point(1,1)));
  TreeNode node2 = node1;
  std::cout << node1.to_string() << node2.to_string() << '\n';
  node1.height = 100;
  node1.segm.left.x = 100;

  std::cout << node1.to_string() << node2.to_string() << '\n';
}