#include <fstream>
#include <iostream>
#include <ctime>
#include "rle_edit_distance.hpp"
#include "helper.hpp"

bool test_sol(std::vector< std::vector<int> > &dyn, std::vector< std::vector<int> > &dyn_rle, rle_string s0, rle_string s1){
  int M = s0.size(), N = s1.size();
  int uncompressed_i = 0, uncompressed_j = 0;
  for(int i = 1; i < M; i++){
    uncompressed_j = 0;
    uncompressed_i += s0[i].len;
    for(int j = 1; j < N; j++){
      uncompressed_j += s1[j].len;
      if(dyn_rle[i][j] != dyn[uncompressed_i][uncompressed_j]){
        std::cout<<i<<' '<<j<<'\n';
        return false;
      }
    }
  }
  return true;
}

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
    int sol = rle_ED::get_naive_edit_dist(s0, s1);
    naive_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Naive time is: "<<naive_time<<'\n';
    start = std::clock();
    int sol_rle = rle_ED::get_rle_edit_dist(rle_s0, rle_s1);
    rle_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"RLE time is: "<<rle_time<<'\n';
    // if(!test_sol(dyn, dyn_rle, rle_s0, rle_s1))
    if(sol != sol_rle)
    {
      std::cout<<"Test " << test << "/"<<T<<" failed for strings " + s0 + " and " + s1 + " with sizes of "<< M<<" and "<<N<<"\n";
      std::cout << "Edit distance is: " << sol << '\n';
      std::cout << "RLE edit distance for strings is: " << sol_rle << '\n';
      return 0;
    }
  }
  std::cout<<"All "<<T<<" tests passed!\n";
  return 0;
}