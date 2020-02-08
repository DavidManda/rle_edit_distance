#include <fstream>
#include "edit_distance.hpp"
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
    std::string s0 = "*", s1 = "*";
    fin >> M >> N;
    helper::read_string(fin, M, s0);
    helper::read_string(fin, N, s1);
    RLE_string_helper rle_helper;
    std::vector<RLE_run> rle_s0 = rle_helper.get_rle_string(s0);
    std::vector<RLE_run> rle_s1 = rle_helper.get_rle_string(s1);
    int sol_rle = edit_distance::get_rle_edit_dist(rle_s0, rle_s1);
    int sol = edit_distance::get_edit_dist(M, N, s0, s1);
    
    if(sol != sol_rle)
    {
      std::cout<<"Test " << test << "/"<<T<<" failed for strings " + s0 + " and " + s1 + "\n";
      std::cout << "Edit distance is: " << sol << '\n';
      std::cout << "RLE edit distance for strings is: " << sol_rle << '\n';
      break;
    }
  }
  return 0;
}