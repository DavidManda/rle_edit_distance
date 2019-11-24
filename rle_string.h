#ifndef rle_string
#define rle_string

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class RLE_run{
public:
    char ch;
    int len;
    RLE_run(char _ch, int _len);
    string print();
};

class RLE_string_helper{
public:
    vector<RLE_run> get_rle_string(string str);

    string print(vector<RLE_run> rle_string);
};
#endif