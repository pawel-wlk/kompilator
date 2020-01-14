#include "labels.hpp"


ConditionLabel::ConditionLabel(unsigned long long start, unsigned long long jump)
{
  this->start = start;
  this->jump = jump;
}

IfLabel::IfLabel(unsigned long long else_jump)
{
  this->else_jump = else_jump;
}

DoWhileLabel::DoWhileLabel(unsigned long long loop_start)
{
  this->loop_start = loop_start;
}

ForLabel::ForLabel(Variable* iterator, ConditionLabel* condition, unsigned int end_loop_addr)
{
  this->iterator = iterator;
  this->condition = condition;
  this->end_loop_addr = end_loop_addr;
}

ForLabel::~ForLabel()
{
  delete condition;
}