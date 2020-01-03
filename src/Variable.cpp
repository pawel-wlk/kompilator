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

string Variable::construct()
{
  if (dependency == 0)
  {
    return "LOAD " + to_string(address);
  }

  string result = "LOAD " + to_string(dependency);

  for (int i=0; i<start; i++)
    result += " DEC";
  
  result += " LOADI 0";

  return result;
}

string Variable::store()
{
  if (dependency == 0)
  {
    return "STORE " + to_string(address);
  }

  string result = "LOAD " + to_string(dependency);

  for (int i=0; i<start; i++)
    result += " DEC";
  
  result += " STOREI 0";

  return result;
}
