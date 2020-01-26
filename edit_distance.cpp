#include <iostream>
#include "edit_distance.hpp"

int min(int a, int b, int c)
{
  return std::min(a, std::min(b, c));
}

int get_edit_dist(const int M, const int N, const std::string &s0, const std::string &s1)
{
  int dyn[M + 1][N + 1];
  
  for(int i = 0; i <= M; i++){
    dyn[i][0] = i;
  }

  for(int i = 0; i <= N; i++){
    dyn[0][i] = i;
  }

  for (int i = 1; i <= M; i++)
  {
    for (int j = 1; j <= N; j++)
    {
      dyn[i][j] = min(dyn[i - 1][j - 1] + (s0[i] != s1[j]), dyn[i - 1][j] + 1, dyn[i][j - 1] + 1);
    }
  }

  return dyn[M][N];
}