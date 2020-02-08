#include <iostream>
#include <fstream>
#include <cstdlib>

void gen_random(std::string *s, const int len)
{
  *s = "";
  static const char alphanum[] = "ab";
  // "0123456789"
  // "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  // "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < len; ++i)
  {
    *s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }
}

int main()
{
  std::ofstream fout("input.in");
  int nr_tests = 10000;
  fout << nr_tests << '\n';
  std::string s0, s1;
  for (int i = 0; i < nr_tests; ++i)
  {
    int N = rand() % 30 + 1;
    int M = rand() % 30 + 1;
    gen_random(&s0, N);
    gen_random(&s1, M);
    fout << N << ' ' << M << '\n'
         << s0 << '\n'
         << s1 << '\n';
  }
  return 0;
}