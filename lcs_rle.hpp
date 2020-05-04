#ifndef lcs_rle
#define lcs_rle

#include "point.hpp"
#include "rle.hpp"
#include "avl_tree.hpp"

typedef std::vector<rle::RLE_run> rle_string;

int get_lcs_rle(rle_string s0, rle_string s1);

#endif