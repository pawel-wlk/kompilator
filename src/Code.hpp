#pragma once

#include <string>

#include "Variable.hpp"
#include "Value.hpp"
#include "Memory.hpp"

using namespace std;

class Code
{
private:
  Memory* memory;
  string code;
public:
  Code(Memory* memory);
  string get_code();
  void read(Variable* var);
  void write(Value* val);
};
