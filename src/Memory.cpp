#include "Memory.hpp"

void Memory::reserve_variable(const char* pid)
{
  this->variables[pid] = new Variable(++this->var_count);
}

void Memory::reserve_array(const char* pid, unsigned int start, unsigned int end)
{
  this->variables[pid] = new Variable(this->var_count+1, start, end);
  auto length = end - start + 1;
  this->var_count += length;
}

Variable* Memory::get_variable(const char* pid)
{
  if (this->variables.find(pid) == this->variables.end())
  {
    throw (string) pid + " is not defined";
  }
  return this->variables[pid];
}

Variable* Memory::get_variable(const char* pid, unsigned int index)
{
  if (this->variables.find(pid) == this->variables.end())
  {
    throw (string) pid + " is not defined";
  }
  auto var = this->variables[pid];

  if (var->start == var->end)
  {
    throw (string) pid + " is not an array";
  }
  auto address = var->address + index - var->start;

  return new Variable(address);
}

Variable* Memory::get_variable(const char* pid, const char* index)
{
  if (this->variables.find(pid) == this->variables.end())
  {
    throw (string) pid + " is not defined";
  }
  auto var = this->variables[pid];

  if (var->start == var->end)
  {
    throw (string) pid + " is not an array";
  }
  auto dependency = this->variables[index];

  return new Variable(var->address, var->start, var->end, dependency->address);
}