#include "io.hpp"


string read(Variable* var)
{
  return "GET STORE " + to_string(var->address);
}

string write(Value val)
{
  // return (val.variable == nullptr  ? val.construct() : val.variable->load()) + " PUT";
  return "";
}