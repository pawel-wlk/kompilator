#include "Constant.hpp"

Constant::Constant(long long value)
{
  this->value = value;
}

bool Constant::is_constant()
{
  return true;
}
