#include "utils.h"

#include "DataGenerator/Setups/MetaInfo/utils.h"

Statistics GetStatistics(std::vector<FirstFail::Percent>& results) {
  if (results.empty()) {
    return Statistics{};
  }
  std::sort(results.begin(), results.end());
  FirstFail::Percent avg = 0;
  for (auto res : results) {
    avg += res;
  }
  avg /= results.size();

  FirstFail::Percent mid = results[results.size() / 2];
  if (results.size() % 2 == 0) {
    mid += results[1 + (results.size() / 2)];
    mid /= 2;
  }
  return Statistics{avg, mid};
}

// TODO: move this function to DataGenerator folder
std::vector<std::filesystem::path> GetTestsBySize(size_t size,
                                                  const std::string& name,
                                                  const std::filesystem::path& prefix) {
  auto all_tests = GetAllTests();
  size_t last_test = 0;
  if (auto it = all_tests.find({name, size}); it != all_tests.end()) {
    last_test = it->second;
  }
  std::vector<std::filesystem::path> answer;
  answer.reserve(last_test);
  for (size_t i = 1; i <= last_test; ++i) {
    std::filesystem::path path = prefix;
    path /= "Tests";
    path /= std::to_string(size);
    path /= name;
    path /= "test" + std::to_string(i) + ".txt";
    answer.push_back(path);
  }
  return answer;
}

