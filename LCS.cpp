#include <iostream>
#include <fstream>

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

int main(){
    ifstream fin("input.in");
    ofstream fout("output.out");

    int N, M;
    string s0 = "0", s1 = "0";
    fin >> N >> M;
    read_string(fin, N, s0);
    read_string(fin, M, s1);

    int sol = get_LCS(N, M, s0, s1);

    cout << sol <<'\n';
    return 0;
}