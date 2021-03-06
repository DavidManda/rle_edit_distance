#include "rle_edit_distance.hpp"
#include "avl_tree.hpp"
#include <assert.h>
#include <cstdlib>
#include <ctime>

#define MAX_SIZE 370000
int dyn[2 * MAX_SIZE];
BST LEFT[MAX_SIZE], TOP[2 * MAX_SIZE], OUT[2 * MAX_SIZE];

typedef std::vector<rle::RLE_run> rle_string;
// this function expects two trees that describe intervals [X_l, X_m] and [X_m, X_r]
// it will return a new balanced tree that is the joining of the two trees
// t1 and t2 will be compromised and shouldn't be used after calling this function
inline BST join(BST t1, BST t2)
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
    // point type of the incident segments will be updated in here when aux is inserted
    t_join = BST::join(t1.root, t2.root, aux);
  }
  return t_join;
}

// split a tree describing interval [x_l, x_r] into two trees describing [x_l, x_m],[x_m, x_r]
inline std::pair<BST, BST> split(BST T, float x_m)
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
  if (min1.y <= min2.y)
  {
    TreeNode::free(t2.root);
    return t1;
  }
  // check if x_m == x_r
  if (max1.y > max2.y)
  {
    TreeNode::free(t1.root);
    return t2;
  }
  Segment S1, S2;
  t2.find_leftmost_smaller(t1.root, S1);
  t1.find_rightmost_smaller(t2.root, S2);
  float x_m = Segment::get_intersection(S1, S2).x;
  std::pair<BST, BST> pair_1 = split(t1, x_m);
  std::pair<BST, BST> pair_2 = split(t2, x_m);
  TreeNode::free(pair_1.first.root);
  TreeNode::free(pair_2.second.root);
  return join(pair_2.first, pair_1.second);
}

void get_collapsed_segments(TreeNode *current, std::vector<Segment> &collapsed_segments)
{
  assert(current != NULL);
  TreeNode::lazy_update(current);
  if (current->left && current->left->t_min == 0)
  {
    get_collapsed_segments(current->left, collapsed_segments);
  }
  if (current->right && current->right->t_min == 0)
  {
    get_collapsed_segments(current->right, collapsed_segments);
  }
  // if node is collapsed
  if (current->get_own_tmin() == 0)
  {
    collapsed_segments.push_back(current->segm);
  }
}

BST SWM(BST tree, int h)
{
  TreeNode *left_most = TreeNode::min(tree.root);
  TreeNode *right_most = TreeNode::max(tree.root);

  if (left_most->segm.left.type == _I)
  {
    Segment empty_segm = Segment(left_most->segm.left, left_most->segm.left);
    tree.insert(empty_segm);
  }
  if (right_most->segm.right.type == D_)
  {
    Segment empty_segm = Segment(right_most->segm.right, right_most->segm.right);
    tree.insert(empty_segm);
  }
  while (h > 0)
  {
    int t_min = tree.root->t_min;
    if (h >= t_min)
    {
      tree.request_swm(t_min);
      h -= t_min;
      while (tree.root->t_min == 0)
      {
        std::vector<Segment> collapsed_segments;
        get_collapsed_segments(tree.root, collapsed_segments);
        for (std::vector<Segment>::iterator it = collapsed_segments.begin(); it != collapsed_segments.end(); ++it)
        {
          tree.delete_node(*it);
        }
      }
    }
    else
    {
      tree.request_swm(h);
      h = 0;
    }
  }
  return tree;
}

BST initialise(int n)
{
  if (n == 0)
    return BST();
  return BST(new TreeNode(Segment(Point(0, 0), Point(n, 0))));
}

BST get_OUT_LEFT(BST LEFT, int h, int w)
{
  if (h <= w)
  {
    BST S = SWM(LEFT, h - 1);
    // get the value here before S is compromised
    float s_h = S.get_value_at_coord(h);
    std::pair<BST, BST> p = split(S, h);
    BST S_l = p.first;
    BST S_r = p.second;

    S_l.request_grad_change(1);
    S_l.request_shift(0, -1);
    BST S1 = S_l;

    BST S2 = initialise(w - h);
    S2.request_grad_change(1);
    S2.request_shift(h, s_h + h - 1);

    S_r.request_shift(w - h, w - 1);
    BST S3 = S_r;
    return join(join(S1, S2), S3);
  }
  else
  {
    BST S = SWM(LEFT, w - 1);
    std::pair<BST, BST> p = split(S, w);
    BST S_l = p.first;
    BST S_r = p.second;

    S_l.request_grad_change(1);
    S_l.request_shift(0, -1);
    BST S1 = S_l;

    S_r.request_shift(0, w - 1);
    BST S2 = S_r;
    return join(S1, S2);
  }
}

