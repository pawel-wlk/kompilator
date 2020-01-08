#pragma once

#include <vector>
#include <iostream>

#include "Constant.hpp"
#include "Variable.hpp"
#include "Value.hpp"
#include "Memory.hpp"
#include "Operation.hpp"

using namespace std;

class Code
{
private:
  Memory* memory;
  vector<Operation> operations;

  void flip_sign(unsigned int addr);
public:
  Code(Memory* memory);
  friend ostream& operator<<(ostream& os, Code& c);

  void store(Variable* var);
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