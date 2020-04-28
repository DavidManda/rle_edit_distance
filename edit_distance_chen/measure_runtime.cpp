#include <iostream>
#include <ctime>
#include <fstream>
#include <math.h> 
#include "edit_distance.hpp"
#include "helper.hpp"
#include <sstream>

int main(){
  std::ofstream fout("run_times.out");
  fout<<"M, N, compression factor 1, compression factor 2, naive time, rle time\n";
  for(int i = 1; i < 50; i+=1){
    // height is i and width is i*2
    std::ifstream fin("../inputs/input" + std::to_string(i) + "x" + std::to_string(i*2) + ".in");
    int M, N;
    fin >> M >> N;
    std::string s0 = "*",s1 = "*";
    helper::read_string(fin, M, s0);
    helper::read_string(fin, N, s1);
    RLE_string_helper rle_helper;
    std::vector<RLE_run> rle_s0 = rle_helper.get_rle_string(s0);
    std::vector<RLE_run> rle_s1 = rle_helper.get_rle_string(s1);
    double compression_fact0 = (double)s0.size()/(double)rle_s0.size();
    double compression_fact1 = (double)s1.size()/(double)rle_s1.size();
    std::clock_t start;
    double naive_time=0, rle_time=0;
    int sol = 0, sol_rle = 0;
    std::cout<<i<<'\n';
    start = std::clock();
    sol = edit_distance::get_edit_dist(M, N, s0, s1);
    naive_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    start = std::clock();
    sol_rle = edit_distance::get_rle_edit_dist(rle_s0, rle_s1);
    rle_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    if (sol_rle != sol)
    {
      std::cout << "Failed test " << i << '\n';
      std::cout << "Expected " << sol << " but received " << sol_rle << '\n';
      std::cout << "Strings are:\n"
                << s0.substr(1, s0.length()) << "\n"
                << s1.substr(1, s1.length()) << '\n';
      return 0;
    }
    std::cout<<"Solution is:"<<sol_rle<<'\n';
    std::cout<<"Naive time is: "<<naive_time<<'\n';
    std::cout<<"RLE time is: "<<rle_time<<'\n';
    fout<<M<<','<<N<<','<<compression_fact0<<','<<compression_fact1<<','<<naive_time<<','<<rle_time<<'\n';
  }
}