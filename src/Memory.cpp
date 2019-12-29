#include "Memory.hpp"

void Memory::reserve_variable(const char* pid)
{
  this->variables[pid] = {++this->var_count, 0, 0, false, 0};
}

void Memory::reserve_array(const char* pid, unsigned int start, unsigned int end)
{
  auto length = end - start + 1;
  this->variables[pid] = {this->var_count+1, start, end, true, 0};

  this->var_count += length;

}

Variable Memory::get_variable(const char* pid)
{
  return this->variables[pid];
}

Variable Memory::get_variable(const char* pid, unsigned int index)
{
  auto var = this->variables[pid];

  // error handling - later
  if (!var.is_array) return {0, 0, 0, false, 0};

  if (index < var.start || index > var.end) return {0, 0, 0, false, 0};

  return {var.address+index-var.start, 0, 0, false, 0};
}

Variable Memory::get_variable(const char* pid, const char* index)
{
  auto var = this->variables[pid];

  // error handling - later
  if (!var.is_array) return {0, 0, 0, false, 0};

  auto dependency_addr = this->get_variable(index).address;

  return {var.address, var.start, var.end, false, dependency_addr};
}