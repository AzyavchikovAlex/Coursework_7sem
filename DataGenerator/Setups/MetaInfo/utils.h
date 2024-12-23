#pragma once

#include <string>
#include <unordered_map>

struct TestInfo {
  std::string name;
  size_t size;

  bool operator==(const TestInfo& info) const {
    return name == info.name && size == info.size;
  }
};

template<>
struct std::hash<TestInfo> {
  std::size_t operator()(const TestInfo& info) const {
    return std::hash<std::string>()(info.name) ^ info.size;
  }
};

std::unordered_map<TestInfo, size_t> GetAllTests();

void SaveTestInfo(const TestInfo& info, size_t last_test_index);
