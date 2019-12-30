#pragma once

#include <string>

#include "Variable.hpp"

using namespace std;

struct Value
{
  long long constant;
  Variable* variable;

  string construct();
};