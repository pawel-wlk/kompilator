#include "Code.hpp"

#include<cmath>

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

void Code::construct_constants()
{
  memory->reserve_constants();
  operations.emplace_back(SUB, 0);
  operations.emplace_back(INC);
  operations.emplace_back(STORE, memory->get_variable("1")->address);
  operations.emplace_back(DEC);
  operations.emplace_back(DEC);
  operations.emplace_back(STORE, memory->get_variable("-1")->address);
}

void Code::generate_memory_offset(string pid)
{
  auto var = memory->get_variable(pid);
  memory->reserve_variable(pid + "/offset");
  auto offset_var = memory->get_variable(pid+"/offset");
  Constant offset(var->address - var->start);
  construct_val(&offset);

  operations.emplace_back(STORE, offset_var->address);
}

void Code::store(Variable* var)
{
  if (var->is_iterator)
  {
    throw (string) ("Modification of for loop iterator " +  var->name);
  }

  var->is_initialized = true;

  if (var->dependency == 0)
  {
    operations.emplace_back(STORE, var->address);
    return;
  }

  auto stored_val = memory->push_to_stack();
  auto tmp = memory->push_to_stack();

  operations.emplace_back(STORE, stored_val);

  operations.emplace_back(LOAD, var->dependency);
  operations.emplace_back(ADD, memory->get_variable(var->name + "/offset")->address);
  operations.emplace_back(STORE, tmp);

  operations.emplace_back(LOAD, stored_val);
  operations.emplace_back(STOREI, tmp);

  memory->pop_from_stack();
  memory->pop_from_stack();
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

    auto number = c.value;

    if (abs(number) < 32)
    {
      operations.emplace_back(SUB, 0);
      for (int i=0; i<abs(number); i++)
        operations.emplace_back(number > 0 ? INC : DEC);

      return;
    }

    auto one = memory->get_variable("1")->address;

    auto result = memory->push_to_stack();
    auto acc = memory->push_to_stack();

    operations.emplace_back(SUB, 0);
    operations.emplace_back(STORE, result);
    
    operations.emplace_back(number > 0 ? INC : DEC);

    number = abs(number);
    while (number >= 1)
    {
      if (number % 2 == 1)
      {
        operations.emplace_back(STORE, acc);
        operations.emplace_back(ADD, result);
        operations.emplace_back(STORE, result);
        operations.emplace_back(LOAD, acc);

      }
      operations.emplace_back(SHIFT, one);
      number /= 2;
    }

    operations.emplace_back(LOAD, result);

    memory->pop_from_stack();
    memory->pop_from_stack();

    return;
  }

  Variable var = *(Variable*) val;

  if (!var.is_initialized)
  {
    throw (string) (var.name + " is not initalizedd");
  }

  if (!var.is_dependent())
  {
    operations.emplace_back(LOAD, var.address);
    return;
  }

  operations.emplace_back(LOAD, var.dependency);
  operations.emplace_back(ADD, memory->get_variable(var.name+"/offset")->address);
  operations.emplace_back(LOADI, 0);
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

  if (!a->is_constant() && !((Variable*)a)->is_dependent())
  {
    construct_val(b);
    operations.emplace_back(ADD, ((Variable*)a)->address);
  }
  else if (!b->is_constant() && !((Variable*)b)->is_dependent())
  {
    construct_val(a);
    operations.emplace_back(ADD, ((Variable*)b)->address);
  }
  else
  {
    construct_val(a);
    operations.emplace_back(STORE, memory->push_to_stack());
    construct_val(b);
    operations.emplace_back(ADD, memory->pop_from_stack());
  }
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


  if (!b->is_constant() && !((Variable*)b)->is_dependent())
  {
    construct_val(a);
    operations.emplace_back(SUB, ((Variable*)b)->address);
  }
  else if (b->is_constant() && (llabs(((Constant*)b)->value) < 10))
  {
    construct_val(a);
    for (int i=0; i<llabs(((Constant*)b)->value); i++)
    {
      operations.emplace_back(((Constant*)b)->value > 0 ? DEC : INC);
    }
  }
  else {
    construct_val(b);
    operations.emplace_back(STORE, memory->push_to_stack());
    construct_val(a);
    operations.emplace_back(SUB, memory->pop_from_stack());
  }
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

  auto one = memory->get_variable("1")->address;
  auto neg_one = memory->get_variable("-1")->address;

  auto result = memory->push_to_stack();
  auto sign = memory->push_to_stack();
  auto a_addr = memory->push_to_stack();
  auto b_addr = memory->push_to_stack();

  operations.emplace_back(SUB, 0);
  operations.emplace_back(STORE, result);
  operations.emplace_back(STORE, sign);

  construct_val(b);
  operations.emplace_back(STORE, b_addr);
  operations.emplace_back(JPOS, operations.size()+7);
  flip_sign(b_addr);
  operations.emplace_back(LOAD, sign);
  operations.emplace_back(INC);
  operations.emplace_back(STORE, sign);

  construct_val(a);
  operations.emplace_back(STORE, a_addr);
  operations.emplace_back(JPOS, operations.size()+7);
  flip_sign(a_addr);
  operations.emplace_back(LOAD, sign);
  operations.emplace_back(DEC);
  operations.emplace_back(STORE, sign);

  // use smaller number as a multiplier
  operations.emplace_back(LOAD, a_addr);
  operations.emplace_back(SUB, b_addr);
  operations.emplace_back(JNEG, operations.size()+7);

  operations.emplace_back(LOAD, a_addr);
  operations.emplace_back(STORE, memory->push_to_stack());
  operations.emplace_back(LOAD, b_addr);
  operations.emplace_back(STORE, a_addr);
  operations.emplace_back(LOAD, memory->pop_from_stack());
  operations.emplace_back(STORE, b_addr);


  auto start_counter = operations.size();

  operations.emplace_back(LOAD, a_addr);
  operations.emplace_back(SHIFT, neg_one);
  operations.emplace_back(SHIFT, one);
  operations.emplace_back(SUB, a_addr);

  operations.emplace_back(JZERO, operations.size()+4);

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

  operations.emplace_back(LOAD, sign);

  operations.emplace_back(JZERO, operations.size()+4);

  operations.emplace_back(SUB, 0);
  operations.emplace_back(SUB, result);
  operations.emplace_back(JUMP, operations.size()+2);
  
  operations.emplace_back(LOAD, result);


  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
}

