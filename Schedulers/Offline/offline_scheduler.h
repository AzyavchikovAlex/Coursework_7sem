#pragma once

#include <unordered_set>

#include "Model/scheduler.h"

class OfflineScheduler : public SchedulerInterface {
 public:
  OfflineScheduler() = delete;
  OfflineScheduler(WorkerType maxWorkerType, TaskTypeId maxTaskType, size_t tasks_count);
  void AddWorkerTypeInfo(WorkerType type,
                         WorkerAbilities abilities) override;
  void AddWorker(Worker worker) override;
  void AddTask(Task task,
               std::function<void(Worker)> onScheduledCallback) override;

  void UpdateSelfTime(Time time) override;

 private:
  std::vector<std::vector<int64_t>> a_;
  std::vector<std::unordered_set<TaskTypeId>> worker_types_abilities_;
  std::vector<Worker> workers_;
  std::vector<Task> tasks_;
  std::vector<std::function<void(Worker)>> tasks_callbacks;
  const size_t tasks_count_;
  size_t current_task_index_{0};
  const int64_t infinity_cost_{10'000'000};
};
