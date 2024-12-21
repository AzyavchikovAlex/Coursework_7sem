#pragma once

#include <chrono>
#include <cstdint>

typedef int64_t TaskTypeId;
typedef int64_t TaskId;
typedef long double Probability; // >=0 && <= 1
typedef std::chrono::seconds Time;

struct TaskType {
  TaskTypeId id;
  Probability probability;
};

struct Task {
  TaskType type;

  TaskId id;
};

