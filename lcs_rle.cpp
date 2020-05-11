#include "lcs_rle.hpp"

#define MAX_SIZE 500
typedef std::vector<rle::RLE_run> rle_string;

int dyn[2][MAX_SIZE];
BST LEFT[MAX_SIZE * MAX_SIZE], TOP[MAX_SIZE * MAX_SIZE], OUT[MAX_SIZE * MAX_SIZE];

BST initialise(int n)
{
  if (n == 0)
    return BST();
  return BST(new TreeNode(Segment(Point(0, 0), Point(n, 0))));
}

std::pair<BST, BST> split(BST T, float x_m)
{
  std::pair<BST, BST> sol;
  // check if we are splitting inside the tree
  Point min = TreeNode::min(T.root)->segm.left;
  Point max = TreeNode::max(T.root)->segm.right;
  assert(x_m >= min.x && x_m <= max.x);
  if (x_m == min.x)
  {
    sol.first = BST();
    sol.second = T;
    return sol;
  }
  if (x_m == max.x)
  {
    sol.first = T;
    sol.second = BST();
    return sol;
  }
  TreeNode *node = T.root->find_node_containing(x_m);
  Segment segm = node->segm;
  if (x_m > segm.left.x && x_m < segm.right.x)
  {
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
  else if (x_m == segm.left.x)
  {
    // we keep the segment in the right part of the tree by using it for splitting
    sol = BST::split(T.root, segm);
  }
  else
  {
    // we keep the segment in the left part of the tree by using its successor to split the tree
    TreeNode *succ = T.find_succ(segm);
    sol = BST::split(T.root, succ->segm);
  }
  // update type of new endpoints
  TreeNode *min_right = TreeNode::min(sol.second.root);
  TreeNode *max_left = TreeNode::max(sol.first.root);
  sol.second.update_point_type(min_right->segm);
  sol.first.update_point_type(max_left->segm);
  return sol;
}

// this function expects two trees that describe intervals [X_l, X_m] and [X_m, X_r]
// it will return a new balanced tree that is the joining of the two trees
// t1 and t2 will be compromised and shouldn't be used after calling this function
BST join(BST t1, BST t2)
{
  if (t1.root == NULL)
  {
    return BST(t2.root);
  }
  if (t2.root == NULL)
  {
    return BST(t1.root);
  }

  Segment largest_t1 = TreeNode::max(t1.root)->segm;
  Segment smallest_t2 = TreeNode::min(t2.root)->segm;
  assert(largest_t1.right.x == smallest_t2.left.x);
  assert(largest_t1.right.y == smallest_t2.left.y);

  int slope1 = (largest_t1.right.y - largest_t1.left.y) /
               (largest_t1.right.x - largest_t1.left.x);
  int slope2 = (smallest_t2.right.y - smallest_t2.left.y) /
               (smallest_t2.right.x - smallest_t2.left.x);
  Segment aux;
  BST t_join;
  // combine segments if they have the same gradient
  if (slope1 == slope2)
  {
    aux.left = largest_t1.left;
    aux.right = smallest_t2.right;
    t1.delete_node(largest_t1);
    t2.delete_node(smallest_t2);
    t_join = BST::join(t1.root, t2.root, aux);
  }
  else
  {
    aux = smallest_t2;
    t2.delete_node(smallest_t2);
    t_join = BST::join(t1.root, t2.root, aux);
  }
  return t_join;
}

BST combine(BST t1, BST t2)
{
  Point min1 = TreeNode::min(t1.root)->segm.left;
  Point min2 = TreeNode::min(t2.root)->segm.left;
  Point max1 = TreeNode::max(t1.root)->segm.right;
  Point max2 = TreeNode::max(t2.root)->segm.right;
  assert(min1.x == min2.x);
  assert(max1.x == max2.x);
  BST sol;
  // check if x_m == x_l
  if (min1.y >= min2.y)
  {
    TreeNode::free(t2.root);
    return t1;
  }
  // check if x_m == x_r
  if (max1.y <= max2.y)
  {
    TreeNode::free(t1.root);
    return t2;
  }
  Segment S1, S2;
  t2.find_rightmost_smaller(t1.root, S1);
  t1.find_leftmost_smaller(t2.root, S2);
  float x_m = Segment::get_intersection(S1, S2).x;
  std::pair<BST, BST> pair_1 = split(t1, x_m);
  std::pair<BST, BST> pair_2 = split(t2, x_m);
  TreeNode::free(pair_1.first.root);
  TreeNode::free(pair_2.second.root);
  return join(pair_2.first, pair_1.second);
}

BST get_OUT_LEFT(BST LEFT, int h, int w){
  BST S1 = initialise(w-1);
  S1.request_shift(1,LEFT.get_value_at_coord(1));
  BST S2 = LEFT; S2.request_shift(w-1,0);
  return join(S1,S2);
}

BST get_OUT_TOP(BST TOP, int h, int w){
  BST S1 = TOP;
  BST S2 = initialise(h-1); S2.request_shift(w, TOP.get_value_at_coord(w));
  return join(S1,S2);
}

BST get_match_OUT(BST LEFT, BST TOP, int h, int w){
  if(h <= w){
    BST S1 = LEFT; S1.request_grad_change(1); S1.request_shift(0,-1);
    std::pair<BST, BST> p = split(TOP,w-h+1);
    BST S2 = p.first; S2.request_shift(h-1, h-1);
    BST S3 = p.second; S3.request_grad_change(-1); S3.request_shift(h-1,w);
    return join(join(S1,S2),S3);
  }
  else{
    std::pair<BST, BST> p = split(LEFT, w);
    BST S1 = p.first; S1.request_grad_change(1); S1.request_shift(0,-1);
    BST S2 = p.second; S2.request_shift(0,w-1);
    BST S3 = TOP; S3.request_grad_change(-1); S3.request_shift(h-1,w);
    return join(join(S1,S2),S3);
  }
}

void init_input_border(BST LEFT[], BST TOP[], int M, int N, rle_string s0, rle_string s1){
  for(int i = 0; i < M; i++){
    BST L = initialise(s0[i].len);
    L.request_shift(1,0);
    LEFT[i * N + 0] = L;
  }
  for(int j = 0; j < N; j++){
    BST T = initialise(s1[j].len);
    T.request_shift(1,0);
    TOP[0 * N + j] = T;
  }
}

void get_input_border(BST LEFT[], BST TOP[], BST OUT[], int i, int j, rle_string &s0, rle_string &s1)
{
  int M = s0.size(), N = s1.size();
  // LEFT[i][j] might have been initialised (if j == 1) so we don't have to do anything in that case
  if (LEFT[i * N + j].root == NULL)
  {
    assert(j > 0);
    // width and height of block to the left
    int w = s1[j - 1].len + 1;
    int h = s0[i].len + 1;
    std::pair<BST, BST> p = split(OUT[i * N + j - 1], w);
    LEFT[i * N + j] = p.second;
    // correct the index
    LEFT[i * N + j].request_shift(-w + 1, 0);
    // assign the top border of the block underneath now, as we have a tree representing it
    // this way we don't have to split the same tree twice, which is not allowed
    if (i + 1 < M)
    {
      TOP[(i + 1) * N + (j - 1)] = p.first;
    }
    else
    {
      // We won't use this so we should free it
      TreeNode::free(p.first.root);
    }
  }

  if (TOP[i * N + j].root == NULL)
  {
    assert(i > 0);
    // Width and height of block above
    int w = s1[j].len + 1;
    int h = s0[i - 1].len + 1;
    std::pair<BST, BST> p = split(OUT[(i - 1) * N + j], w);
    TOP[i * N + j] = p.first;
    if (j == N - 1)
    {
      // We won't use this so we should free it
      TreeNode::free(p.second.root);
    }
  }
}

bool match(RLE_run a, RLE_run b)
{
  return a.ch == b.ch;
}

int get_lcs_rle(rle_string s0, rle_string s1){
  const int M = s0.size();
  const int N = s1.size();
  // reset values
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      dyn[i][j] = 0;
      LEFT[i * N + j] = BST();
      TOP[i * N + j] = BST();
      OUT[i * N + j] = BST();
    }
  }
  init_input_border(LEFT, TOP, M, N, s0, s1);
  for(int i = 0; i < M; i++){
    for(int j = 0; j < N; j++){
      int h = s0[i].len + 1;
      int w = s1[j].len + 1;
      get_input_border(LEFT, TOP, OUT, i, j, s0, s1);
      if(match(s0[i], s1[j])){
        OUT[i * N + j] = get_match_OUT(LEFT[i * N + j], TOP[i * N + j], h, w);
      }
      else{
        BST OUT_LEFT = get_OUT_LEFT(LEFT[i * N + j], h, w);
        BST OUT_TOP = get_OUT_TOP(TOP[i * N + j], h, w);
        OUT[i * N + j] = combine(OUT_TOP, OUT_LEFT);
      }
      dyn[i][j] = OUT[i * N + j].get_value_at_coord(w);
    }
  }
  TreeNode::free(OUT[(M - 1) * (N) + N - 1].root);
  return dyn[M - 1][N - 1];
}

int get_lcs(std::string &s0, std::string &s1)
{
  s0 = "*" + s0;
  s1 = "*" + s1;
  const int M = s0.length(), N = s1.length();
  int current_row = 0,prev_row = 1;
  for(int i = 0; i < N; i++)
    dyn[current_row][i] = 0;

  for (int i = 1; i < M; i++)
  {
    prev_row = current_row;
    current_row = 1 - current_row;
    dyn[current_row][0] = 0;
    for (int j = 1; j < N; j++)
    {
      dyn[current_row][j] = std::max(dyn[prev_row][j - 1] + (s0[i] == s1[j]), std::max(dyn[prev_row][j], dyn[current_row][j - 1]));
    }
  }

  return dyn[current_row][N-1];
}