#ifndef edit_distance
#define edit_distance

#include <string.h>
#include <iostream>

// Returns the edit distance for strings s0 and s1
// Expects the strings to start at index 1
int get_edit_dist(const int M, const int N, const std::string& s0, const std::string& s1);

#endif