BST get_OUT_TOP(BST TOP, int h, int w)
{
  if (h <= w)
  {
    BST S = SWM(TOP, h - 1);
    std::pair<BST, BST> p = split(S, w);
    BST S_l = p.first;
    BST S_r = p.second;
    S_l.request_shift(0, h - 1);
    BST S1 = S_l;

    S_r.request_grad_change(-1);
    S_r.request_shift(0, w + h - 1);
    BST S2 = S_r;
    return join(S1, S2);
  }
  else
  {
    BST S = SWM(TOP, w - 1);
    // get the value here before S is compromised
    float s_w = S.get_value_at_coord(w);
    std::pair<BST, BST> p = split(S, w);
    BST S_l = p.first;
    BST S_r = p.second;

    S_l.request_shift(0, h - 1);
    BST S1 = S_l;
    BST S2 = initialise(h - w);
    S2.request_grad_change(-1);
    S2.request_shift(w, s_w + h - 1);

    S_r.request_grad_change(-1);
    S_r.request_shift(h - w, 2 * w - 1);
    BST S3 = S_r;
    return join(join(S1, S2), S3);
  }
}

void init_input_border(BST LEFT[], BST TOP[], int M, int N, rle_string s0, rle_string s1)
{
  int char_count = 0;
  for (int i = 0; i < M; i++)
  {
    // LEFT is indexed bottom-to-up
    BST L = initialise(s0[i].len);
    L.request_grad_change(-1);
    L.request_shift(1, s0[i].len + char_count);
    LEFT[i] = L;
    char_count += s0[i].len;
  }

  char_count = 0;
  for (int j = 0; j < N; j++)
  {
    // TOP is indexed left-to-right
    BST T = initialise(s1[j].len);
    T.request_grad_change(1);
    T.request_shift(1, char_count);
    TOP[0 * N + j] = T;
    char_count += s1[j].len;
  }
}

void get_input_border(BST LEFT[], BST TOP[], BST OUT[], int i, int j, rle_string &s0, rle_string &s1, int current_row)
{
  int M = s0.size(), N = s1.size();
  // LEFT[i][j] might have been initialised (if j == 0) so we don't have to do anything in that case
  if (j > 0)
  {
    // width and height of block to the left
    int w = s1[j - 1].len + 1;
    int h = s0[i].len + 1;
    std::pair<BST, BST> p = split(OUT[current_row * N + (j - 1)], w);
    LEFT[i] = p.second;
    // correct the index
    LEFT[i].request_shift(-w + 1, 0);
    // assign the top border of the block underneath now, as we have a tree representing it
    // this way we don't have to split the same tree twice, which is not allowed
    if (i + 1 < M)
    {
      TOP[(1-current_row) * N + (j - 1)] = p.first;
    }
    else
    {
      // We won't use this so we should free it
      TreeNode::free(p.first.root);
    }
  }

  if (TOP[current_row * N + j].root == NULL)
  {
    assert(i > 0);
    // Width and height of block above
    int w = s1[j].len + 1;
    int h = s0[i - 1].len + 1;
    std::pair<BST, BST> p = split(OUT[(1 - current_row) * N + j], w);
    TOP[current_row * N + j] = p.first;
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

int get_rle_edit_dist(rle_string s0, rle_string s1)
{
  const int M = s0.size();
  const int N = s1.size();
  // reset values
  int current_row = 0, prev_row = 1;
  for (int j = 0; j < N; j++)
  {
    dyn[current_row * N + j] = 0;
    TOP[0 * N + j] = BST();
    TOP[1 * N + j] = BST();
  }
  init_input_border(LEFT, TOP, M, N, s0, s1);
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      int h = s0[i].len + 1;
      int w = s1[j].len + 1;
      // Retrieve input border for current block
      get_input_border(LEFT, TOP, OUT, i, j, s0, s1, current_row);
      if (match(s0[i], s1[j]))
      {
        BST L = LEFT[i];
        BST T = TOP[current_row * N + j];
        // shift top to the right h positions so we can join with left and get OUT
        T.request_shift(h - 1, 0);
        OUT[current_row * N + j] = join(L, T);
      }
      else
      {
        BST OUT_LEFT = get_OUT_LEFT(LEFT[i], h, w);
        BST OUT_TOP = get_OUT_TOP(TOP[current_row * N + j], h, w);
        OUT[current_row * N + j] = combine(OUT_TOP, OUT_LEFT);
      }
      dyn[current_row * N + j] = OUT[current_row * N + j].get_value_at_coord(w);
      // Reset value in TOP
      TOP[current_row * N + j] = BST();
    }
    prev_row = current_row;
    current_row = 1-current_row;
  }
  TreeNode::free(OUT[prev_row * N + N - 1].root);
  return dyn[prev_row * N + N - 1];
}

int get_naive_edit_dist(std::string s0, std::string s1)
{
  // Add a character at the beginning so strings start at index 1
  s0 = "*" + s0;
  s1 = "*" + s1;
  const int M = s0.length(), N = s1.length();
  int current_row = 0,prev_row = 1;
  for (int i = 0; i < N; i++)
  {
    dyn[current_row * N + i] = i;
  }

  for (int i = 1; i < M; i++)
  {
    prev_row = current_row;
    current_row = 1 - current_row;
    dyn[current_row * N + 0] = i;
    for (int j = 1; j < N; j++)
    {
      dyn[current_row * N + j] = std::min(dyn[prev_row * N + (j - 1)] + (s0[i] != s1[j]), std::min(dyn[prev_row * N + j] + 1, dyn[current_row * N + (j - 1)] + 1));
    }
  }

  return dyn[current_row * N + (N - 1)];
}