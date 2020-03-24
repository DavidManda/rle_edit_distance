#include "rle_edit_distance.hpp"
#include "avl_tree.hpp"
#include <assert.h>
#include <cstdlib>

typedef std::vector<std::vector<BST> > border_t;
typedef std::vector<rle::RLE_run> rle_string;
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
  assert(largest_t1.right.x == smallest_t2.left.x);

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
  // check if we are splitting inside the tree
  Point min = TreeNode::min(T.root)->segm.left;
  Point max = TreeNode::max(T.root)->segm.right;
  assert(x_m >= min.x && x_m <= max.x);
  if(x_m == min.x){
    sol.first = BST();
    sol.second = T;
    return sol;
  }
  if(x_m == max.x){
    sol.first = T;
    sol.second = BST();
    return sol;
  }
  TreeNode* node = T.root->find_node_containing(x_m);
  Segment segm = node->segm;
  if(x_m > segm.left.x && x_m < segm.right.x){
    // If x_m is strictly within the segment, we split it and then split the tree on the 
    // right part of the segment
    T.delete_node(segm);
    float val = segm.get_val_at_coord(x_m);
    Segment s1 = Segment(segm.left, Point(x_m, val));
    Segment s2 = Segment(Point(x_m, val), segm.right);
    T.insert(s1);
    T.insert(s2);
    sol = BST::split(T.root, s2);
  }
  else if(x_m == segm.left.x){
    // we keep the segment in the right part of the tree by using it for splitting
    sol = BST::split(T.root,segm);
  }
  else{
    // we keep the segment in the left part of the tree by using its successor to split the tree
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

void find_leftmost_smaller(TreeNode *node, BST T, Segment &s, bool &found_segm){

  if(node == NULL || found_segm)
    return;
  find_leftmost_smaller(node->left, T, s, found_segm);

  float val = T.get_value_at_coord(node->segm.right.x);
  if(node->segm.right.y <= val && found_segm == false){
    s = node->segm;
    found_segm = true;
    return;
  }

  find_leftmost_smaller(node->right, T, s, found_segm);
}

void find_rightmost_larger(TreeNode *node, BST T, Segment &s, bool &found_segm){
  if(node == NULL || found_segm)
    return;
  find_rightmost_larger(node->right, T, s, found_segm);
  float val = T.get_value_at_coord(node->segm.left.x);
  if(val > node->segm.left.y && found_segm == false){
    s = node->segm;
    found_segm = true;
    return;
  }

  find_rightmost_larger(node->left, T, s, found_segm);
}

BST combine(BST t1, BST t2){
  Point min1 = TreeNode::min(t1.root)->segm.left;
  Point min2 = TreeNode::min(t2.root)->segm.left;
  Point max1 = TreeNode::max(t1.root)->segm.right;
  Point max2 = TreeNode::max(t2.root)->segm.right;
  assert(min1.x == min2.x);
  assert(max1.x == max2.x);
  BST sol;
  // check if x_m == x_l
  // std::cout<<min1.to_string()<<' '<<min2.to_string()<<'\n';
  if(min1.y <= min2.y){
    return t1;
  }
  // check if x_m == x_r
  if(max1.y > max2.y){
    return t2;
  }
  Segment S1, S2;
  bool found_segm = false;
  find_leftmost_smaller(t1.root, t2, S1, found_segm);
  found_segm = false;
  find_rightmost_larger(t2.root, t1, S2, found_segm);

  float x_m  = Segment::get_intersection(S1, S2).x;
  std::pair<BST, BST> pair_1 = split(t1, x_m);
  std::pair<BST, BST> pair_2 = split(t2, x_m);
  return join(pair_2.first, pair_1.second);
}

TreeNode* find_and_remove_collapsed_segm(TreeNode *root){
  assert(root != NULL);
  TreeNode::lazy_update(root);
  if(root->left && root->left->t_min == 0){
    root->left =  find_and_remove_collapsed_segm(root->left);
    root->recompute_tmin();
  }

  if(root->right && root->right->t_min == 0){
    root->right = find_and_remove_collapsed_segm(root->right);
    root->recompute_tmin();
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

    // print_2D(tree);
  return tree;
}

BST initialise(int n){
  if(n == 0)
    return BST();
  return BST(new TreeNode(Segment(Point(0,0), Point(n,0))));
}

BST get_OUT_LEFT(BST LEFT, int h, int w){
  if(h <= w){
    BST S = SWM(LEFT, h-1);
    // print_2D(S);
    // get the value here before S is compromised
    float s_h = S.get_value_at_coord(h);
    std::pair<BST, BST> p = split(S,h);
    BST S_l = p.first;
    BST S_r = p.second;

    S_l.change_grad(1);
    S_l.shift(0,-1);
    BST S1 = S_l;
    
    BST S2 = initialise(w-h); S2.change_grad(1); S2.shift(h,s_h + h - 1);

    S_r.shift(w-h,w-1);
    BST S3 = S_r;
    // print_2D(S1);
    // print_2D(S2);
    // print_2D(S3);
    return join(join(S1,S2),S3);
  }
  else{
    // std::cout<<"askhgdkjhasg\n";
    // print_2D(LEFT);
    BST S = SWM(LEFT, w-1);
    // print_2D(S);
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
    BST S = SWM(TOP,h-1);
    std::pair<BST, BST> p = split(S,w);
    BST S_l = p.first;
    BST S_r = p.second;
    S_l.shift(0,h-1);
    BST S1 = S_l;

    S_r.change_grad(-1); S_r.shift(0,w+h-1);
    BST S2 = S_r;
    return join(S1,S2);
  }
  else{
    BST S = SWM(TOP,w-1);
    // get the value here before S is compromised
    float s_w = S.get_value_at_coord(w);
    std::pair<BST, BST> p = split(S,w);
    BST S_l = p.first;
    BST S_r = p.second;

    S_l.shift(0,h-1);
    BST S1 = S_l;
    BST S2 = initialise(h-w); S2.change_grad(-1); S2.shift(w,s_w + h - 1);

    S_r.change_grad(-1); S_r.shift(h - w,2*w-1);
    BST S3 = S_r;
    // print_2D(S1);
    // print_2D(S2);
    // print_2D(S3);
    return join(join(S1,S2),S3);
  }
}

void init_input_border(border_t &LEFT, border_t &TOP, int M, int N, rle_string s0, rle_string s1)
{
  int char_count = 0;
  for (int i = 1; i < M; i++)
  {
    // LEFT is indexed bottom-to-up
    LEFT[i][1] = BST(new TreeNode(Segment(Point(1, s0[i].len + char_count), 
                                          Point(s0[i].len + 1, char_count))));
    char_count += s0[i].len;
  }

  char_count = 0;
  for (int j = 1; j < N; j++)
  {
    // top is indexed left-to-right
    TOP[1][j] = BST(new TreeNode(Segment(Point(1, char_count), 
                                         Point(s1[j].len + 1, s1[j].len + char_count))));
    char_count += s1[j].len;
  }
}

void get_input_border(border_t &LEFT, border_t &TOP, border_t OUT, int i, int j, std::vector<std::vector<int > > dyn, rle_string s0, rle_string s1)
{
  // LEFT[i][j] might have been initialised (if j == 1) so we don't have to do anything in that case
  if (LEFT[i][j].root == NULL)
  {
    assert(j > 1);
    // width and height of block to the left
    int w = s1[j - 1].len + 1;
    int h = s0[i].len + 1;
    BST aux = BST::get_new_copy(OUT[i][j-1]);
    // TODO make sure we are splitting at the correct point
    LEFT[i][j] = split(aux, w).second;
    // correct the index
    LEFT[i][j].shift(-w + 1,0);
  }

  if (TOP[i][j].root == NULL)
  {
    assert(i > 1);
    // Width and height of block above
    int w = s1[j].len + 1;
    int h = s0[i - 1].len + 1;
    // TODO make sure we are splitting at the correct point
    // print_2D(OUT[i-1][j]);
    BST aux = BST::get_new_copy(OUT[i-1][j]);
    TOP[i][j] = split(aux, w).first;
    // print_2D(TOP[i][j]);
  }
}

int get_rle_edit_dist(rle_string s0, rle_string s1){
  const int M = s0.size();
  const int N = s1.size();
  std::vector< std::vector< int > > dyn(M, std::vector<int>(N));
  border_t LEFT(M, std::vector<BST>(N));
  border_t TOP(M, std::vector<BST>(N));
  border_t OUT(M, std::vector<BST>(N));
  // RLE_string_helper rle_helper;
  // std::cout<<rle_helper.to_string(s0)<<'\n'<<rle_helper.to_string(s1)<<'\n';
  init_input_border(LEFT, TOP, M, N, s0, s1);
  for (int i = 1; i < M; i++)
  {
    for (int j = 1; j < N; j++)
    {
      int h = s0[i].len + 1;
      int w = s1[j].len + 1;
      // Retrieve input border for current block
      std::cout<<i<<' '<<j<<'\n';
      get_input_border(LEFT, TOP, OUT, i, j, dyn, s0, s1);
      // print_2D(LEFT[i][j]);print_2D(TOP[i][j]);
      // std::cout<<"here\n";
      if(s0[i].ch == s1[j].ch)
      {
        BST L = LEFT[i][j];
        BST T = TOP[i][j];
        // if(i == 2 && j == 1){
        //   print_2D(L);
        // }
        // shift top to the right h positions so we can join with left and get OUT
        T.shift(h - 1,0);
        // if(i == 2 && j == 1){
        //   print_2D(T);
        // }
        // std::cout<<"before join\n";
        OUT[i][j] = join(L,T);
        // if(i == 2 && j == 1){
        //   print_2D(OUT[i][j]);
        // }
      }
      else
      {
        // if(i == 3 && j == 3){
        //   print_2D(LEFT[i][j]);
        //   print_2D(TOP[i][j]);
        // }
        BST OUT_LEFT = get_OUT_LEFT(LEFT[i][j], h, w);
        // std::cout<<"here\n";
        BST OUT_TOP = get_OUT_TOP(TOP[i][j], h, w);
        // if(i == 3 && j == 3){
        //   print_2D(OUT_LEFT);
        //   print_2D(OUT_TOP);
        // }
        OUT[i][j] = combine(OUT_TOP, OUT_LEFT);
        // if(i == 2 && j == 2){
        //   print_2D(OUT[i][j]);
        // }
      }
      // print_2D(OUT[i][j]);

      dyn[i][j] = OUT[i][j].get_value_at_coord(w);
    }
  }
  // for (int i = 1; i < M; i++)
  // {
  //   for (int j = 1; j < N; j++)
  //   {
  //     TreeNode::free(LEFT[i][j].root);
  //     TreeNode::free(TOP[i][j].root);
  //     TreeNode::free(OUT[i][j].root);
  //   }
  // }
  return dyn[M-1][N-1];
}

int get_naive_edit_dist(const int M, const int N, const std::string &s0, const std::string &s1){
  int dyn[M + 1][N + 1];

  for (int i = 0; i <= M; i++)
  {
    dyn[i][0] = i;
  }

  for (int i = 0; i <= N; i++)
  {
    dyn[0][i] = i;
  }

  for (int i = 1; i <= M; i++)
  {
    for (int j = 1; j <= N; j++)
    {
      dyn[i][j] = std::min(dyn[i - 1][j - 1] + (s0[i] != s1[j]),std::min(dyn[i - 1][j] + 1, dyn[i][j - 1] + 1));
    }
  }

  return dyn[M][N];
}