#include "Code.hpp"

Code::Code(Memory* memory)
{
  this->memory = memory;
  this->code = "";
}

string Code::get_code()
{
  return this->code + "HALT";
}

void Code::read(Variable* var)
{
  this->code += this->memory->push_to_stack();
  this->code += " GET " + var->store() + " " ;
  this->code += this->memory->pop_from_stack() + "\n";
}

void Code::write(Value* val)
{
  this->code += this->memory->push_to_stack();
  this->code += " " + val->construct() + " PUT ";
  this->code += this->memory->pop_from_stack() + "\n";
}

void Code::assign(Variable* var)
{
  code += var->store() + " ";
  code += memory->pop_from_stack() + "\n";
}

void Code::construct_val(Value* val)
{
  this->code += memory->push_to_stack();
  this->code += val->construct();
}

void Code::add(Value* a, Value* b)
{
  if (a->is_constant() && b->is_constant())
  {
    auto a_val = ((Constant*) a)->value;
    auto b_val = ((Constant*) b)->value;
    Constant constant(a_val+b_val);
    construct_val(&constant);
    return;
  }

  code += b->construct() + " ";
  code += memory->push_to_stack() + " ";
  code += a->construct() + " ";
  auto b_addr = memory->get_stack_top();
  code += "ADD " + to_string(b_addr) + "\n";
  memory->pop_from_stack();
}

void Code::subtract(Value* a, Value* b)
{
  if (a->is_constant() && b->is_constant())
  {
    auto a_val = ((Constant*) a)->value;
    auto b_val = ((Constant*) b)->value;
    Constant constant(a_val-b_val);
    construct_val(&constant);
    return;
  }

  code += b->construct() + " ";
  code += memory->push_to_stack() + " ";
  code += a->construct() + " ";
  auto b_addr = memory->get_stack_top();
  code += "SUB " + to_string(b_addr) + "\n";
  memory->pop_from_stack();
}

void Code::multiply(Value* a, Value* b)
{
  if (a->is_constant() && b->is_constant())
  {
    auto a_val = ((Constant*) a)->value;
    auto b_val = ((Constant*) b)->value;
    Constant constant(a_val*b_val);
    construct_val(&constant);
    return;
  }

  // TODO figure out multiplication
}

void Code::divide(Value* a, Value* b)
{
  if (b->is_constant() && ((Constant*) b)->value == 0)
  {
    throw (string) "Division by 0";
  }

  if (a->is_constant() && b->is_constant())
  {
    auto a_val = ((Constant*) a)->value;
    auto b_val = ((Constant*) b)->value;
    Constant constant(a_val/b_val);
    construct_val(&constant);
    return;
  }

  // TODO figure out division
}

void Code::modulo(Value* a, Value* b)
{
  if (b->is_constant() && ((Constant*) b)->value == 0)
  {
    throw (string) "Modulo by 0";
  }

  if (a->is_constant() && b->is_constant())
  {
    auto a_val = ((Constant*) a)->value;
    auto b_val = ((Constant*) b)->value;
    auto b_sign = (b_val > 0) - (b_val < 0);
    Constant constant((a_val%b_val) * b_sign);
    construct_val(&constant);
    return;
  }

  // TODO figure out modulo
}