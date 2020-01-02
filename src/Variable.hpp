#pragma once

struct Variable
{
  unsigned int address;
  unsigned int start;
  unsigned int end;
  unsigned int dependency;

  Variable(unsigned int address);
  Variable(unsigned int address, unsigned int start, unsigned int end);
  Variable(unsigned int address, unsigned int start, unsigned int end, unsigned int dependency);
};
