#include "Memory.hpp"

#include<sstream>
#include<iostream>

void Memory::reserve_constants()
{
  this->variables["1"] = new Variable("1", ++this->var_count);
  this->variables["-1"] = new Variable("-1", ++this->var_count);
}

void Memory::reserve_variable(string pid)
{
  if (this->variables.find(pid) != this->variables.end())
  {
    throw (string) pid + " is already defined";
  }
  this->variables[pid] = new Variable(pid, ++this->var_count);
}

void Memory::reserve_array(string pid, int start, int end)
{
  if (this->variables.find(pid) != this->variables.end())
  {
    throw (string) pid + " is already defined";
  }
  if (start > end)
  {
    throw (string) "Wrong range of array " + pid + " " + to_string(start) + " to " + to_string(end);
  }
  this->variables[pid] = new Variable(pid, this->var_count+1, start, end);
  auto length = end - start + 1;
  this->var_count += length;
}

Variable* Memory::reserve_iterator(string pid)
{
  if (this->variables.find(pid) != this->variables.end())
  {
    throw (string) pid + " is already defined";
  }

  this->variables[pid] = new Variable(pid, ++this->var_count);
  this->variables[pid]->is_iterator = true;
  this->variables[pid]->is_initialized = true;

  return this->variables[pid];
}

void Memory::destroy_iterator(string pid)
{
  variables.erase(pid);
  var_count--;
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

Variable* Memory::get_variable(string pid, int index)
{
  if (this->variables.find(pid) == this->variables.end())
  {
    throw (string) pid + " is not defined";
  }
  auto var = this->variables[pid];

  if (!var->is_array)
  {
    throw (string) pid + " is not an array";
  }
  auto address = var->address + index - var->start;

  stringstream name;
  name << pid <<"(" << index << ")";

  auto result = new Variable(name.str(), address);
  result->is_initialized = true;
  result->dependency = 0;
  return result;
}

Variable* Memory::get_variable(string pid, string index)
{
  if (this->variables.find(pid) == this->variables.end())
  {
    throw (string) pid + " is not defined";
  }
  auto var = this->variables[pid];

  if (!var->is_array)
  {
    throw (string) pid + " is not an array";
  }
  if (this->variables.find(index) == this->variables.end())
  {
    throw (string) index + " is not defined";
  }
  auto dependency = this->variables[index];

  if (!dependency->is_initialized)
  {
    throw (string) (index + " is not initialized");
  }


  auto new_var = new Variable(pid, var->address, var->start, var->end, dependency->address);
  new_var->is_array = false;

  return new_var;

}

unsigned int Memory::push_to_stack()
{
  return ++this->var_count;
}

unsigned int Memory::pop_from_stack()
{
  return this->var_count--;
}

unsigned int Memory::get_stack_top()
{
  return this->var_count;
}