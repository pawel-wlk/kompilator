#include "labels.hpp"


ConditionLabel::ConditionLabel(unsigned long long start, unsigned long long jump)
{
  this->start = start;
  this->jump = jump;
}