void Code::division_core(unsigned int scaled_divisor, unsigned int remain, unsigned int one, unsigned int neg_one, unsigned int result, unsigned int multiple, unsigned int sign)
{
  operations.emplace_back(LOAD, scaled_divisor);
  auto zero_divisor_jump = operations.size();
  operations.emplace_back(JZERO, 0);
  operations.emplace_back(SUB, remain);
  auto equal_jump = operations.size();
  operations.emplace_back(JZERO, 0);
  // prepare variables
  operations.emplace_back(SUB, 0);
  operations.emplace_back(STORE, result);
  operations.emplace_back(STORE, sign);
  operations.emplace_back(INC);
  operations.emplace_back(STORE, multiple);

  // prepare signs
  operations.emplace_back(LOAD, remain);
  operations.emplace_back(JPOS, operations.size()+7);
  operations.emplace_back(LOAD, sign);
  operations.emplace_back(INC);
  operations.emplace_back(STORE, sign);
  flip_sign(remain);

  operations.emplace_back(LOAD, scaled_divisor);
  operations.emplace_back(JPOS, operations.size()+7);
  operations.emplace_back(LOAD, sign);
  operations.emplace_back(DEC);
  operations.emplace_back(STORE, sign);
  flip_sign(scaled_divisor);

  auto scaling_loop_start = operations.size();
  operations.emplace_back(LOAD, scaled_divisor);
  operations.emplace_back(SUB, remain);
  operations.emplace_back(JPOS, operations.size()+8);
  operations.emplace_back(LOAD, multiple);
  operations.emplace_back(SHIFT, one);
  operations.emplace_back(STORE, multiple);
  operations.emplace_back(LOAD, scaled_divisor);
  operations.emplace_back(SHIFT, one);
  operations.emplace_back(STORE, scaled_divisor);
  operations.emplace_back(JUMP, scaling_loop_start);

  auto dividing_loop_start = operations.size();

  operations.emplace_back(LOAD, remain);
  operations.emplace_back(SUB, scaled_divisor);
  operations.emplace_back(JNEG, operations.size()+5);
  operations.emplace_back(STORE, remain);
  operations.emplace_back(LOAD, result);
  operations.emplace_back(ADD, multiple);
  operations.emplace_back(STORE, result);

  operations.emplace_back(LOAD, scaled_divisor);
  operations.emplace_back(SHIFT, neg_one);
  operations.emplace_back(STORE, scaled_divisor);
  operations.emplace_back(LOAD, multiple);
  operations.emplace_back(SHIFT, neg_one);
  operations.emplace_back(STORE, multiple);
  operations.emplace_back(JZERO, operations.size()+2);
  operations.emplace_back(JUMP, dividing_loop_start);

  operations.emplace_back(JUMP, operations.size()+7);
  operations[zero_divisor_jump].argument = operations.size();
  operations.emplace_back(STORE, remain);
  operations.emplace_back(STORE, result);

  operations.emplace_back(JUMP, operations.size()+4);
  operations[equal_jump].argument = operations.size();
  operations.emplace_back(STORE, remain);
  operations.emplace_back(INC);
  operations.emplace_back(STORE, result);
}
void Code::divide(Value* a, Value* b)
{
  if (b->is_constant() && ((Constant*) b)->value == 0)
  {
    Constant constant(0);
    construct_val(&constant);
    return;
  }

  if (a->is_constant() && b->is_constant())
  {
    auto a_val = ((Constant*) a)->value;
    auto b_val = ((Constant*) b)->value;
    Constant constant(floor(((float)a_val) / ((float)b_val)));
    construct_val(&constant);
    return;
  }

  if (b->is_constant())
  {
    auto b_val = ((Constant*) b)->value;
    auto log2b = log2(b_val);

    if (ceil(log2b) == floor(log2b))
    {
      construct_val(a);
      auto neg_one = memory->get_variable("-1")->address;
      for (int i=0; i<log2b; i++)
        operations.emplace_back(SHIFT, neg_one);
      return;
    }
  }

  auto one = memory->get_variable("1")->address;
  auto neg_one = memory->get_variable("-1")->address;

  auto scaled_divisor = memory->push_to_stack();
  auto remain = memory->push_to_stack();
  auto result = memory->push_to_stack();
  auto multiple = memory->push_to_stack();
  auto sign = memory->push_to_stack(); // if 0 result should be positive, otherwise negative

  construct_val(a);
  operations.emplace_back(STORE, remain);
  construct_val(b);
  operations.emplace_back(STORE, scaled_divisor);

  division_core(scaled_divisor, remain, one, neg_one, result, multiple, sign);

  // flip sign if needed
  operations.emplace_back(LOAD, sign);
  operations.emplace_back(JZERO, operations.size()+9);
  flip_sign(result);
  
  // handle negative division rounding
  operations.emplace_back(LOAD, remain);
  operations.emplace_back(JZERO, operations.size()+4);
  operations.emplace_back(LOAD, result);
  operations.emplace_back(DEC);
  operations.emplace_back(JUMP, operations.size()+2);


  operations.emplace_back(LOAD, result);

  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
}

