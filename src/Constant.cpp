#include "Constant.hpp"

Constant::Constant(long long value)
{
  this->value = value;
}

string Constant::construct()
{
  string result = "SUB 0 ";

  for (int i=0; i<this->value; i++) {
    result += "INC ";
  }

  return result;
}