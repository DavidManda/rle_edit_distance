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

    int sol = edit_distance::get_edit_dist(M, N, s0, s1);
    std::cout << "Edit distance for strings " + s0 + " and " + s1 + " is: " << sol << '\n';
  }
  return 0;
}