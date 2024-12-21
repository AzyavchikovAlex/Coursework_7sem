#pragma once

#include <vector>
#include <random>

#include "Model/task.h"

class RandomEventGenerator {
 public:
  RandomEventGenerator() = delete;
  explicit RandomEventGenerator(const std::vector<Probability>& probabilities);

  size_t GetRandomEventIndex() const;

 private:
  std::vector<Probability> sums_;
  mutable std::random_device rd;
  mutable std::mt19937_64 generator;
  mutable std::uniform_real_distribution<Probability> distrib{0, 1};
};