void Code::modulo(Value* a, Value* b)
{
  if (b->is_constant() && ((Constant*) b)->value == 0)
  {
    Constant constant(0);
    construct_val(&constant);
    return;
  }

  if (a->is_constant() && b->is_constant())
  {
    auto a_val = ((Constant*) a)->value;
    auto b_val = ((Constant*) b)->value;
    Constant constant((a_val % b_val + b_val) % b_val);
    construct_val(&constant);
    return;
  }

  if (b->is_constant() && llabs(((Constant*) b)->value) == 2)
  {
    auto one = memory->get_variable("1")->address;
    auto neg_one = memory->get_variable("-1")->address;
    auto a_addr = memory->push_to_stack();

    construct_val(a);
    operations.emplace_back(STORE, a_addr);

    operations.emplace_back(SHIFT, neg_one);
    operations.emplace_back(SHIFT, one);
    operations.emplace_back(SUB, a_addr);

    if (((Constant*) b)-> value > 0)
    {
      operations.emplace_back(JZERO, operations.size()+3);
      operations.emplace_back(INC);
      operations.emplace_back(INC);
    }

    memory->pop_from_stack();

    return;
  }

  auto one = memory->get_variable("1")->address;
  auto neg_one = memory->get_variable("-1")->address;

  auto scaled_divisor = memory->push_to_stack();
  auto remain = memory->push_to_stack();
  auto result = memory->push_to_stack();
  auto multiple = memory->push_to_stack();
  auto sign = memory->push_to_stack();

  construct_val(a);
  operations.emplace_back(STORE, remain);
  construct_val(b);
  operations.emplace_back(STORE, scaled_divisor);

  division_core(scaled_divisor, remain, one, neg_one, result, multiple, sign);

  // modulo if  else shittery
  operations.emplace_back(LOAD, remain);
  auto remain_zero_jump = operations.size();
  operations.emplace_back(JZERO, 0);


  construct_val(a);
  
  operations.emplace_back(JPOS, operations.size()+4);
  flip_sign(remain);

  operations.emplace_back(LOAD, sign);
  auto jump_to_end = operations.size();
  operations.emplace_back(JZERO, 0);

  construct_val(b);

  operations.emplace_back(ADD, remain);
  operations.emplace_back(STORE, remain);
  operations[jump_to_end].argument = operations.size();
  operations[remain_zero_jump].argument = operations.size();

  operations.emplace_back(LOAD, remain);


  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
  memory->pop_from_stack();
}

