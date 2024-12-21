#include "filesystem.h"

std::vector<std::filesystem::path> GetAllDirectoryFiles(const std::filesystem::path& dir) {
  std::vector<std::filesystem::path> answer;
  for (const auto& it : std::filesystem::directory_iterator{dir}) {
    if (it.is_regular_file()) {
      answer.push_back(it.path());
    }
  }
  return answer;
}

std::vector<std::filesystem::path> GetAllSubdirectories(const std::filesystem::path& dir) {
  std::vector<std::filesystem::path> answer;
  for (const auto& it : std::filesystem::directory_iterator{dir}) {
    if (it.is_directory()) {
      answer.push_back(it.path());
    }
  }
  return answer;
}
