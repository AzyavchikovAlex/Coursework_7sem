#include "utils.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include <cassert>

std::filesystem::path GetStoragePath() {
  std::filesystem::path path;
  path /= "..";
  path /= "DataGenerator";
  path /= "Setups";
  path /= "MetaInfo";
  path /= "GeneratedTestsInfo.txt";
  return path;
}

std::unordered_map<TestInfo, size_t> GetAllTests() {
  auto path = GetStoragePath();
  std::ifstream in(path);
  std::unordered_map<TestInfo, size_t> answer;
  if (!in.is_open()) {
    return answer;
  }
  while (!in.eof()) {
    TestInfo info;
    size_t last_test_index;
    in >> info.name >> info.size >> last_test_index;
    if (info.name.empty()) {
      break;
    }
    answer[info] = last_test_index;
  }
  in.close();
  return answer;
}

void SaveTestInfo(const TestInfo& info, size_t last_test_index) {
  auto all_tests = GetAllTests();
  assert(!all_tests.contains(info) || all_tests[info] <= last_test_index);
  all_tests[info] = last_test_index;
  auto path = GetStoragePath();
  std::ofstream out(path);
  if (!out.is_open()) {
    throw std::runtime_error{path.string() + " - not found"};
  }
  for (const auto& test : all_tests) {
    out << test.first.name << " " << test.first.size << " " << test.second
        << "\n";
  }

  out.close();
}