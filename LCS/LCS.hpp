#ifndef LCS
#define LCS

#include "rle_string.hpp"
#include <string.h>
#include <map>

void precompute(std::vector<std::map<char, int>> &vec, std::vector<RLE_run> s);

void read_string(std::istream &fin, int len, std::string &s);

// Retruns the longest common subsequence for strings s0 and s1
// Expects the strings to start at index 1
int get_LCS(const int N, const int M, const std::string &s0, const std::string &s1);

int get_rle_lcs(const std::vector<RLE_run> s0, const std::vector<RLE_run> s1);

int get_rle_lcs_fast(const std::vector<RLE_run> s0, const std::vector<RLE_run> s1);
#endif