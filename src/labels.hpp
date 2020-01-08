#pragma once

struct ConditionLabel
{
  unsigned long long start;
  unsigned long long jump;

  ConditionLabel(unsigned long long start, unsigned long long jump);
};