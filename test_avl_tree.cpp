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
  TreeNode::free(t.root);
}

void test_specific_join(){
  BST t1, t2, t;
  for(int i = 8; i <= 19; i++){
    t2.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  t1.insert(Segment(Point(6,6), Point(7,7)));
  Segment s(Point(7,7), Point(8,8));

}

void test_join_avl(){
  for(int i = 0; i < 100; i++){
    for(int j = i+1; j < 100; j++){
      test_join_(i,j);
    }
  }
}

void test_delete(){
  BST tree;
  for(int i = 0; i <= 100; i++){
    tree.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  for(int i = 20; i <= 90; i++){
    tree.delete_node(Segment(Point(i,i), Point(i+1, i+1)));
    bool balanced = tree.is_balanced();
    if(!balanced){
      std::cout<<i<<'\n';
      print_2D(tree.root);
    }
    assert(tree.is_balanced());
  }
}

void test_simple_shift(){
  TreeNode* node = new TreeNode(Segment(Point(0,0), Point(1,1)));
  node->shift(1,1);
  TreeNode* aux = node->find(Segment(Point(1,1), Point(2,2)));
  assert(aux != NULL);
  assert(aux->segm == Segment(Point(1,1), Point(2,2)));
}

void test_shift_propagates(){
  BST tree;
  for(int i = 0; i <= 50; i++){
    tree.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  tree.root->shift(5, 6);

  for(int i = 0; i <= 50; i++){
    TreeNode* node = tree.find(Segment(Point(i+5,i+6), Point(i+1+5, i+1+6)));
    assert(node != NULL);
  }
}

void test_delete_with_lazy_simple(){
  BST tree;
  tree.insert(Segment(Point(0,0), Point(1, 1)));
  tree.root->shift(1,2);
  tree.delete_node(Segment(Point(1,2), Point(2, 3)));
  assert(tree.root == NULL);
}

void test_delete_with_lazy(){
  BST tree;
  for(int i = 0; i <= 2; i++){
    tree.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  // root node must have segment (1,1) - (2,2)
  tree.root->shift(2, 1);
  // delete root
  tree.delete_node(Segment(Point(3,2), Point(4, 3)));
  TreeNode* node1 = tree.find(Segment(Point(2,1), Point(3, 2)));
  TreeNode* node2 = tree.find(Segment(Point(4,3), Point(5, 4)));
  assert(node1 != NULL);
  assert(node2 != NULL);
}

void test_join_with_lazy_update_(int n, int m)
{
  BST t1, t2, t;
  for(int i = 0; i < n; i++){
    t1.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  for(int i = n; i <= m; i++){
    t2.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  t1.root->shift(1,0);
  t2.root->shift(2,3);
  t = BST::join(t1.root, t2.root);
  // check that the t1 part of t has the correct shift applied to it
  for(int i = 0; i < n; i++){
    TreeNode* node =  t.find(Segment(Point(i+1,i+0), Point(i+1+1, i+1+0)));
    assert(node != NULL);
  }
  // print_2D(t.root);
  // check that the t2 part of t has the correct shift applied to it
  for(int i = n; i <= m; i++){
    TreeNode* node =  t.find(Segment(Point(i+2,i+3), Point(i+1+2, i+1+3)));
    assert(node != NULL);
  }
  // TreeNode::free(t.root);
}

void test_join_with_lazy_update(){
  for(int i = 1; i < 100; i++){
    for(int j = i+1; j < 100; j++){
      test_join_with_lazy_update_(i,j);
    }
  }
}

void test_split_with_lazy_update_simple(){
  BST tree;
  for(int i = 0; i <= 2; i++){
    tree.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  // root node must have segment (1,1) - (2,2)
  tree.root->shift(2, 1);
  std::pair<BST, BST> trees = BST::split(tree.root, Segment(Point(3,2), Point(4,3)));
  TreeNode *node = trees.first.find(Segment(Point(2,1), Point(3,2)));
  assert(node != NULL);
  node = trees.second.find(Segment(Point(4,3), Point(5,4)));
  assert(node != NULL);
}

void test_split_with_lazy_update_(int x){
  int N = 100;
  assert(x <= N);
  BST tree;
  for(int i = 0; i <= N; i++){
    tree.insert(Segment(Point(i,i), Point(i+1, i+1)));
  }
  tree.root->shift(10,10);
  std::pair<BST, BST> trees = BST::split(tree.root, Segment(Point(x+10,x+10), Point(x+1+10,x+1+10)));
  for(int i = 0; i < x; i++){
    TreeNode *node = trees.first.find(Segment(Point(i+10,i+10), Point(i+1+10,i+1+10)));
    assert(node != NULL);
  }
  for(int i = x; i <= N; i++){
    TreeNode *node = trees.second.find(Segment(Point(i+10,i+10), Point(i+1+10,i+1+10)));
    assert(node != NULL);
  }
}

void test_split_with_lazy_update(){
  for(int i = 2; i < 50; i++){
    test_split_with_lazy_update_(i);
  }
}

void test_simple_gradient_change(){
  BST t;
  for(int i = 0; i<3; i++){
    t.insert(Segment(Point(i,1), Point(i+1, 1)));
  }
  t.root->change_grad(1);
  t.root->shift(0,2);
  for(int i = 0; i < 3; i++){
    TreeNode *node = t.find(Segment(Point(i,1 + i + 2), Point(i+1, 1 + (i + 1) + 2)));
    assert(node != NULL);
  }
}

void test_gradient_and_shift(){
  BST tree;
  for(int i = 0; i <= 100; i++){
    tree.insert(Segment(Point(i,0), Point(i+1, 0)));
  }
  tree.root->change_grad(1);
  tree.root->shift(5, 6);

  for(int i = 0; i <= 50; i++){
    TreeNode* node = tree.find(Segment(Point(i+5,i+6), Point(i+1+5, i+1+6)));
    assert(node != NULL);
  }
}

void test_lazy_propagation(){
  test_simple_shift();
  test_shift_propagates();
  test_delete_with_lazy_simple();
  test_delete_with_lazy();
  test_join_with_lazy_update();
  test_split_with_lazy_update_simple();
  test_split_with_lazy_update();
  test_simple_gradient_change();
  test_gradient_and_shift();
}

bool left_right_paths_have_no_pending_updates(TreeNode *root){
  bool sol = true;
  TreeNode *aux = root;
  while(aux && sol){
    sol = sol && aux->active;
    aux = aux->left;
  }
  aux = root;
  while(aux && sol){
    sol = sol && aux->active;
    aux = aux->right;
  }
  return sol;
}

void test_invariant_lr_insert(){
  BST tree;
  for(int i = 0; i <= 100; i++){
    tree.insert(Segment(Point(i,1), Point(i+1, 1)));
  }
  tree.apply_swm(3);
  assert(left_right_paths_have_no_pending_updates(tree.root));
  tree.change_grad(0);
  assert(left_right_paths_have_no_pending_updates(tree.root));
  tree.shift(0,2);
  assert(left_right_paths_have_no_pending_updates(tree.root));
}

void test_invariant_lr_split_(int x){
  int N = 10;
  assert(x <= N);
  BST tree;
  for(int i = 0; i <= N; i++){
    tree.insert(Segment(Point(i,1), Point(i+1, 1)));
  }
  tree.apply_swm(4);
  tree.change_grad(-1);
  tree.shift(3,4);
  std::pair<BST, BST> trees = BST::split(tree.root, tree.root->left->segm);
  assert(left_right_paths_have_no_pending_updates(trees.first.root));
  assert(left_right_paths_have_no_pending_updates(trees.second.root));
}

void test_invariant_lr_split(){
  for(int i = 2; i < 5; i++){
    test_invariant_lr_split_(i);
  }
}

void test_invariant_left_right_path(){
  test_invariant_lr_insert();
  test_invariant_lr_split();
}

void test_avl_tree(){
  test_insert();
  test_split_basics();
  test_join_avl();
  test_split();
  test_delete();
  test_lazy_propagation();
  test_invariant_left_right_path();
}