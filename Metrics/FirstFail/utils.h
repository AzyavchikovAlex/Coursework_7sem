#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <unordered_set>

#include "first_fail.h"
#include "Schedulers/Mock/mock_scheduler.h"

template<typename T>
std::vector<FirstFail::Percent> MeasureOnTests(const std::vector<std::filesystem::path>& test_files) {
  std::vector<FirstFail::Percent> results;
  for (const auto& file : test_files) {
    std::ifstream in(file);
    if (!in.is_open()) {
      continue;
    }

    TestCase test = MockScheduler::ParseTestCase(std::move(in));
    results.push_back(FirstFail::Measure(
        std::make_shared<T>(test.max_worker_type, test.max_task_type),
        test));
  }
  return results;
}

struct Statistics {
  FirstFail::Percent average{0};
  FirstFail::Percent median{0};
};

Statistics GetStatistics(std::vector<FirstFail::Percent>& results);

std::vector<std::filesystem::path> GetTestsBySize(size_t size,
                                                  const std::string& name,
                                                  const std::filesystem::path& prefix = "");
