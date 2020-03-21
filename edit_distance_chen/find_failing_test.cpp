#include <fstream>
#include "edit_distance.hpp"
#include "helper.hpp"

bool check_strings(std::string s0, std::string s1)
{
  int M = s0.size(), N = s1.size();
  RLE_string_helper rle_helper;
  std::vector<RLE_run> rle_s0 = rle_helper.get_rle_string(s0);
  std::vector<RLE_run> rle_s1 = rle_helper.get_rle_string(s1);
  int sol_rle = edit_distance::get_rle_edit_dist(rle_s0, rle_s1);
  int sol = edit_distance::get_edit_dist(M, N, s0, s1);
  return sol == sol_rle;
}

int main()
{
  int T, N, M;
  for(int gen0 = (1<<15); (gen0 >> 25) < 1; gen0++){
    for(int gen1 = (1<<15); (gen1 >> 25) < 1; gen1++){
      T++;
      std::string s0 = "*", s1 = "*";
      for(int i = 0; (1<<i) <= gen0; i++){
        s1 += ('a' + ((gen0 >> i) & 1));
      }
      for(int i = 0; (1<<i) <= gen1; i++){
        s0 += ('a' + ((gen1 >> i) & 1));
      }
      std::cout<<s0<<' '<<s1<<' '<<T<<"\n";
      if(!check_strings(s0,s1)){
        std::cout<<"Test "<<T<<"failed!\n";
        std::cout<<"Failed on strings:\n"<<s0<<"\n"<<s1<<"\n";
        return 0;
      }
    }
  }
}