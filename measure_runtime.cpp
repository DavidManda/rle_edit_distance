#include <iostream>
#include <ctime>
#include <fstream>
#include <math.h>
#include "rle_edit_distance.hpp"
#include "helper.hpp"

std::string get_str(int size, int max_run_len)
{
  std::string sol = "";
  for (int i = 0; i < size; i++)
  {
    char ch = 'a' + i % 2;
    int run_len = rand() % max_run_len + 1;
    for (int j = 0; j < run_len; j++)
      sol += ch;
  }
  return sol;
}

void modify_runs(std::vector<RLE_run> &s, int max_run_len)
{
  for (int i = 0; i < s.size(); i++)
  {
    int run_len = rand() % max_run_len + 1;
    s[i].len = run_len;
  }
}
int main()
{
  std::ofstream fout("run_times.out");
  // std::ofstream fout_naive("run_times_naive.out");
  fout << "M, N, compression factor 1, compression factor 2, naive time, rle time\n";
  // fout_rle<<"M, N, compression factor 1, compression factor 2, time\n";
  int fixed_size = 100;
  std::string s0, s1;
  int M, N, m, n;
  for (int i = 1; i < 50; i += 1)
  {
    // height is i and width is i*2
    std::ifstream fin("inputs/input" + std::to_string(i) + "x" + std::to_string(i) + ".in");
    // std::ifstream fin("inputs/input" + std::to_string(i) + ".in");
    fin >> M >> N;
    s0 = "";s1 = "";
    helper::read_string(fin, M, s0);
    helper::read_string(fin, N, s1);
    // m = i;
    // n = i;
    // s0 = get_str(m, 20);
    // s1 = get_str(n,20);
    // M = s0.length();
    // N = s1.length();
    RLE_string_helper rle_helper;
    std::vector<RLE_run> rle_s0 = rle_helper.get_rle_string(s0);
    std::vector<RLE_run> rle_s1 = rle_helper.get_rle_string(s1);
    std::cout << "Sizes are " << M << " and " << N << '\n';
    std::cout << "Compressed sizes are " << rle_s0.size() << " and " << rle_s1.size() << '\n';
    double compression_fact0 = (double)s0.size() / (double)rle_s0.size();
    double compression_fact1 = (double)s1.size() / (double)rle_s1.size();
    std::clock_t start;
    double naive_time = 0, rle_time = 0;
    int sol = 0, sol_rle = 0;
    start = std::clock();
    sol = rle_ED::get_naive_edit_dist(s0, s1);
    naive_time = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    rle_time = 0;
    int times = 5;
    for(int j = 0; j < times; j++){
      start = std::clock();
      sol_rle = rle_ED::get_rle_edit_dist(rle_s0, rle_s1);
      rle_time += (std::clock() - start) / (double)CLOCKS_PER_SEC;
    }
    rle_time /= times;
    if (sol != sol_rle)
    {
      std::cout << "Failed!!!\n"
                << sol_rle << ' ' << sol << '\n';
      return 0;
    }
    std::cout << i << '\n';
    std::cout << "Naive time is: " << naive_time << '\n';
    std::cout << "RLE time is: " << rle_time << '\n';
    fout << M << ',' << N << ',' << compression_fact0 << ',' << compression_fact1 << ',' << naive_time << ',' << rle_time << '\n';
    // fout_naive<<M<<','<<N<<','<<compression_fact0<<','<<compression_fact1<<','<<naive_time<<'\n';
  }
}