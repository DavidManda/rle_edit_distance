#include<fstream>
#include<iostream>
#include <algorithm>

using namespace std;

void read_string(istream& fin, int len, string& s){
    char ch;
    for(int i = 0; i < len; i++){
        fin >> ch;
        s += ch;
    }
}

int min(int a, int b, int c){
    return min(min(a,b),c);
}

int get_edit_dist(const int N, const int M, const string& s0, const string& s1){
    int dyn[N + 1][M + 1];
    for(int i = 0; i <= N; i++){
        dyn[i][0] = i;
    }
    for(int i = 0; i <= M; i++){
        dyn[0][i] = i;
    }
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= M; j++){
            dyn[i][j] = min(dyn[i-1][j] + 1, dyn[i][j-1] + 1, dyn[i-1][j-1] + (s0[i-1] != s1[j-1]));
        }
    }
    return dyn[N][M];
}

int main() {
    ifstream fin("input");
    ofstream fout("output");
    int N, M;
    string s0, s1;
    fin >> N >> M;
    read_string(fin, N, s0);
    read_string(fin, M, s1);
    int sol = get_edit_dist(N, M, s0, s1);
    fout << sol << '\n';
}

