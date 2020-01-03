#include "Memory.hpp"

#include<iostream>

void Memory::reserve_variable(string pid)
{
  if (this->variables.find(pid) != this->variables.end())
  {
    throw (string) pid + " is already defined";
  }
  this->variables[pid] = new Variable(++this->var_count);
}

void Memory::reserve_array(string pid, unsigned int start, unsigned int end)
{
  if (this->variables.find(pid) != this->variables.end())
  {
    throw (string) pid + " is already defined";
  }
  if (start >= end)
  {
    throw (string) "Wrong range of array " + pid;
  }
  this->variables[pid] = new Variable(this->var_count+1, start, end);
  auto length = end - start + 1;
  this->var_count += length;
}

Variable* Memory::get_variable(string pid)
{
  if (this->variables.find(pid) == this->variables.end())
  {
    throw (string) pid + " is not defined";
  }
  auto var = this->variables[pid];
  // if (!var->initialized)
  // {
  //   throw (string) pid + " is not initialized";
  // }
  return var;
}

Variable* Memory::get_variable(string pid, unsigned int index)
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

Variable* Memory::get_variable(string pid, string index)
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

string Memory::push_to_stack()
{
  return "STORE " + to_string(++this->var_count);
}

string Memory::pop_from_stack()
{
  return "LOAD " + to_string(this->var_count--);
}

unsigned int Memory::get_stack_top()
{
  return this->var_count;
}