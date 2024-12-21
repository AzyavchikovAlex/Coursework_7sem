#include <iostream>
#include <fstream>
#include <filesystem>

#include "first_fail.h"
#include "Schedulers/Mock/mock_scheduler.h"
#include "Schedulers/Version1/scheduler_v1.h"
#include "Schedulers/Version2/scheduler_v2.h"
#include "Schedulers/Version3/scheduler_v3.h"
#include "utils.h"
#include "Utils/filesystem.h"

template<typename T>
Statistics MeasureModel(const std::filesystem::path& test_directory) {
  auto results = MeasureOnTests<T>(GetAllDirectoryFiles(test_directory));
  return GetStatistics(results);
}

int main() {
  int p = 90;
  std::vector<std::pair<Statistics, Statistics>> results;
  for (int setup = 1; setup <= 10; ++setup) {
    if (setup != 1 && setup != 3 && setup != 4 && setup != 7 && setup != 8 && setup != 9 && setup != 10) {
      continue;
    }
    // std::cerr << "Setup" << setup << "\n";
    // std::cerr << "SchedulerV1\n";
    auto s1 = MeasureModel<SchedulerV1>(
        "/home/alexazyavchikov/Documents/BSU/Coursework_7sem/Tests/100/EqualProbability/task-appearance="
            + std::to_string(p) + "%/setup" + std::to_string(setup));
    // std::cerr << "\nSchedulerV2\n";
    auto s2 = MeasureModel<SchedulerV2>(
        "/home/alexazyavchikov/Documents/BSU/Coursework_7sem/Tests/100/EqualProbability/task-appearance=" + std::to_string(p) + "%/setup"
            + std::to_string(setup));
    results.emplace_back(s1, s2);
  }

  std::sort(results.begin(), results.end(), [](auto& x, auto& y) {
    if (x.second.average != y.second.average) {
      return x.second.average < y.second.average;
    }
    return x.first.average < y.first.average;
  });
  results.resize(10);
  int i = 1;
  for (auto res : results) {
    std::cout << "Setup-" << i << " & ";
    ++i;
    std::cout << res.first.average << " & " << res.first.median << " & " << res.second.average << " & " << res.second.median << " \\\\ \\hline\n";
  }

  // std::string test_name ;
  // std::cout << "Enter test name:\n";
  // std::cin >> test_name;
  // size_t size;
  // std::cout << "Enter tests size:\n";
  // std::cin >> size;
  //
  // std::cerr << "SchedulerV1\n";
  // MeasureModel<SchedulerV1>(size, test_name);
  // std::cerr << "\nSchedulerV2\n";
  // MeasureModel<SchedulerV2>(size, test_name);
  // std::cerr << "\nSchedulerV3\n";
  // MeasureModel<SchedulerV3>(size, test_name);
}