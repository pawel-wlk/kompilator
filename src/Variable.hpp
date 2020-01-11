#pragma once

#include<string>

#include "Value.hpp"

using namespace std;

struct Variable : public Value
{
  unsigned int address;
  unsigned int start;
  unsigned int end;
  unsigned int dependency;
  bool initialized;
  bool is_array;
  bool is_iterator;
  string name;

  Variable(string name, unsigned int address);
  Variable(string name, unsigned int address, unsigned int start, unsigned int end);
  Variable(string name, unsigned int address, unsigned int start, unsigned int end, unsigned int dependency);

  bool is_constant();
  bool is_dependent();
};
