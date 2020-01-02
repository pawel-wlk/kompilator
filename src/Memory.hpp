#pragma once

#include <map>
#include <string>

#include "Variable.hpp"

using namespace std;


class Memory
{
public:
  void reserve_variable(string pid);
  void reserve_array(string pid, unsigned int start, unsigned int end);
  Variable* get_variable(string pid);
  Variable* get_variable(string pid, unsigned int index);
  Variable* get_variable(string pid, string index);
private:
  unsigned int var_count = 0;
  map<string, Variable*> variables; 
};