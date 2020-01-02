#include "Variable.hpp"

Variable::Variable(unsigned int address)
{
  this->address = address;
}

Variable::Variable(unsigned int address, unsigned int start, unsigned int end)
{
  this->address = address;
  this->start = start;
  this->end = end;
}

Variable::Variable(unsigned int address, unsigned int start, unsigned int end, unsigned int dependency)
{
  this->address = address;
  this->start = start;
  this->end = end;
  this->dependency = dependency;
}
