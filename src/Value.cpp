#include "Value.hpp"

string Value::construct()
{
  string result = "SUB 0 ";

  for (int i=0; i<this->constant; i++) {
    result += "INC ";
  }

  return result;
}