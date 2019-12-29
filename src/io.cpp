#include "io.hpp"

string read(Variable var)
{
  return "GET STORE " + to_string(var.address);
}

string write()
{
  return "";
}