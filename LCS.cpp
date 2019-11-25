#include <iostream>
#include "LCS.hpp"

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
    int v_offset = 0, h_offset = 0;

    if(s0[i].len == s1[j].len)
        return;

    while(i < s0.size() && j < s1.size()){
        // proceed vertically
        if(s0[i].len - v_offset < s1[j].len - h_offset){
            // exited through bottom
            h_offset = s0[i].len - v_offset;
            v_offset = 0;
            int next_i = i + 1;
            while(next_i < s0.size() && s0[next_i].ch != s1[j].ch){
                ++next_i;
            }
            // reached end of s0
            if(next_i >= s0.size()){
                return;
            }
            int d = min(s0[next_i].len - v_offset, s1[j].len - h_offset);
            dyn[next_i][j] = max(dyn[next_i][j], dyn[i][j] + d);
            i = next_i;
        }
        // proceed horizontally
        else if(s0[i].len - v_offset > s1[j].len - h_offset){
            // exited through right-hand side
            h_offset = 0;
            v_offset = s1[j].len - h_offset;
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
        }
        else{
            // exiting though corner
            return;
        }
    } 
    return;
}

void print_2d_vect(vector< vector<int> > vect){
    for(int i = 1; i < vect.size(); i++){
        for(int j = 1; j < vect[i].size(); j++){
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
            int curr_val = dyn[i][j];
            // start forced path at i j
            dyn[i][j] = dyn[i-1][j-1] + d;
            forced_path_update(s0, s1, i, j, dyn);

            dyn[i][j] = max(curr_val, dyn[i][j], max(dyn[i-1][j], dyn[i][j-1]));
            // print_2d_vect(dyn);
        }
    }

    return dyn[N-1][M-1];
}

void precompute(vector< map<char, int> >& vec, vector<RLE_run> s){
    map<char, int> last_position;
    char ch;
    int len, prev_val;
    for(int i = 1; i < s.size(); i++){
        ch = s[i].ch;
        len = s[i].len;
        if(last_position.find(ch) == last_position.end()){
            prev_val = 0;
        }
        else{
            prev_val = vec[last_position[ch]][ch];
        }
        vec[i-1][ch] = prev_val;
        vec[i][ch] = prev_val + len;
        last_position[ch] = i;
    }
}

int get_rle_lcs_fast(const vector<RLE_run> s0, const vector<RLE_run> s1){
    const int M = s0.size();
    const int N = s1.size();
    vector< map<char, int> > TOP(N);
    vector< map<char, int> > LEFT(M);
    precompute(TOP, s1);
    precompute(LEFT, s0);
    return 0;
}

