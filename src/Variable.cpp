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
}

Variable::Variable(unsigned int address, unsigned int start, unsigned int end, unsigned int dependency)
{
  this->address = address;
  this->start = start;
  this->end = end;
  this->dependency = dependency;
  this->initialized = true;
}

bool Variable::is_constant()
{
  return false;
}

string Variable::construct(unsigned int* counter)
{
  if (dependency == 0)
  {
    *counter++;
    return "LOAD " + to_string(address);
  }

  string result = "LOAD " + to_string(dependency);

  for (int i=0; i<start-1; i++)
    result += " DEC";
  
  result += " LOADI 0";

  *counter += start + 1;

  return result;
}

string Variable::store(unsigned int* counter)
{
  if (dependency == 0)
  {
    *counter++;
    return "STORE " + to_string(address);
  }

  // TODO add memory reference so we can push current value of 0th cell
  string result = "LOAD " + to_string(dependency);

  for (int i=0; i<start-1; i++)
    result += " DEC";
  
  result += " STOREI 0";

  *counter += start + 1;

  return result;
}
