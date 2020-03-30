#include <iostream>
#include <ctime>
#include <fstream>
#include <math.h> 
#include "rle_edit_distance.hpp"
#include "helper.hpp"

#define MAX_SIZE 20000

std::vector< std::vector<int> > dyn(MAX_SIZE, std::vector<int>(MAX_SIZE));
std::vector< std::vector<int> > dyn_rle(MAX_SIZE, std::vector<int>(MAX_SIZE));

std::string get_str(int size, int max_run_len){
  std::string sol = "";
  for(int i = 0; i < size; i++){
    char ch = 'a' + i%2;
    int run_len = rand() % max_run_len + 1;
    for(int j = 0; j < run_len; j++)
      sol += ch;
  }
  return sol;
}

void modify_runs(std::vector<RLE_run> &s, int max_run_len){
  for(int i = 0; i < s.size(); i++){
    int run_len = rand() % max_run_len + 1;
    s[i].len = run_len;
  }
}
int main(){
  std::ofstream fout_rle("run_times_rle.out");
  std::ofstream fout_naive("run_times_naive.out");
  for(int size = 1; size <= 1000; size+=10){
    int M = size, N = size;
    std::string s0 = "*", s1 = "*";
    s1 += get_str(M, 30);
    s0 += get_str(N, 30);

    RLE_string_helper rle_helper;
    std::vector<RLE_run> rle_s0 = rle_helper.get_rle_string(s0);
    std::vector<RLE_run> rle_s1 = rle_helper.get_rle_string(s1);
    // modify_runs(rle_s0, 50);
    // modify_runs(rle_s1, 50);
    std::clock_t start;
    double naive_time, rle_time;
    start = std::clock();
    int sol = rle_ED::get_naive_edit_dist(s0, s1, dyn);
    naive_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    start = std::clock();
    int sol_rle = rle_ED::get_rle_edit_dist(rle_s0, rle_s1, dyn_rle);
    rle_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<size<<'\n';
    std::cout<<"Naive time is: "<<naive_time<<'\n';
    std::cout<<"RLE time is: "<<rle_time<<'\n';
    fout_rle<<rle_time<<',';
    fout_naive<<naive_time<<',';
    // s0 = "*"; s1 = "*";
    // s1 += get_str(M, 100);
    // s0 += get_str(N, 100);
    // rle_s0 = rle_helper.get_rle_string(s0);
    // rle_s1 = rle_helper.get_rle_string(s1);
    // start = std::clock();
    // sol_rle = rle_ED::get_rle_edit_dist(rle_s0, rle_s1, dyn_rle);
    // rle_time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    // fout_big<<rle_time<<',';
    // std::cout<<"Size: "<< size<<"\nRLE big time is: "<<rle_time<<'\n';
  }
  fout_naive<<'\n';
  fout_rle<<'\n';
}