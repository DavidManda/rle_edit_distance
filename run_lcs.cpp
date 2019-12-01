#include <fstream>
#include "LCS.hpp"
#include "rle_string.hpp"

int main(){
    std::ifstream fin("input.in");
    std::ofstream fout("output.out");

    int T, N, M;
    fin >> T;
    int nr_fail = 0;
    for(int i = 0; i < T; i++){
        std::string s0 = "0", s1 = "0";
        fin >> N >> M;
        LCS::read_string(fin, N, s0);
        LCS::read_string(fin, M, s1);

        int sol = get_LCS(N, M, s0, s1);

        RLE_string_helper helper;
        std::vector<RLE_run> rle_s0 = helper.get_rle_string(s0);
        std::vector<RLE_run> rle_s1 = helper.get_rle_string(s1);
        int rle_sol = get_rle_lcs(rle_s0, rle_s1);
        if(rle_sol != sol){
            nr_fail ++;
            std::cout << "Failed test " << i << '\n';
            std::cout << "Expected " << sol << " but received " << rle_sol << '\n';
            std::cout << "Strings are:\n" << s0.substr(1, s0.length()) << "\n" << s1.substr(1,s1.length()) << '\n';
        }
    }
    std::cout << "Passed " << T - nr_fail << "/" << T <<" tests!\n";
    return 0;
}