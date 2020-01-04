#include "Constant.hpp"

Constant::Constant(long long value)
{
  this->value = value;
}

bool Constant::is_constant()
{
  return true;
}

string Constant::construct(unsigned int* counter)
{
  string result = "SUB 0 ";

  for (int i=0; i<this->value; i++) {
    result += "INC ";
  }

  counter += value+1;

  return result;
}