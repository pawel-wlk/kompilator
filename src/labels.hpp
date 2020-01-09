#pragma once

struct ConditionLabel
{
  unsigned long long start;
  unsigned long long jump;

  ConditionLabel(unsigned long long start, unsigned long long jump);
};

struct IfLabel
{
  unsigned long long else_jump;

  IfLabel(unsigned long long else_jump);
};

struct DoWhileLabel
{
  unsigned long long loop_start;

  DoWhileLabel(unsigned long long loop_start);
};