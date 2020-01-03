#pragma once

#include <string>

using namespace std;

//abstract class
class Value
{
public:
  virtual string construct() = 0;
};