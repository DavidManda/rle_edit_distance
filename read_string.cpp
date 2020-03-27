#include <fstream>
#include <iostream>
using namespace std;

int main(){
  ifstream fin("input.in");
  string str;
  int len=0;
  while(getline(fin, str)){
    len+=str.length();
  }
  cout<<len<<'\n';
}