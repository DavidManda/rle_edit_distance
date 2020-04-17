#ifndef edit_distance
#define edit_distance

#include <string.h>
#include <iostream>
#include "rle.hpp"
#include "point.hpp"

typedef std::vector< std::vector< std::vector< Point > > > border_type;
typedef std::vector<rle::RLE_run> rle_string;

// Returns the edit distance for strings s0 and s1
// Expects the strings to start at index 1
int get_edit_dist(const int M, const int N, const std::string& s0, const std::string& s1);

// Returns the edit distance for 2 run-lenght encoded strings s0 and s1
// Expects strings to be indexed at 1
// The complexity is O(m * n^2), where m and n represent the number of runs in each string
int get_rle_edit_dist(const rle_string &s0, const rle_string &s1);

#endif