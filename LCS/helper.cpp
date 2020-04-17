#include "helper.hpp"

void read_string(std::istream &fin, int len, std::string &s)
{
  char ch;
  for (int i = 0; i < len; i++)
  {
    fin >> ch;
    s += ch;
  }
}