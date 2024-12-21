#pragma once

#include <vector>

#include "querry.h"

struct TestCase {
  std::vector<Query> queries;
  WorkerType max_worker_type;
  TaskTypeId max_task_type;
};