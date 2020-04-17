#include <iostream>
#include <ctime>
#include <fstream>
#include <math.h> 
#include "LCS.hpp"
#include "helper.hpp"
#include "rle_string.hpp"

int main(){
  std::ofstream fout("run_times.out");
  fout<<"M, N, compression factor 1, compression factor 2, naive time, rle time\n";
  for(int i = 1; i <= 40; i++){
    // height is i and width is i*2
    std::ifstream fin("../inputs/input"+std::to_string(i)+"x"+std::to_string(i*2) + ".in");
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
    start = std::clock();
    sol = LCS::get_LCS(s0, s1);
    naive_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    start = std::clock();
    sol_rle = LCS::get_rle_lcs_fast(rle_s0, rle_s1);
    rle_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<i<<'\n';
    std::cout<<"Naive time is: "<<naive_time<<'\n';
    std::cout<<"RLE time is: "<<rle_time<<'\n';
    fout<<M<<','<<N<<','<<compression_fact0<<','<<compression_fact1<<','<<naive_time<<','<<rle_time<<'\n';
  }
}