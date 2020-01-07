#include "Operation.hpp"

string Operation::stringify()
{
  switch(this->command)
  {
  case GET:
    return "GET";
  case PUT:
    return "PUT";
  case LOAD:
    return "LOAD " + to_string(argument);
  case STORE:
    return "STORE " + to_string(argument);
  case LOADI:
    return "LOADI " + to_string(argument);
  case STOREI:
    return "STOREI " + to_string(argument);
  case ADD:
    return "ADD " + to_string(argument);
  case SUB:
    return "SUB " + to_string(argument);
  case SHIFT:
    return "SHIFT " + to_string(argument);
  case INC:
    return "INC";
  case DEC:
    return "DEC";
  case JUMP:
    return "JUMP " + to_string(argument);
  case JPOS:
    return "JPOS " + to_string(argument);
  case JZERO:
    return "JZERO " + to_string(argument);
  case JNEG:
    return "JNEG " + to_string(argument);
  case HALT:
    return "HALT";
  }

  return "#wrong operation";
}

Operation::Operation(Command c)
{
  command = c;
}

Operation::Operation(Command c, long long arg)
{
  command = c;
  argument = arg;
}