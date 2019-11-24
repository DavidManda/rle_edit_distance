#include <iostream>
#include <fstream>
#include "rle_string.h"

using namespace std;

int max(int a, int b, int c){
    return max(max(a,b),c);
}

void read_string(istream& fin, int len, string& s){
    char ch;
    for(int i = 0; i < len; i++){
        fin >> ch;
        s += ch;
    }
}

int get_LCS(const int N, const int M, const string& s0, const string& s1){
    int dyn[N + 1][M + 1];
    memset(dyn, 0, sizeof(dyn));

    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= M; j++){
            dyn[i][j] = max(dyn[i-1][j-1] + (s0[i] == s1[j]), dyn[i-1][j], dyn[i][j-1]);
        }
    }

    return dyn[N][M];
}

void forced_path_update(const vector<RLE_run> s0, const vector<RLE_run> s1, int i, int j, vector< vector<int> >& dyn){
    int v_offset, h_offset;

    if(s0[i].len < s1[j].len){
        h_offset = s0[i].len;
        v_offset = 0;
    }
    else if(s0[i].len > s1[j].len){
        h_offset = 0;
        v_offset = s1[j].len;
    }
    else{
        return;
    }

    while(i < s0.size() && j < s1.size()){
        // proceed vertically
        if(s0[i].len < s1[j].len){
            
            if(v_offset != 0){
            // this means I exited through the right-hand side and now I am going down, 
            // which means I will exit through the corner
                return;
            }

            int next_i = i + 1;
            while(next_i < s0.size() && s0[next_i].ch != s1[j].ch){
                ++next_i;
            }
            // reached end of s0
            if(next_i >= s0.size()){
                return;
            }
            int d = min(s0[next_i].len, s1[j].len - h_offset);
            dyn[next_i][j] = max(dyn[next_i][j], dyn[i][j] + d);
            i = next_i;
            if(s0[next_i].len < s1[j].len - h_offset){
                // exit through the bottom
                h_offset = h_offset + d;
                v_offset = 0;
            }
            else if(s0[next_i].len > s1[j].len - h_offset){
                // exit through the right-hand side
                h_offset = 0;
                v_offset = d;
            }
            else{
                return;
            }
        }
        // proceed horizontally
        else{

            if(h_offset != 0){
                // this means I exited through the bottom side and now I am going right,
                // which means I will exit through the corner
                return; 
            }
            int next_j = j + 1;
            while(next_j < s1.size() && s0[i].ch != s1[next_j].ch){
                ++next_j;
            }
            // reached end of s1
            if(next_j >= s1.size()){
                return;
            }
            int d = min(s0[i].len - v_offset, s1[next_j].len);
            dyn[i][next_j] = max(dyn[i][next_j], dyn[i][j] + d);
            j = next_j;
            if(s0[i].len - v_offset < s1[next_j].len){
                // exit through the bottom
                h_offset = d;
                v_offset = 0;
            }
            else if(s0[i].len - v_offset > s1[next_j].len){
                // exit through the right-hand side
                h_offset = 0;
                v_offset = v_offset + d;
            }
            else{
                return;
            }
        }

    } 
    return;
}

void print_2d_vect(vector< vector<int> > vect){
    for(int i = 0; i < vect.size(); i++){
        for(int j = 0; j < vect[i].size(); j++){
            cout << vect[i][j] << ' ';
        }
        cout << '\n';
    }
    cout <<'\n';
}

int get_rle_lcs(const vector<RLE_run> s0, const vector<RLE_run> s1){
    const int N = s0.size();
    const int M = s1.size();
    vector< vector<int> > dyn(N, vector<int>(M));

    for(int i = 1; i < N; i++){
        for(int j = 1; j < M; j++){
            // miss match block
            if(s0[i].ch != s1[j].ch){
                dyn[i][j] = max(dyn[i-1][j], dyn[i][j-1]);
                continue;
            }
            // match block
            int d = min(s0[i].len, s1[j].len);
            dyn[i][j] = max(dyn[i-1][j-1] + d, dyn[i][j], max(dyn[i-1][j], dyn[i][j-1]));
            forced_path_update(s0, s1, i, j, dyn);
        }
    }

    return dyn[N-1][M-1];
}

int main(){
    ifstream fin("input.in");
    ofstream fout("output.out");

    int N, M;
    string s0 = "0", s1 = "0";
    fin >> N >> M;
    read_string(fin, N, s0);
    read_string(fin, M, s1);

    int sol = get_LCS(N, M, s0, s1);

    RLE_string_helper helper;
    vector<RLE_run> rle_s0 = helper.get_rle_string(s0);
    vector<RLE_run> rle_s1 = helper.get_rle_string(s1);
    int rle_sol = get_rle_lcs(rle_s0, rle_s1);
    cout << sol <<'\n';
    cout << rle_sol <<'\n';
    return 0;
}