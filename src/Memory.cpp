#include "Memory.hpp"

void Memory::reserve_variable(const char* pid)
{
  this->variables[pid] = new Variable;
  this->variables[pid]->address = ++this->var_count;

}

void Memory::reserve_array(const char* pid, unsigned int start, unsigned int end)
{
  auto length = end - start + 1;
  // this->variables[pid] = {this->var_count+1, start, end, true, 0};
  this->variables[pid] = new Variable;
  this->variables[pid]->address = ++this->var_count;
  this->variables[pid]->start = start;
  this->variables[pid]->end = end;
  this->variables[pid]->is_array = true;
}

Variable* Memory::get_variable(const char* pid)
{
  return this->variables[pid];
}

Variable* Memory::get_variable(const char* pid, unsigned int index)
{
  auto var = this->variables[pid];

  // error handling - later
  if (!var->is_array) return nullptr;

  if (index < var->start || index > var->end) return nullptr;

  Variable* result = new Variable;
  result->address = var->address+index-var->start;
  return result;
}

Variable* Memory::get_variable(const char* pid, const char* index)
{
  auto var = this->variables[pid];

  // error handling - later
  if (!var->is_array) return nullptr;

  auto dependency_addr = this->get_variable(index)->address;

  auto result = new Variable;
  result->address = var->address;
  result->start = var->start;
  result->end = var->end;
  result->dependency = dependency_addr;

  return result;
}