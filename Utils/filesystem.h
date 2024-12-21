#pragma once

#include <vector>
#include <filesystem>

std::vector<std::filesystem::path> GetAllDirectoryFiles(const std::filesystem::path& dir);

std::vector<std::filesystem::path> GetAllSubdirectories(const std::filesystem::path& dir);