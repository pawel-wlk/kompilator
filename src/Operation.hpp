#pragma once

#include <string>

using namespace std;

enum Command 
{
  GET, PUT, LOAD, STORE, LOADI, STOREI, ADD, SUB, SHIFT, INC, DEC, JUMP, JPOS, JZERO, JNEG, HALT
};

struct Operation
{
  Command command;
  long long argument;
  string stringify();
  Operation(Command c);
  Operation(Command c, long long arg);
};