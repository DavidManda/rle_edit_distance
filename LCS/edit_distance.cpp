#include <fstream>
#include <iostream>
#include <algorithm>
#include "rle_string.hpp"

void read_string(std::istream &fin, int len, std::string &s)
{
  char ch;
  for (int i = 0; i < len; i++)
  {
    fin >> ch;
    s += ch;
  }
}

int min(int a, int b, int c)
{
  return std::min(std::min(a, b), c);
}

int get_edit_dist(const int N, const int M, const std::string &s0, const std::string &s1)
{
  int dyn[N + 1][M + 1];

  for (int i = 0; i <= N; i++)
  {
    dyn[i][0] = i;
  }

  for (int i = 0; i <= M; i++)
  {
    dyn[0][i] = i;
  }

  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= M; j++)
    {
      dyn[i][j] = min(dyn[i - 1][j] + 1, dyn[i][j - 1] + 1, dyn[i - 1][j - 1] + (s0[i] != s1[j]));
    }
  }
  return dyn[N][M];
}

int main()
{
  std::ifstream fin("input.in");
  std::ofstream fout("output.out");
  int N, M;
  // The first element in the strings is a buffer element for use of implementation
  std::string s0 = "0", s1 = "0";
  fin >> N >> M;
  read_string(fin, N, s0);
  read_string(fin, M, s1);
  int sol = get_edit_dist(N, M, s0, s1);
  fout << sol << '\n';

  RLE_run my_run('a', 10);
  std::cout << my_run.print() << '\n';
  RLE_string_helper helper;
  std::vector<RLE_run> rle_str = helper.get_rle_string("aaabbaatttttt");
  std::cout << helper.print(rle_str) << '\n';

  return 0;
}
