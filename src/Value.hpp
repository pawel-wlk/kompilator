#pragma once

#include <string>

using namespace std;

//abstract class
class Value
{
public:
  virtual bool is_constant() = 0;
};