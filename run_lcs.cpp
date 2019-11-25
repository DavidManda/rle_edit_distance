#include <fstream>
#include "LCS.hpp"
#include "rle_string.h"

int main(){
    ifstream fin("input.in");
    ofstream fout("output.out");

    int T, N, M;
    fin >> T;
    int nr_fail = 0;
    for(int i = 0; i < T; i++){
        string s0 = "0", s1 = "0";
        fin >> N >> M;
        LCS::read_string(fin, N, s0);
        LCS::read_string(fin, M, s1);

        int sol = get_LCS(N, M, s0, s1);

        RLE_string_helper helper;
        vector<RLE_run> rle_s0 = helper.get_rle_string(s0);
        vector<RLE_run> rle_s1 = helper.get_rle_string(s1);
        int rle_sol = get_rle_lcs(rle_s0, rle_s1);
        if(rle_sol != sol){
            nr_fail ++;
            cout << "Failed test " << i << '\n';
            cout << "Expected " << sol << " but received " << rle_sol << '\n';
            cout << "Strings are:\n" << s0.substr(1, s0.length()) << "\n" << s1.substr(1,s1.length()) << '\n';
        }
    }
    cout << "Passed " << T - nr_fail << "/" << T <<" tests!\n";
    return 0;
}