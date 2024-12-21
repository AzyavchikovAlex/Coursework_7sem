#include <bits/stdc++.h>

#include "Utils/Random/random_event_generator.h"

void GenerateSetup(size_t workers_types_count,
                   size_t tasks_types_count,
                   Probability ability_probability,
                   size_t version) {
  assert(ability_probability >= 0 && ability_probability <= 1);
  RandomEventGenerator generator(std::vector<Probability>({ability_probability,
                                                           Probability(1)
                                                               - ability_probability})); // 0 - no ability, 1 - has ability
  std::vector<std::vector<int>>
      matrix(workers_types_count, std::vector<int>(tasks_types_count, 0));
  for (size_t i = 0; i < matrix.size(); ++i) {
    for (size_t j = 0; j < matrix[i].size(); ++j) {
      matrix[i][j] = int(generator.GetRandomEventIndex());
    }
  }

  Probability worker_probability = Probability(1) / workers_types_count;
  Probability task_probability = Probability(1) / tasks_types_count;

  int p = static_cast<int>(ability_probability * 100);
  std::filesystem::path output;
  output /= "..";
  output /= "DataGenerator";
  output /= "Setups";
  output /= "EqualProbability";
  if (!exists(output)) {
    std::filesystem::create_directory(output);
  }
  output /= "task-appearance=" + std::to_string(p) + "%";
  if (!exists(output)) {
    std::filesystem::create_directory(output);
  }
  output /= "setup"
      + std::to_string(version) + ".txt";
  std::ofstream out(output);

  out << workers_types_count << "\n";
  for (size_t i = 0; i < workers_types_count; ++i) {
    out << worker_probability << " ";
  }
  out << "\n" << tasks_types_count << "\n";
  for (size_t i = 0; i < tasks_types_count; ++i) {
    out << task_probability << " ";
  }
  out << "\n\n";
  for (size_t i = 0; i < matrix.size(); ++i) {
    out << std::accumulate(matrix[i].begin(), matrix[i].end(), int(0)) << "\n";
    for (size_t j = 0; j < matrix[i].size(); ++j) {
      if (matrix[i][j] != 0) {
        out << j << " ";
      }
    }
    out << "\n";
  }
  out.close();
}

int main() {
  size_t workers_types_count = 20;
  size_t tasks_types_count = 10;

  for (int p = 10; p < 100; p += 10) {
    p = 90;
    for (size_t i = 1; i <= 10; ++i) {
      GenerateSetup(workers_types_count,
                    tasks_types_count,
                    Probability(p) / 100,
                    i);
    }
    break;
  }

}