ConditionLabel* Code::equal(Value* a, Value* b)
{
  auto start_addr = operations.size();
  subtract(a, b);

  operations.emplace_back(JZERO, operations.size()+2);
  operations.emplace_back(JUMP, 0); // replace later with address of block's end


  return new ConditionLabel(start_addr, operations.size()-1);
}

ConditionLabel* Code::not_equal(Value* a, Value* b)
{
  auto start_addr = operations.size();
  subtract(a, b);

  operations.emplace_back(JZERO, 0); // replace later with address of block's end

  return new ConditionLabel(start_addr, operations.size()-1);
}

ConditionLabel* Code::greater_or_equal(Value* a, Value* b)
{
  auto start_addr = operations.size();
  subtract(b, a);

  operations.emplace_back(JPOS, 0); // replace later with address of block's end

  return new ConditionLabel(start_addr, operations.size()-1);
}

ConditionLabel* Code::less_or_equal(Value* a, Value* b)
{
  auto start_addr = operations.size();
  subtract(b, a);

  operations.emplace_back(JNEG, 0); // replace later with address of block's end

  return new ConditionLabel(start_addr, operations.size()-1);
}

ConditionLabel* Code::greater(Value* a, Value* b)
{
  auto start_addr = operations.size();
  subtract(b, a);

  operations.emplace_back(JNEG, operations.size()+2);
  operations.emplace_back(JUMP, 0); // replace later with address of block's end

  return new ConditionLabel(start_addr, operations.size()-1);
}

ConditionLabel* Code::less(Value* a, Value* b)
{
  auto start_addr = operations.size();
  subtract(b, a);

  operations.emplace_back(JPOS, operations.size()+2);
  operations.emplace_back(JUMP, 0); // replace later with address of block's end

  return new ConditionLabel(start_addr, operations.size()-1);
}


void Code::single_if(ConditionLabel* condition)
{
  operations[condition->jump].argument = operations.size();

  delete condition;
}

void Code::while_loop(ConditionLabel* condition)
{
  operations.emplace_back(JUMP, condition->start);
  operations[condition->jump].argument = operations.size();

  delete condition;
}

IfLabel* Code::if_else_first(ConditionLabel* condition)
{
  operations.emplace_back(JUMP, 0);
  operations[condition->jump].argument = operations.size();

  return new IfLabel(operations.size() - 1);
}

void Code::if_else_second(IfLabel* if_lbl)
{
  operations[if_lbl->else_jump].argument = operations.size();

  delete if_lbl;
}

DoWhileLabel* Code::do_loop_first()
{
  return new DoWhileLabel(operations.size());
}

void Code::do_loop_second(DoWhileLabel* do_lbl, ConditionLabel* condition)
{
  operations[condition->jump].argument = operations.size() + 1;

  operations.emplace_back(JUMP, do_lbl->loop_start);

  delete do_lbl;
  delete condition;
}

ForLabel* Code::for_loop_first(string iterator_name, Value* start, Value* end, bool ascending)
{
  auto iterator = memory->reserve_iterator(iterator_name);
  auto end_addr = memory->push_to_stack();
  construct_val(end);
  operations.emplace_back(STORE, end_addr);
  construct_val(start);
  operations.emplace_back(STORE, iterator->address);

  // auto condition = ascending ? less_or_equal(iterator, end) : greater_or_equal(iterator, end);
  auto considtion_start_addr = operations.size();
  operations.emplace_back(SUB, end_addr);
  auto jump_addr = operations.size();
  operations.emplace_back(ascending ? JPOS : JNEG, 0);

  auto condition = new ConditionLabel(considtion_start_addr, jump_addr);

  return new ForLabel(iterator, condition, end_addr);
}


void Code::for_loop_second(ForLabel* loop, bool ascending)
{
  operations.emplace_back(LOAD, loop->iterator->address);
  operations.emplace_back(ascending ? INC : DEC);
  operations.emplace_back(STORE, loop->iterator->address);

  operations.emplace_back(JUMP, loop->condition->start);
  operations[loop->condition->jump].argument = operations.size();

  memory->pop_from_stack();
  memory->destroy_iterator(loop->iterator->name);

  delete loop;
}
