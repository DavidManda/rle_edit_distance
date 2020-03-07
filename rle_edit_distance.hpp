#pragma once

#include "point.hpp"
#include "avl_tree.hpp"

float get_val_at_coord(float coord, Point p1, Point p2);

// this function expects two trees that describe intervals [X_l, X_m] and [X_m, X_r]
// it will return a new balanced tree that is the joining of the two trees
BST join(BST t1, BST t2);

// Returns the node whose segment containes the coordinate x
// Throws if x is not in the interval described by the tree with root root 
TreeNode* find_segm_containing(TreeNode* root, float x);

// split a tree describing interval [x_l, x_r] into two trees describing [x_l, x_m],[x_m, x_r]
std::pair<BST, BST> split(BST T, float x_m);