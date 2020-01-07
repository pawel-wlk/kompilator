#include "Code.hpp"

Code::Code(Memory* memory)
{
  this->memory = memory;
  this->code = "";
  counter = 0;
}

string Code::get_code()
{
  string result;
  for (auto op = operations.begin(); op != operations.end(); op++)
  {
    result += op->stringify() + "\n";
  }

  return result + "HALT";
}

void Code::store(Variable* var)
{
  if (var->dependency == 0)
  {
    operations.emplace_back(STORE, var->address);
    return;
  }

  // TODO store dependent vaiiables
  operations.emplace_back(STORE, memory->push_to_stack());
  operations.emplace_back(LOAD, var->dependency);
}

void Code::read(Variable* var)
{
  operations.emplace_back(GET);
  store(var);
}

void Code::write(Value* val)
{
  construct_val(val);
  operations.emplace_back(PUT);
}

void Code::assign(Variable* var)
{
  store(var);
}

void Code::construct_val(Value* val)
{
  if (val->is_constant())
  {
    Constant c = *(Constant*) val;

    bool is_negative = c.value < 0;

    for (int i=0; i<abs(c.value); i++)
    {
      operations.emplace_back(is_negative ? DEC : INC);
    }
    return;
  }

  Variable var = *(Variable*) val;

  if (var.dependency == 0)
  {
    operations.emplace_back(LOAD, var.address);
    return;
  }

  Constant start(var.start);
  construct_val(&start);
  auto start_addr = memory->push_to_stack();

  operations.emplace_back(STORE, start_addr);
  operations.emplace_back(LOAD, var.dependency);
  operations.emplace_back(SUB, start_addr);

  operations.emplace_back(LOADI, 0);

  memory->pop_from_stack();
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

  if (a->is_constant())
  {
    construct_val(a);
    operations.emplace_back(ADD, ((Variable*) b)->address);
    return;
  }

  construct_val(b);
  operations.emplace_back(ADD, ((Variable*) a)->address);
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

  construct_val(a);
  operations.emplace_back(SUB, ((Variable*) b)->address);
  return;
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

  // code += "SUB 0 ";
  // code += memory->push_to_stack() + " ";
  // auto result = memory->get_stack_top();
  // code += "DEC ";
  // code += memory->push_to_stack();
  // auto neg_one = memory->get_stack_top();
  // code += "INC INC ";
  // code += memory->push_to_stack();

  // counter += 7;

  // auto one = memory->get_stack_top();
  // code += b->construct(&counter) + " ";
  // code += memory->push_to_stack() + " ";
  // auto b_addr = memory->get_stack_top();
  // code += a->construct(&counter) + " ";
  // code += memory->push_to_stack() + " ";
  // auto a_addr = memory->get_stack_top();

  // counter += 2;


  // code += "JPOS " + to_string(counter+7);
  // code += "SUB 0 SUB " + to_string(b_addr) + "STORE " + to_string(b_addr) + " ";
  // code += "SUB 0 SUB " + to_string(a_addr) + "STORE " + to_string(a_addr) + " ";
  // counter += 7;

  // auto start_counter = counter;
  // code += "LOAD " + to_string(a_addr) + " SHIFT " + to_string(neg_one) + " SHIFT " + to_string(one) + " SUB " + to_string(a_addr) + " ";
  // counter += 4;
  // code += "JZERO " + to_string(counter + 4) + " ";
  // code += "LOAD " + to_string(result) + "ADD " + to_string(b_addr) + " STORE " + to_string(result) + " ";
  // counter += 4;
  
  // code += "LOAD " + to_string(b_addr) + " SHIFT " + to_string(one) + " STORE " + to_string(b_addr) + " ";
  // counter += 3;
  // code += "LOAD " + to_string(a_addr) + " SHIFT " + to_string(neg_one) + " " + " STORE " + to_string(a_addr) + " ";
  // counter += 3;
  
  // code += "JZERO " + to_string(counter+2) + " ";
  // code += "JUMP " + to_string(start_counter) + " ";

  // code += "LOAD " + to_string(result) + "\n";

  // counter += 3;

  // memory->pop_from_stack();
  // memory->pop_from_stack();
  // memory->pop_from_stack();
  // memory->pop_from_stack();
  // memory->pop_from_stack();
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

  // code += "SUB 0 ";
  // code += memory->push_to_stack() + " ";
  // auto result = memory->get_stack_top();
  // code += "INC ";
  // code += memory->push_to_stack();
  // auto multiple = memory->get_stack_top();
  // code += memory->push_to_stack();
  // auto one = memory->get_stack_top();
  // code += "DEC DEC ";
  // code += memory->push_to_stack();
  // auto neg_one = memory->get_stack_top();
  // code += a->construct(&counter) + " ";
  // code += memory->push_to_stack() + " ";
  // auto dividend = memory->get_stack_top();
  // code += b->construct(&counter) + " ";
  // code += memory->push_to_stack() + " ";
  // auto divisor = memory->get_stack_top();

  // counter += 10;

  // auto start_counter = counter;

  // code += "LOAD " + to_string(multiple) + " SHIFT " + to_string(one) + " STORE " + to_string(multiple) + " ";
  // code += "LOAD " + to_string(divisor) + " SHIFT " + to_string(one) + " STORE " + to_string(divisor) + " ";

  // code += "SUB " + to_string(dividend) + " ";
  // counter += 7;

  // code += "JNEG " + to_string(counter + 2);

  // code += "JUMP " + to_string(start_counter) + " ";
  // counter += 2;

  // start_counter = counter;

  // code += "LOAD " + to_string(dividend) + "SUB " + to_string(divisor);
  // counter += 2;
  // code += "JNEG " + to_string(counter+5) + " ";
  // code += "STORE " + to_string(dividend) + " ";
  // code += "LOAD " + to_string(result) + " ADD " + to_string(multiple) + " STORE " + to_string(result) + " ";
  // counter += 5;


  // code += "LOAD " + to_string(divisor) + " SHIFT " + to_string(neg_one) + " STORE " + to_string(divisor) + " ";
  // code += "LOAD " + to_string(multiple) + " SHIFT " + to_string(neg_one) + " STORE " + to_string(multiple) + " ";
  // counter += 6;
  
  // code += "JZERO " + to_string(counter+2);
  // code += "JUMP " + to_string(start_counter) + " ";

  // code += "LOAD " + to_string(result) + "\n";

  // counter += 3;

  // memory->pop_from_stack();
  // memory->pop_from_stack();
  // memory->pop_from_stack();
  // memory->pop_from_stack();
  // memory->pop_from_stack();
  // memory->pop_from_stack();
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