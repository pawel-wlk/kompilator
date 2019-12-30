#pragma once

#include<string>

using namespace std;

struct Variable
{
  unsigned int address;
  unsigned int start;
  unsigned int end;
  bool is_array;
  unsigned int dependency;

  string assign();
  string load();
};
