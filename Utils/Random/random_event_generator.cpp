#include "random_event_generator.h"

#include <cassert>
#include <algorithm>

RandomEventGenerator::RandomEventGenerator(
    const std::vector<Probability>& probabilities
) : sums_(probabilities.size(), 0), rd(), generator(rd()) {
  assert(!probabilities.empty());
  sums_[0] = 0;
  for (size_t i = 1; i < probabilities.size(); ++i) {
    assert(probabilities[i] >= 0 && probabilities[i] <= 1);
    sums_[i] = sums_[i - 1] + probabilities[i - 1];
  }
  assert(sums_.back() + probabilities.back() <= 1 + 0.0001);
  sums_.push_back(2);
}

size_t RandomEventGenerator::GetRandomEventIndex() const {
  Probability searching_value = distrib(generator);
  assert(searching_value >= 0 && searching_value <= 1);
  auto it = std::upper_bound(sums_.begin(), sums_.end(), searching_value);
  size_t pos = std::distance(sums_.begin(), it);
  assert(pos > 0);
  return pos - 1;
}
