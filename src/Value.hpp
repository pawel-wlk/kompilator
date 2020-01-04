#pragma once

#include <string>

using namespace std;

//abstract class
class Value
{
public:
  virtual string construct(unsigned int* counter) = 0;
  virtual bool is_constant() = 0;
};