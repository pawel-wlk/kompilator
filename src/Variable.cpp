#include "Variable.hpp"

Variable::Variable(string name, unsigned int address)
{
  this->address = address;
  this->is_array = false;
  this->is_initialized = false;
  this->name = name;
}

Variable::Variable(string name, unsigned int address, unsigned int start, unsigned int end)
{
  this->address = address;
  this->start = start;
  this->end = end;
  this->is_initialized = true;
  this->is_array = true;
  this->is_iterator = false;
  this->name = name;
}

Variable::Variable(string name, unsigned int address, unsigned int start, unsigned int end, unsigned int dependency)
{
  this->address = address;
  this->start = start;
  this->end = end;
  this->dependency = dependency;
  this->is_initialized = true;
  this->is_array = true;
  this->is_iterator = false;
  this->name = name;
}

bool Variable::is_constant()
{
  return false;
}

bool Variable::is_dependent()
{
  return dependency != 0;
}