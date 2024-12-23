#pragma once

#include "Model/test_case.h"

class TestsGenerator {
 public:
  TestsGenerator() = default;

  virtual TestCase Generate() = 0;
};