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
  this->initialized = true;
  this->is_array = true;
}

Variable::Variable(unsigned int address, unsigned int start, unsigned int end, unsigned int dependency)
{
  this->address = address;
  this->start = start;
  this->end = end;
  this->dependency = dependency;
  this->initialized = true;
  this->is_array = true;
}

bool Variable::is_constant()
{
  return false;
}

bool Variable::is_dependent()
{
  return dependency != 0;
}