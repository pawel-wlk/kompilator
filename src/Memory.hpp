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
  Variable* reserve_iterator(string pid);
  void destroy_iterator(string pid);
  Variable* get_variable(string pid);
  Variable* get_variable(string pid, unsigned int index);
  Variable* get_variable(string pid, string index);
  unsigned int push_to_stack();
  unsigned int pop_from_stack();
  unsigned int get_stack_top();
private:
  unsigned int var_count = 0;
  map<string, Variable*> variables; 
};