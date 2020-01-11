#pragma once

#include <vector>
#include <iostream>

#include "Constant.hpp"
#include "Variable.hpp"
#include "Value.hpp"
#include "Memory.hpp"
#include "Operation.hpp"
#include "labels.hpp"

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

  ConditionLabel* equal(Value* a, Value* b);
  ConditionLabel* not_equal(Value* a, Value* b);
  ConditionLabel* less(Value* a, Value* b);
  ConditionLabel* greater(Value* a, Value* b);
  ConditionLabel* less_or_equal(Value* a, Value* b);
  ConditionLabel* greater_or_equal(Value* a, Value* b);

  void single_if(ConditionLabel* condition);
  void while_loop(ConditionLabel* condition);

  IfLabel* if_else_first(ConditionLabel* condition);
  void if_else_second(IfLabel* if_lbl);

  DoWhileLabel* do_loop_first();
  void do_loop_second(DoWhileLabel* do_lbl, ConditionLabel* condition);

  ForLabel* for_loop_first(string iterator, Value* start, Value* end, bool ascending);
  void for_loop_second(ForLabel* loop, bool ascending);
};
