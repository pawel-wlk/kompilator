#pragma once

#include <string>

#include "Value.hpp"

using namespace std;

struct Constant: public Value
{
  Constant(long long value);

  long long value;

  bool is_constant();
};