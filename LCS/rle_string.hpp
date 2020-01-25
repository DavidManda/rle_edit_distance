#ifndef rle_string
#define rle_string

#include <iostream>
#include <vector>
#include <sstream>

class RLE_run{
public:
    char ch;
    int len;
    RLE_run(char _ch, int _len);
    std::string print();
};

class RLE_string_helper{
public:
    std::vector<RLE_run> get_rle_string(std::string str);

    std::string print(std::vector<RLE_run> rle_string);
};
#endif