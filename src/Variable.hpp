#pragma once

#include "Value.hpp"

struct Variable : public Value
{
  unsigned int address;
  unsigned int start;
  unsigned int end;
  unsigned int dependency;
  bool initialized;

  Variable(unsigned int address);
  Variable(unsigned int address, unsigned int start, unsigned int end);
  Variable(unsigned int address, unsigned int start, unsigned int end, unsigned int dependency);

  string construct();
  string store();
};
