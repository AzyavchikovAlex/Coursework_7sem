#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "task.h"

typedef int64_t WorkerType;
typedef int64_t WorkerId;

typedef std::vector<TaskType> WorkerAbilities;

struct Worker {
  WorkerType type;
  WorkerId id;
};

struct WorkerTypeInfo {
  WorkerType type;
  WorkerAbilities abilities;
};
