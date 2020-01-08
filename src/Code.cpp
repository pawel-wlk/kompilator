#include "Code.hpp"

Code::Code(Memory* memory)
{
  this->memory = memory;
}

ostream& operator<<(ostream& os, Code& c)
{
  for (auto op = c.operations.begin(); op != c.operations.end(); op++)
  {
    os << op->stringify() << endl;
  }

  os << "HALT";

  return os;
}

void Code::store(Variable* var)
{
  if (var->dependency == 0)
  {
    operations.emplace_back(STORE, var->address);
    return;
  }

  auto stored_val = memory->push_to_stack();
  auto tmp = memory->push_to_stack();

  operations.emplace_back(STORE, stored_val);

  Constant start(var->start);
  construct_val(&start);

  operations.emplace_back(STORE, tmp);
  operations.emplace_back(LOAD, var->dependency);
  operations.emplace_back(SUB, tmp);
  operations.emplace_back(STORE, tmp);

  operations.emplace_back(LOAD, stored_val);
  operations.emplace_back(STOREI, tmp);

  memory->pop_from_stack();
  memory->pop_from_stack();


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

  if (!var.is_dependent())
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

// TODO fix dependent variables in arithmetics
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

  construct_val(a);
  operations.emplace_back(STORE, memory->push_to_stack());
  construct_val(b);
  operations.emplace_back(ADD, memory->pop_from_stack());
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

  construct_val(b);
  operations.emplace_back(STORE, memory->push_to_stack());
  construct_val(a);
  operations.emplace_back(ADD, memory->pop_from_stack());
}


void Code::flip_sign(unsigned int addr)
{
  operations.emplace_back(SUB, 0);
  operations.emplace_back(SUB, addr);
  operations.emplace_back(STORE, addr);
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

  auto result = memory->push_to_stack();
  auto one = memory->push_to_stack();
  auto neg_one = memory->push_to_stack();
  auto a_addr = memory->push_to_stack();
  auto b_addr = memory->push_to_stack();

  operations.emplace_back(SUB, 0);
  operations.emplace_back(STORE, result);
  operations.emplace_back(DEC);
  operations.emplace_back(STORE, neg_one);
  operations.emplace_back(INC);
  operations.emplace_back(INC);
  operations.emplace_back(STORE, one);

  construct_val(b);
  operations.emplace_back(STORE, b_addr);
  construct_val(a);
  operations.emplace_back(STORE, a_addr);

  operations.emplace_back(JPOS, operations.size()+7);
  flip_sign(b_addr);
  flip_sign(a_addr);

  auto start_counter = operations.size();

  operations.emplace_back(LOAD, a_addr);
  operations.emplace_back(SHIFT, neg_one);
  operations.emplace_back(SHIFT, one);
  operations.emplace_back(SUB, a_addr);

  operations.emplace_back(JZERO, operations.size()+3);

  operations.emplace_back(LOAD, result);
  operations.emplace_back(ADD, b_addr);
  operations.emplace_back(STORE, result);
  
  operations.emplace_back(LOAD, b_addr);
  operations.emplace_back(SHIFT, one);
  operations.emplace_back(STORE, b_addr);
  
  operations.emplace_back(LOAD, a_addr);
  operations.emplace_back(SHIFT, neg_one);
  operations.emplace_back(STORE, a_addr);
  
  operations.emplace_back(JZERO, operations.size()+2);
  operations.emplace_back(JUMP, start_counter);

  operations.emplace_back(LOAD, result);

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