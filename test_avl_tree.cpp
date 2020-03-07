#include "avl_tree.hpp"
#include <assert.h>

void test_split_basics(){
  TreeNode* node = NULL;
  std::pair<BST,BST> tree_pair = BST::split(node, Segment());
  assert(tree_pair.first.root == NULL);
  assert(tree_pair.second.root == NULL);

  BST tree;
  Segment s_0011(Point(0,0), Point(1,1));
  tree.insert(s_0011);
  tree_pair = BST::split(tree.root, s_0011);
  assert(tree_pair.first.root == NULL);
  assert(tree_pair.second.root->segm == s_0011);

  Segment s_1122(Point(1,1), Point(2,2));
  tree.insert(s_1122);
  tree_pair = BST::split(tree.root, s_1122);
  assert(tree_pair.first.root->segm == s_0011);
  assert(tree_pair.second.root->segm == s_1122); 
}

void test_insert(){
  BST tree;
  for(int i = 0; i <= 1000; i++){
    tree.insert(Segment(Point(i,i), Point(i+1, i+1)));
    bool balanced = tree.is_balanced();
    assert(tree.is_balanced());
  }
}

void test_split_(int n, Segment s){
  BST tree;
  for(int i = 0; i <= n; i++){
    tree.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  assert(tree.is_balanced());

  std::pair<BST,BST> pair = BST::split(tree.root, s);
  assert(pair.first.is_balanced());
  assert(pair.second.is_balanced());
}

void test_split(){
  for(int i = 0; i <= 100; i++){
    for(int j = 0; j < i; j++){
      test_split_(i, Segment(Point(j, j), Point(j+1, j+1)));
    }
  }
}

void test_join_(int n, int m){
  BST t1, t2, t;
  for(int i = 0; i < n; i++){
    t1.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  for(int i = n; i <= m; i++){
    t2.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  t = BST::join(t1.root, t2.root);
  assert(t.is_balanced());
}

void test_specific_join(){
  BST t1, t2, t;
  for(int i = 8; i <= 19; i++){
    t2.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  t1.insert(Segment(Point(6,6), Point(7,7)));
  Segment s(Point(7,7), Point(8,8));

}
void test_join(){
  for(int i = 0; i < 100; i++){
    for(int j = i+1; j < 100; j++){
      test_join_(i,j);
    }
  }
}

void test_avl_tree(){
  test_insert();
  test_split_basics();
  test_join();
  test_split();
}