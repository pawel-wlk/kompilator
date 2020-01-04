#include "Code.hpp"

Code::Code(Memory* memory)
{
  this->memory = memory;
  this->code = "";
  counter = 0;
}

string Code::get_code()
{
  return this->code + "HALT";
}

void Code::read(Variable* var)
{
  this->code += this->memory->push_to_stack();
  this->code += " GET " + var->store(&counter) + " " ;
  this->code += this->memory->pop_from_stack() + "\n";
  counter += 3;
  code += "\n# counter :" +to_string(counter) +"\n";
}

void Code::write(Value* val)
{
  this->code += this->memory->push_to_stack();
  this->code += " " + val->construct(&counter) + " PUT ";
  this->code += this->memory->pop_from_stack() + "\n";
  counter += 3;
}

void Code::assign(Variable* var)
{
  code += var->store(&counter) + " ";
  code += memory->pop_from_stack() + "\n";
  counter++;
}

void Code::construct_val(Value* val)
{
  this->code += memory->push_to_stack();
  this->code += val->construct(&counter);
  counter++;
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

  code += b->construct(&counter) + " ";
  code += memory->push_to_stack() + " ";
  code += a->construct(&counter) + " ";
  auto b_addr = memory->get_stack_top();
  code += "ADD " + to_string(b_addr) + "\n";
  memory->pop_from_stack();

  counter += 2;
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

  code += b->construct(&counter) + " ";
  code += memory->push_to_stack() + " ";
  code += a->construct(&counter) + " ";
  auto b_addr = memory->get_stack_top();
  code += "SUB " + to_string(b_addr) + "\n";
  memory->pop_from_stack();

  counter += 2;
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

  code += "SUB 0 ";
  code += memory->push_to_stack() + " ";
  auto result = memory->get_stack_top();
  code += "DEC ";
  code += memory->push_to_stack();
  auto neg_one = memory->get_stack_top();
  code += "INC INC ";
  code += memory->push_to_stack();

  counter += 7;

  auto one = memory->get_stack_top();
  code += b->construct(&counter) + " ";
  code += memory->push_to_stack() + " ";
  auto b_addr = memory->get_stack_top();
  code += a->construct(&counter) + " ";
  code += memory->push_to_stack() + " ";
  auto a_addr = memory->get_stack_top();

  counter += 2;


  code += "JPOS " + to_string(counter+7);
  code += "SUB 0 SUB " + to_string(b_addr) + "STORE " + to_string(b_addr) + " ";
  code += "SUB 0 SUB " + to_string(a_addr) + "STORE " + to_string(a_addr) + " ";
  counter += 7;

  auto start_counter = counter;
  code += "LOAD " + to_string(a_addr) + " SHIFT " + to_string(neg_one) + " SHIFT " + to_string(one) + " SUB " + to_string(a_addr) + " ";
  counter += 4;
  code += "JZERO " + to_string(counter + 4) + " ";
  code += "LOAD " + to_string(result) + "ADD " + to_string(b_addr) + " STORE " + to_string(result) + " ";
  counter += 4;
  
  code += "LOAD " + to_string(b_addr) + " SHIFT " + to_string(one) + " STORE " + to_string(b_addr) + " ";
  counter += 3;
  code += "LOAD " + to_string(a_addr) + " SHIFT " + to_string(neg_one) + " " + " STORE " + to_string(a_addr) + " ";
  counter += 3;
  
  code += "JZERO " + to_string(counter+2) + " ";
  code += "JUMP " + to_string(start_counter) + " ";

  code += "LOAD " + to_string(result) + " ";

  counter += 3;

  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
}

void Code::divide(Value* a, Value* b)
{
  if (b->is_constant() && ((Constant*) b)->value == 0)
  {
    Constant constant(0);
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
    Constant constant(0);
  }

  if (a->is_constant() && b->is_constant())
  {
    auto a_val = ((Constant*) a)->value;
    auto b_val = ((Constant*) b)->value;
    auto b_sign = (b_val > 0) - (b_val < 0);
    Constant constant(abs(a_val%b_val) * b_sign);
    construct_val(&constant);
    return;
  }

  // TODO figure out modulo
}