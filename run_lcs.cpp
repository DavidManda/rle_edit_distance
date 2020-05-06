#include <fstream>
#include <iostream>
#include <ctime>
#include "lcs_rle.hpp"
#include "helper.hpp"

int main()
{
  std::ifstream fin("input.in");
  std::ofstream fout("output.out");

  int T, N, M;
  fin >> T;
  int nr_fail = 0;
  for(int test = 0; test < T; test++){
    // Initialise strings with buffer character at the beggining so they start at index 1
    std::string s0, s1;
    fin >> M >> N;
    helper::read_string(fin, M, s0);
    helper::read_string(fin, N, s1);
    RLE_string_helper rle_helper;
    std::vector<RLE_run> rle_s0 = rle_helper.get_rle_string(s0);
    std::vector<RLE_run> rle_s1 = rle_helper.get_rle_string(s1);
    std::cout<<"Uncompressed lengths are: "<<s0.size()<<" and "<<s1.size()<<'\n';
    std::cout<<"Compressed lengths are: "<<rle_s0.size()<<" and "<<rle_s1.size()<<'\n';
    std::clock_t start;
    double naive_time, rle_time;
    start = std::clock();
    int sol = lcs_rle::get_lcs(s0, s1);
    naive_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Naive time is: "<<naive_time<<'\n';
    start = std::clock();
    int sol_rle = lcs_rle::get_lcs_rle(rle_s0, rle_s1);
    rle_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"RLE time is: "<<rle_time<<'\n';
    if(sol != sol_rle)
    {
      std::cout<<"Test " << test << "/"<<T<<" failed for strings " + s0 + " and " + s1 + " with sizes of "<< M<<" and "<<N<<"\n";
      std::cout << "LCS is: " << sol << '\n';
      std::cout << "RLE LCS for strings is: " << sol_rle << '\n';
      return 0;
    }
  }
  std::cout<<"All "<<T<<" tests passed!\n";
  return 0;
}