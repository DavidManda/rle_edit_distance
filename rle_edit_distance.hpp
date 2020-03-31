#ifndef rle_ED
#define rle_ED

#include "point.hpp"
#include "rle.hpp"
#include "avl_tree.hpp"

typedef std::vector<rle::RLE_run> rle_string;

float get_val_at_coord(float coord, Point p1, Point p2);

// this function expects two trees that describe intervals [X_l, X_m] and [X_m, X_r]
// it will return a new balanced tree that is the joining of the two trees
BST join(BST t1, BST t2);

// Returns the node whose segment containes the coordinate x
// Throws if x is not in the interval described by the tree with root root 
TreeNode* find_segm_containing(TreeNode* root, float x);

// preforms inorder traversal of tree with root node and finds the leftmost segment s in the
// tree such that s.right.y <= T(s.right.x), where T(x) is the value at coordinate x in tree T
void find_leftmost_smaller(TreeNode *node, BST T, Segment &s, bool &found_segm);

// preforms reverse inorder traversal of tree with root node and finds the rightmost segment s in the
// tree such that s.left.y > T(s.left.x), where T(x) is the value at coordinate x in tree T
void find_rightmost_larger(TreeNode *node, BST T, Segment &s, bool &found_segm);

BST get_OUT_LEFT(BST LEFT, int h, int w);
BST get_OUT_TOP(BST TOP, int h, int w);
// split a tree describing interval [x_l, x_r] into two trees describing [x_l, x_m],[x_m, x_r]
std::pair<BST, BST> split(BST T, float x_m);

// Takes the minumum of the functions described by t1 and t2
// The functions should be over the same interval [x_l, x_r]
// Assumes there exists x_m in [x_l, x_r] such that:
// F1(x) > F2(x) if x < x_m and F1(x) <= F2(x) if x >= x_m
// where F1 and F2 are the functions described by t1 and t2
BST combine(BST t1, BST t2);

BST SWM(BST tree, int h);

int get_rle_edit_dist(rle_string s0, rle_string s1);

int get_naive_edit_dist(std::string &s0, std::string &s1);
#endif