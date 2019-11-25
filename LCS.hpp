#ifndef LCS
#define LCS

#include "rle_string.h"
#include <string.h>
#include <map>

void precompute(vector< map<char, int> >& vec, vector<RLE_run> s);

void read_string(istream& fin, int len, string& s);

int get_LCS(const int N, const int M, const string& s0, const string& s1);

int get_rle_lcs(const vector<RLE_run> s0, const vector<RLE_run> s1);

#endif