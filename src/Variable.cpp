#include "Variable.hpp"

string Variable::assign()
{
  return "";
}

string Variable::load()
{
  string result;

  if (dependency == 0)
  {
    result = "LOAD " + to_string(address);
  }
  else
  {
    result = "LOAD " + to_string(dependency) + " ";
    for (int i=0; i < this->start; i++)
    {
      result += "INC ";
    }

    result += "LOADI 0";
  }

  return result;
}