#pragma once

#include <iostream>
#include <fstream>

#include "Model/scheduler.h"
#include "Model/test_case.h"

class MockScheduler : public SchedulerInterface {
 public:
  MockScheduler(WorkerType max_worker_type,
                TaskTypeId max_task_type,
                size_t queries_count,
                std::ofstream && out);
  ~MockScheduler() override;
  void AddWorkerTypeInfo(WorkerType type,
                         WorkerAbilities abilities) override;
  void AddWorker(Worker worker) override;
  void AddTask(Task task,
               std::function<void(Worker)> /*onScheduledCallback*/) override;

  void UpdateSelfTime(Time /*time*/) override;

  static TestCase ParseTestCase(std::ifstream in);

 private:
  std::ofstream out_;
  size_t queries_count_;
  size_t query_index_{0};
};