#pragma once

#include <map>
#include <string>

#include "Variable.hpp"

using namespace std;


class Memory
{
public:
  void reserve_variable(const char* pid);
  void reserve_array(const char* pid, unsigned int start, unsigned int end);
  Variable* get_variable(const char* pid);
  Variable* get_variable(const char* pid, unsigned int index);
  Variable* get_variable(const char* pid, const char* index);
private:
  unsigned int var_count = 0;
  map<string, Variable*> variables; 
};