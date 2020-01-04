#pragma once

#include <string>

#include "Constant.hpp"
#include "Variable.hpp"
#include "Value.hpp"
#include "Memory.hpp"

using namespace std;

class Code
{
private:
  Memory* memory;
  string code;
  unsigned int counter;
public:
  Code(Memory* memory);
  string get_code();
  void read(Variable* var);
  void write(Value* val);
  void assign(Variable* var);
  void construct_val(Value* val);
  void add(Value* a, Value* b);
  void subtract(Value* a, Value* b);
  void multiply(Value* a, Value* b);
  void divide(Value* a, Value* b);
  void modulo(Value* a, Value* b);
};
