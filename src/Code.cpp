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